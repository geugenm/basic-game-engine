#pragma once

#include "render/shaders/uniform.hxx"

namespace GFX
{

struct GFXProgram
{
    virtual ~GFXProgram() = default;

    virtual void set_uniform(const Uniform &uniform) = 0;
    virtual Position2D vertex_shader(const Position2D &vertex) = 0;
    virtual ColorRGB fragment_shader(const Position2D &vertex) = 0;
};
} // namespace GFX
