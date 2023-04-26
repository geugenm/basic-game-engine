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
            auto iter = vertices_.find(vertex);
            if (iter == vertices_.end() || !iter->second.is_drawn)
            {
                vertices_[vertex] = {vertices_.size(), false};
            }
        }
    }

    void draw_on(Texture &texture, const ColorRGB &color)
    {
        for (auto &[vertex, data] : vertices_)
        {
            if (!data.is_drawn)
            {
                texture.set_pixel(vertex, color);
                data.is_drawn = true;
            }
        }
    }

  private:
    struct VertexData
    {
        size_t index;
        bool is_drawn;
    };

    std::unordered_map<Position2D, VertexData, Position2DHash> vertices_;
};
