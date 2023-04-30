#pragma once

#include "polygon_2d.hxx"

class TriangulatedShape2D {
  public:
    TriangulatedShape2D(const Shape2D & shape) {
        vertices_ = shape.get_vertices();
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
    Vertices vertices_;
};
