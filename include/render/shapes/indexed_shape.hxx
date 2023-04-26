#pragma once

#include "render/shapes/line_2d.hxx"

#include <unordered_map>

struct Position2DHash
{
    std::size_t operator()(const Position2D &pos) const
    {
        std::size_t h1 = std::hash<double>{}(pos.x);
        std::size_t h2 = std::hash<double>{}(pos.y);
        return h1 ^ (h2 << 1);
    }
};

class IndexedShape
{
  public:
    IndexedShape() = default;

    void add_2d_shape(const Shape2D &shape)
    {
        const auto vertices = shape.get_vertices();

        for (const auto &vertex : vertices)
        {
            if (std::find(vertices_.begin(), vertices_.end(), vertex) == vertices_.end())
            {
                vertices_.reserve(vertices_.size() + 1);
                vertices_.push_back(vertex);
                indexes_.push_back(indexes_.size());
            }
        }
    }

    void draw_on(Texture &texture, const ColorRGB &color)
    {
        for (size_t i = 0; i < indexes_.size(); i += 3)
        {
            const auto &v0 = vertices_[indexes_[i]];
            const auto &v1 = vertices_[indexes_[i + 1]];
            const auto &v2 = vertices_[indexes_[i + 2]];

            Line2D l1(v0, v1);
            l1.draw_on(texture, color);

            Line2D l2(v1, v2);
            l2.draw_on(texture, color);

            Line2D l3(v2, v0);
            l3.draw_on(texture, color);
        }
    }

  private:
    Vertices vertices_;
    std::vector<size_t> indexes_;
};
