#pragma once

#include <filesystem>
#include <utility>

#include "render/shapes/abstract_shape.hxx"
#include "render/shapes/line_2d.hxx"
#include "render/textures/texture.hxx"

#include "render/textures/ppm_handler.hxx"

class Polygon2D final : public Shape2D
{
  public:
    explicit Polygon2D(BoundingBox box, const size_t &sides_amount) : bounding_box_(std::move(box))
    {
        init_random(sides_amount);
    }

    explicit Polygon2D(BoundingBox box, Vertices vertices)
        : bounding_box_(std::move(box)), vertices_(std::move(vertices))
    {
    }

    Polygon2D(const Position2D &start, const Position2D &end, const size_t &sides_amount)
    {
        bounding_box_ = BoundingBox(start, end);
        init_random(sides_amount);
    }

    Polygon2D(const Polygon2D &other) : Shape2D(other)
    {
        init_random(other.get_vertices().size());
    }

    Polygon2D(Polygon2D &&other) noexcept
        : bounding_box_(std::move(other.bounding_box_)), vertices_(std::move(other.vertices_))
    {
    }

    Polygon2D &operator=(const Polygon2D &other)
    {
        if (this != &other)
        {
            bounding_box_ = other.bounding_box_;
            vertices_ = other.vertices_;
        }
        return *this;
    }

    Polygon2D &operator=(Polygon2D &&other) noexcept
    {
        if (this != &other)
        {
            bounding_box_ = std::move(other.bounding_box_);
            vertices_ = std::move(other.vertices_);
        }
        return *this;
    }

    ~Polygon2D() override = default;

    void draw_mesh(Texture &texture)
    {
        const auto &shape = texture.get_shape();

        constexpr float kMeshDensity = 0.02f;
        const auto step = static_cast<size_t>((float)std::max(shape.width, shape.height) * kMeshDensity);

        for (std::size_t y = 0; y < shape.height; y += step)
        {
            Line2D line({0, static_cast<int>(y)}, {static_cast<int>(shape.width - 1), static_cast<int>(y)});
            line.draw_on(texture, {0, 255, 255});
        }

        for (std::size_t x = 0; x < shape.width; x += step)
        {
            Line2D line({static_cast<int>(x), 0}, {static_cast<int>(x), static_cast<int>(shape.height - 1)});
            line.draw_on(texture, {0, 255, 255});
        }
    }

    void draw_on(Texture &texture, const ColorRGB &color) override
    {
        draw_mesh(texture);
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            const Position2D &p1 = vertices_[i];
            const Position2D &p2 = vertices_[(i + 1) % vertices_.size()];
            Line2D l(p1, p2);
            l.draw_on(texture, color);
        }
    }

    [[nodiscard]] std::unique_ptr<Shape2D> clone() const override
    {
        return std::make_unique<Polygon2D>(*this);
    }

    [[nodiscard]] std::string string() const override
    {
        return bounding_box_.string();
    }

    void rasterize()
    {
    }

    void add_vertex(const Position2D &position)
    {
        for (const auto &vertex : vertices_)
        {
            if (vertex == position)
            {
                return;
            }
        }
        vertices_.reserve(vertices_.size() + 1);
        vertices_.push_back(position);
    }

    [[nodiscard]] Vertices get_vertices() const override
    {
        return vertices_;
    }

    /// TODO: make sure that everything drawn up
    void triangulate()
    {
        if (vertices_.size() < 3)
        {
            return;
        }

        std::vector<size_t> indices(vertices_.size());
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            indices[i] = i;
        }

        std::vector<bool> ear(vertices_.size());
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            ear[i] = is_ear(i);
        }

        Texture texture;
        texture.set_shape(bounding_box_);

        size_t remaining_vertices = vertices_.size();
        while (remaining_vertices > 3)
        {
            // Find the first ear
            size_t ear_index = 0;
            while (!ear[ear_index])
            {
                ear_index++;
            }

            draw_on(texture, {255, 255, 0});

            // Remove the ear
            remove_ear(ear_index);
            ear.erase(ear.begin() + static_cast<long>(ear_index));
            indices.erase(indices.begin() + static_cast<long>(ear_index));

            // Update the ears of the neighboring vertices
            size_t previous_index = (ear_index == 0) ? (vertices_.size() - 1) : (ear_index - 1);
            size_t next_index = ear_index % (vertices_.size() - 1);
            ear[previous_index] = is_ear(previous_index);
            ear[next_index] = is_ear(next_index);

            remaining_vertices--;
        }

        draw_on(texture, {255, 255, 0});
        PpmHandler ppm_handler("12345.ppm", texture);
        ppm_handler.save();

    }

    bool has_vertex_on(const Position2D &position)
    {
        if (std::ranges::any_of(vertices_, [&](const auto &vertex) { return vertex == position; }))
        {
            return true;
        }
        return false;
    }

    [[nodiscard]] bool is_ear(const size_t &index) const
    {
        if (is_reflex(index))
        {
            return false;
        }

        const auto &prev_vertex = vertices_[(index + vertices_.size() - 1) % vertices_.size()];
        const auto &curr_vertex = vertices_[index];
        const auto &next_vertex = vertices_[(index + 1) % vertices_.size()];

        Polygon2D triangle(bounding_box_, {prev_vertex, curr_vertex, next_vertex});

        for (const auto &vertex : vertices_)
        {
            const bool is_current_vertex = (&vertex == &curr_vertex);
            const bool is_prev_vertex = (&vertex == &prev_vertex);
            const bool is_next_vertex = (&vertex == &next_vertex);

            if (is_current_vertex || is_prev_vertex || is_next_vertex)
            {
                continue;
            }

            if (triangle.has_vertex_on(vertex))
            {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] bool is_reflex(size_t index) const
    {
        const Position2D &p1 = vertices_[(index + vertices_.size() - 1) % vertices_.size()];
        const Position2D &p2 = vertices_[index];
        const Position2D &p3 = vertices_[(index + 1) % vertices_.size()];

        double cross_product = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);

        return (cross_product < 0);
    }

    void remove_ear(size_t index)
    {
        vertices_.erase(vertices_.begin() + static_cast<long>(index));
    }

  private:
    void init_random(const size_t &sides_amount)
    {
        double angle = 2.0 * M_PI / static_cast<double>(sides_amount);

        vertices_.resize(sides_amount);
        int32_t radius_x = static_cast<int32_t>(bounding_box_.width) / 2;
        int32_t radius_y = static_cast<int32_t>(bounding_box_.height) / 2;

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
            vertices_[i] = {static_cast<int>(x), static_cast<int>(y)};
        }
    }

    BoundingBox bounding_box_;

    Vertices vertices_;
};
