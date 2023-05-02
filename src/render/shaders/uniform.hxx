#pragma once

#include "render/textures/texture.hxx"

namespace GFX
{
struct Uniform
{
    double f0        = 0;
    double f1        = 0;
    double f2        = 0;
    double f3        = 0;
    double f4        = 0;
    double f5        = 0;
    double f6        = 0;
    double f7        = 0;
    Texture* texture = nullptr;
};
} // namespace GFX
