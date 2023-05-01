#pragma once

#include "gfx_program.hxx"

namespace GFX
{
class TestShader: public GFXProgram
{
  public:
    ~TestShader() override = default;

    void set_uniform(const Uniform & uniform) override
    {

    }

    Position2D vertex_shader(const Position2D &vertex) override
    {
            return vertex;
    }

    ColorRGB fragment_shader(const Position2D &vertex) override
    {
        if (vertex.x < 800) {
            return {0, 255, 200};
        }
        return {100, 0, 0};
    }

  private:

};
} // namespace GFX
