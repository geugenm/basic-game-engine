#pragma once

#include "render/colors/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"
#include "render/textures/texture.hxx"

#include <memory>
#include <unordered_map>
#include <vector>

using Vertices = std::vector<Position2D>;

class Shape2D
{
  public:
    Shape2D() = default;

    virtual ~Shape2D() = default;

    virtual void draw_on(Texture &texture, const ColorRGB &color) const = 0;

    [[nodiscard]] virtual Texture get_texture() = 0;

    [[nodiscard]] virtual std::unique_ptr<Shape2D> clone() const = 0;

    [[nodiscard]] virtual std::string string() const = 0;

    [[nodiscard]] virtual Vertices get_vertices() const = 0;
};



struct Position2DHash {
    std::size_t operator()(const Position2D& pos) const {
        std::size_t h1 = std::hash<double>{}(pos.x);
        std::size_t h2 = std::hash<double>{}(pos.y);
        return h1 ^ (h2 << 1);
    }
};



class IndexedShape
{
  public:
    IndexedShape() = default;

    void add_2d_shape(const Shape2D& shape)
    {
        // Get the shape's vertices
        const auto vertices = shape.get_vertices();

        // Loop through the vertices
        for (const auto& vertex : vertices)
        {
            // Check if the vertex has already been drawn
            auto iter = vertices_.find(vertex);
            if (iter == vertices_.end() || !iter->second.is_drawn)
            {
                // Vertex has not been drawn, add it to the index
                vertices_[vertex] = { vertices_.size(), false };
            }
        }
    }

    void draw_on(Texture& texture, const ColorRGB& color)
    {
        // Loop through the vertices
        for (auto& [vertex, data] : vertices_)
        {
            // Check if the vertex has been drawn
            if (!data.is_drawn)
            {
                // Vertex has not been drawn, draw it and mark it as drawn
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

