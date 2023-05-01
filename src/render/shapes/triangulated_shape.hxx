#pragma once

#include "polygon_2d.hxx"

class TriangulatedShape2D : Shape2D
{
  public:
    explicit TriangulatedShape2D(const Shape2D &shape)
    {
        set_vertices(shape.get_vertices());
        set_bounding_box(shape.get_bounding_box());
    }

    ~TriangulatedShape2D() override = default;

    [[nodiscard]] std::string string() const override
    {
        return Shape2D::string();
    }

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

    /// TODO: make sure that everything drawn up
    void triangulate(Texture &texture, const ColorRGB &color)
    {
        if (access_vertices().size() < 3)
        {
            return;
        }

        std::vector<size_t> indices(access_vertices().size());
        for (size_t i = 0; i < access_vertices().size(); ++i)
        {
            indices[i] = i;
        }

        std::vector<bool> ear(access_vertices().size());
        for (size_t i = 0; i < access_vertices().size(); ++i)
        {
            ear[i] = is_ear(i);
        }

        texture.set_shape(get_bounding_box());

        size_t remaining_vertices = access_vertices().size();
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
            size_t previous_index = (ear_index == 0) ? (access_vertices().size() - 1) : (ear_index - 1);
            size_t next_index = ear_index % (access_vertices().size() - 1);
            ear[previous_index] = is_ear(previous_index);
            ear[next_index] = is_ear(next_index);

            remaining_vertices--;
        }

        draw_on(texture, {255, 255, 0});
    }

    [[nodiscard]] bool is_ear(const size_t &index)
    {
        if (is_reflex(index))
        {
            return false;
        }

        const auto &prev_vertex = access_vertices()[(index + access_vertices().size() - 1) % access_vertices().size()];
        const auto &curr_vertex = access_vertices()[index];
        const auto &next_vertex = access_vertices()[(index + 1) % access_vertices().size()];

        Polygon2D triangle(get_bounding_box(), {prev_vertex, curr_vertex, next_vertex});

        for (const auto &vertex : access_vertices())
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

    [[nodiscard]] bool is_reflex(const size_t &index)
    {
        const Position2D &p1 = access_vertices()[(index + access_vertices().size() - 1) % access_vertices().size()];
        const Position2D &p2 = access_vertices()[index];
        const Position2D &p3 = access_vertices()[(index + 1) % access_vertices().size()];

        double cross_product = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);

        return (cross_product < 0);
    }

    void remove_ear(const size_t &index)
    {
        access_vertices().erase(access_vertices().begin() + static_cast<long>(index));
    }
};
