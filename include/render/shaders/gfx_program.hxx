#pragma once

namespace GFX
{
struct Uniform
{
    double f0 = 0;
    double f1 = 0;
    double f2 = 0;
    double f3 = 0;
    double f4 = 0;
    double f5 = 0;
    double f6 = 0;
    double f7 = 0;
    Texture *texture = nullptr;
};

struct GFXProgram
{
    virtual ~GFXProgram() = default;

    virtual void set_uniform(const Uniform &) = 0;
    virtual Position2D vertex_shader(const Position2D &vertex) = 0;
    virtual ColorRGB fragment_shader(const Position2D &vertex) = 0;
};
} // namespace GFX
