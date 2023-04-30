#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"
#include "render/textures/texture.hxx"

#include <memory>
#include <vector>

using Vertices = std::vector<Position2D>;

class Shape2D
{
  public:
    Shape2D() = default;

    virtual ~Shape2D() = default;

    virtual void draw_on(Texture &texture, const ColorRGB &color) = 0;

    [[nodiscard]] virtual std::string string() const
    {
        std::string result = "Shape2D: (" + std::to_string(vertices_.size()) + ") vertices\n";
        for (std::size_t i = 0; i < vertices_.size(); ++i)
        {
            const auto &vertex = vertices_[i];
            result += "Vertex " + std::to_string(i) + ": " + vertex.string() + "\n";
        }
        return result;
    }

    [[nodiscard]] const Vertices &get_vertices() const
    {
        return vertices_;
    }

    [[nodiscard]] const BoundingBox &get_bounding_box() const
    {
        return bounding_box_;
    }

    bool has_vertex_on(const Position2D &position)
    {
        if (std::ranges::any_of(vertices_, [&](const auto &vertex) { return vertex == position; }))
        {
            return true;
        }
        return false;
    }

  protected:
    void set_bounding_box(const BoundingBox &boundingBox)
    {
        bounding_box_ = boundingBox;
    }

    void set_vertices(const Vertices &vertices)
    {
        access_vertices() = vertices;
    }

    void shrink_bounding_box() {
        int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
        for (const auto &vertex : access_vertices())
        {
            min_x = std::min(min_x, vertex.x);
            min_y = std::min(min_y, vertex.y);
            max_x = std::max(max_x, vertex.x);
            max_y = std::max(max_y, vertex.y);
        }

        const Position2D bottom_left = {min_x, min_y};
        const Position2D top_right = {max_x, max_y};
        set_bounding_box(BoundingBox(bottom_left, top_right));
    }

    [[nodiscard]] Vertices &access_vertices()
    {
        return vertices_;
    }

    [[nodiscard]] BoundingBox &access_bounding_box()
    {
        return bounding_box_;
    }

  private:
    BoundingBox bounding_box_;

    Vertices vertices_;
};
