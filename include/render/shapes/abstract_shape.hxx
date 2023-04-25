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



class IndexedShape {
  public:
    IndexedShape() = default;

    void add_2d_shape(const Shape2D& shape) {
        for (const auto& vertex : shape.get_vertices()) {
            auto it = vertices_.find(vertex);
            if (it == vertices_.end()) {
                vertices_[vertex] = vertex_count_;
                vertex_count_++;
            }
        }
    }

    void add_vertex(const Position2D& vertex) {
        auto it = vertices_.find(vertex);
        if (it == vertices_.end()) {
            vertices_[vertex] = vertex_count_;
            vertex_count_++;
        }
    }

  private:
    std::unordered_map<Position2D, size_t, Position2DHash> vertices_;
    size_t vertex_count_ = 0;
};
