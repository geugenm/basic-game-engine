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
        : bounding_box_(std::move(other.bounding_box_)), triangles_(std::move(other.triangles_)),
          vertices_(std::move(other.vertices_))
    {
    }

    Polygon2D &operator=(const Polygon2D &other)
    {
        if (this != &other)
        {
            bounding_box_ = other.bounding_box_;
            vertices_ = other.vertices_;
            triangles_ = other.triangles_;
        }
        return *this;
    }

    Polygon2D &operator=(Polygon2D &&other) noexcept
    {
        if (this != &other)
        {
            bounding_box_ = std::move(other.bounding_box_);
            vertices_ = std::move(other.vertices_);
            triangles_ = std::move(other.triangles_);
        }
        return *this;
    }

    ~Polygon2D() override = default;

    void draw_on(Texture &texture, const ColorRGB &color) override
    {
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            const Position2D &p1 = vertices_[i];
            const Position2D &p2 = vertices_[(i + 1) % vertices_.size()];
            Line2D l(p1, p2);
            l.draw_on(texture, color);
        }

        if (triangles_.empty())
        {
            return;
        }
        for (auto &triangle : triangles_)
        {
            triangle.draw_on(texture, color);
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
        texture.set_shape({200, 200});

        size_t remaining_vertices = vertices_.size();
        while (remaining_vertices > 3)
        {
            // Find the first ear
            size_t ear_index = 0;
            while (!ear[ear_index])
            {
                ear_index++;
            }

            draw_on(texture, {0, 255, 255});

            // Remove the ear
            remove_ear(ear_index);
            ear.erase(ear.begin() + ear_index);
            indices.erase(indices.begin() + ear_index);

            // Update the ears of the neighboring vertices
            size_t previous_index = (ear_index == 0) ? (vertices_.size() - 1) : (ear_index - 1);
            size_t next_index = ear_index % (vertices_.size() - 1);
            ear[previous_index] = is_ear(previous_index);
            ear[next_index] = is_ear(next_index);

            remaining_vertices--;
        }

        draw_on(texture, {0, 255, 255});

        std::filesystem::path path;
        path = "test.ppm";
        PpmHandler handler(path, texture);
        handler.save();
    }

    bool contains(const Position2D &position)
    {
        for (auto &vertex : vertices_)
        {
            if (vertex == position)
            {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool is_ear(size_t index) const
    {
        const Position2D &p1 = vertices_[(index + vertices_.size() - 1) % vertices_.size()];
        const Position2D &p2 = vertices_[index];
        const Position2D &p3 = vertices_[(index + 1) % vertices_.size()];

        if (is_reflex(index))
        {
            return false;
        }

        Polygon2D triangle({1000, 1000}, {p1, p2, p3});
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            if ((i != index) && (i != (index + 1) % vertices_.size()) &&
                (i != (index + vertices_.size() - 1) % vertices_.size()))
            {
                if (triangle.contains(vertices_[i]))
                {
                    return false;
                }
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
        Polygon2D triangle({1000, 1000}, 0);
        triangle.add_vertex(vertices_[(index + vertices_.size() - 1) % vertices_.size()]);
        triangle.add_vertex(vertices_[index]);
        triangle.add_vertex(vertices_[(index + 1) % vertices_.size()]);

        vertices_.erase(vertices_.begin() + index);
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

    std::vector<Polygon2D> triangles_;

    Vertices vertices_;
};
