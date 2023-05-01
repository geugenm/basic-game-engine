#pragma once

#include <filesystem>
#include <utility>

#include "../textures/texture.hxx"
#include "abstract_shape.hxx"
#include "line_2d.hxx"

#include "../shaders/gfx_program.hxx"

#include "../textures/ppm_handler.hxx"

class Polygon2D : public Shape2D
{
  public:
    explicit Polygon2D(const BoundingBox &box, const size_t &sides_amount)
    {
        set_bounding_box(box);
        init_random(sides_amount);
    }

    explicit Polygon2D(const BoundingBox &box, const Vertices &vertices)
    {
        set_bounding_box(box);
        set_vertices(vertices);
    }

    Polygon2D(const Position2D &start, const Position2D &end, const size_t &sides_amount)
    {
        set_bounding_box(BoundingBox(start, end));
        init_random(sides_amount);
    }

    Polygon2D(const Polygon2D &other) : Shape2D(other)
    {
        set_vertices(other.get_vertices());
        set_bounding_box(other.get_bounding_box());
    }

    Polygon2D(Polygon2D &&other) noexcept
    {
        set_vertices(other.get_vertices());
        set_bounding_box(other.get_bounding_box());
    }

    ~Polygon2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) override
    {
        for (size_t i = 0; i < access_vertices().size(); ++i)
        {
            const Position2D &p1 = access_vertices()[i];
            const Position2D &p2 = access_vertices()[(i + 1) % access_vertices().size()];
            Line2D l(p1, p2);
            l.draw_on(texture, color);
        }
    }

    void interpolate(Texture &texture, const ColorRGB & gradient_begin, const ColorRGB & gradient_end)
    {
        // Get the minimum and maximum y-coordinates of the polygon vertices
        auto vertices = get_vertices();
        auto [min_y, max_y] = std::ranges::minmax_element(vertices, {}, [](const auto &vertex) { return vertex.y; });

        // Loop over all rows of pixels within the polygon's bounding box
        for (int y = min_y->y; y <= max_y->y; ++y)
        {
            std::vector<std::pair<int, int>> intersections;
            compute_edge_intersections(y, vertices, intersections);

            // Sort the intersecting edges by their x-coordinate at the point of intersection with the current row
            std::sort(intersections.begin(), intersections.end());

            // Fill in the pixels between adjacent pairs of intersecting edges
            for (size_t i = 0; i < intersections.size(); i += 2)
            {
                const int x_start = std::max(0, intersections[i].first);
                const int x_end = std::min(static_cast<int>(get_bounding_box().width) - 1, intersections[i + 1].first);
                for (int x = x_start; x <= x_end; ++x)
                {
                    double t = (static_cast<double>(x) - intersections[i].first) / (intersections[i + 1].first - intersections[i].first);
                    ColorRGB interpolated_color = ColorRGB::interpolate_linearly(gradient_begin, gradient_end, t);
                    texture.set_pixel({x, y}, interpolated_color);
                }
            }
        }
    }

    void apply_shader(Texture &texture, GFX::GFXProgram & gfx_program)
    {
        // Get the minimum and maximum y-coordinates of the polygon vertices
        auto vertices = get_vertices();
        auto [min_y, max_y] = std::ranges::minmax_element(vertices, {}, [](const auto &vertex) { return vertex.y; });

        // Loop over all rows of pixels within the polygon's bounding box
        for (int y = min_y->y; y <= max_y->y; ++y)
        {
            std::vector<std::pair<int, int>> intersections;
            compute_edge_intersections(y, vertices, intersections);

            // Sort the intersecting edges by their x-coordinate at the point of intersection with the current row
            std::sort(intersections.begin(), intersections.end());

            Vertices vertices1;
            for (auto & intersection : intersections) {
                vertices1.emplace_back(intersection.first, y);
            }

            for (auto & vertex : vertices1) {
                vertex = gfx_program.vertex_shader(vertex);
                vertex.color = gfx_program.fragment_shader(vertex);
            }

            if (vertices1.size() != intersections.size()) {
                throw std::invalid_argument("Sizes do not match.");
            }

            // Fill in the pixels between adjacent pairs of intersecting edges
            for (size_t i = 0; i < intersections.size(); i += 2)
            {
                const int x_start = std::max(0, intersections[i].first);
                const int x_end = std::min(static_cast<int>(get_bounding_box().width) - 1, intersections[i + 1].first);
                ColorRGB gradient_begin = vertices1[i].color;
                ColorRGB gradient_end = vertices1[i + 1].color;
                for (int x = x_start; x <= x_end; ++x)
                {
                    double t = (static_cast<double>(x) - intersections[i].first) / (intersections[i + 1].first - intersections[i].first);
                    ColorRGB interpolated_color = ColorRGB::interpolate_linearly(gradient_begin, gradient_end, t);
                    texture.set_pixel({x, y}, interpolated_color);
                }
            }
        }
    }


    void fill(Texture &texture, const ColorRGB &color)
    {
        // Get the minimum and maximum y-coordinates of the polygon vertices
        auto vertices = get_vertices();
        auto [min_y, max_y] = std::ranges::minmax_element(vertices, {}, [](const auto &vertex) { return vertex.y; });

        // Loop over all rows of pixels within the polygon's bounding box
        for (int y = min_y->y; y <= max_y->y; ++y)
        {
            std::vector<std::pair<int, int>> intersections;
            compute_edge_intersections(y, vertices, intersections);

            // Sort the intersecting edges by their x-coordinate at the point of intersection with the current row
            std::sort(intersections.begin(), intersections.end());

            // Fill in the pixels between adjacent pairs of intersecting edges
            for (size_t i = 0; i < intersections.size(); i += 2)
            {
                const int x_start = std::max(0, intersections[i].first);
                const int x_end = std::min(static_cast<int>(get_bounding_box().width) - 1, intersections[i + 1].first);
                for (int x = x_start; x <= x_end; ++x)
                {
                    texture.set_pixel({x, y}, color);
                }
            }
        }
    }

    [[nodiscard]] std::string string() const override
    {
        return Shape2D::string();
    }

    void add_vertex(const Position2D &position)
    {
        for (const auto &vertex : access_vertices())
        {
            if (vertex == position)
            {
                return;
            }
        }
        access_vertices().reserve(access_vertices().size() + 1);
        access_vertices().push_back(position);
    }

  private:
    void init_random(const size_t &sides_amount)
    {
        double angle = 2.0 * M_PI / static_cast<double>(sides_amount);

        access_vertices().resize(sides_amount);
        int32_t radius_x = static_cast<int32_t>(access_bounding_box().width) / 2;
        int32_t radius_y = static_cast<int32_t>(access_bounding_box().height) / 2;

        if (radius_x != 0)
        {
            radius_x -= 1;
        }

        if (radius_y != 0)
        {
            radius_y -= 1;
        }

        Position2D center = {radius_x, radius_y};

        for (size_t i = 0; i < sides_amount; ++i)
        {
            const double theta = angle * static_cast<double>(i);
            const double x = center.x + radius_x * std::cos(theta);
            const double y = center.y + radius_y * std::sin(theta);
            access_vertices()[i] = {static_cast<int>(x), static_cast<int>(y)};
        }
    }

    static void compute_edge_intersections(int y, const std::vector<Position2D>& vertices, std::vector<std::pair<int, int>>& intersections)
    {
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            const Position2D &p1 = vertices[i];
            const Position2D &p2 = vertices[(i + 1) % vertices.size()];

            const bool edge_starts_above = p1.y > y;
            const bool edge_ends_below = p2.y <= y;

            const bool edge_starts_below = p1.y <= y;
            const bool edge_ends_above = p2.y > y;

            if ((edge_starts_above && edge_ends_below) || (edge_starts_below && edge_ends_above))
            {
                // Compute the x-coordinate of the edge at the point of intersection with the current row
                const double slope = static_cast<double>(p2.x - p1.x) / static_cast<double>(p2.y - p1.y);
                const double x = p1.x + slope * (y - p1.y);
                intersections.emplace_back(static_cast<int>(x), static_cast<int>(i));
            }
        }
    }
};
