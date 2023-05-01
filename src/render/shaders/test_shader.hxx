#pragma once

#include <cmath>

#include "gfx_program.hxx"

namespace GFX
{
class TestShader : public GFXProgram
{
  public:
    ~TestShader() override = default;

    void set_uniform(const Uniform &uniform) override
    {
    }

    Position2D vertex_shader(const Position2D &vertex) override
    {
        const float strength = 170000.2f;  // adjust this to control the strength of the distortion
        const Position2D center(500, 500); // set the center point of the distortion

        // calculate the distance from the center point
        const double distance = center.distance_to(vertex);

        // calculate the amount to shift the vertex based on its distance from the center
        const double shift = strength / (distance * distance);

        // apply the shift to the vertex position
        const Position2D warped_vertex = vertex + (vertex - center) * shift;

        return warped_vertex;
    }

    ColorRGB fragment_shader(const Position2D &vertex) override
    {
        return {0, 255, 0};
    }
};
} // namespace GFX
