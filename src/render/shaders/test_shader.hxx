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
        return vertex;
    }

    ColorRGB fragment_shader(const Position2D &vertex) override
    {
        auto distortion = static_cast<float>(0.05f * std::sin(5.0f * static_cast<float>(vertex.x)));
        Position2D distortedVertex = vertex + Position2D(static_cast<int32_t>(distortion), 0.0f);

        ColorRGB texColor = distortedVertex.color;

        return texColor;
    }
};
} // namespace GFX
