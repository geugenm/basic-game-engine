#pragma once

#include "render/textures/texture.hxx"

#include <concepts>

namespace GFX
{
template <Numeric T>
struct Uniform {
    T f0;
    T f1;
    T f2;
    T f3;
    T f4;
    T f5;
    T f6;
    T f7;
    Texture* texture0;

    constexpr Uniform(T f0, T f1, T f2, T f3, T f4, T f5, T f6, T f7, Texture* texture0)
        : f0(f0), f1(f1), f2(f2), f3(f3), f4(f4), f5(f5), f6(f6), f7(f7), texture0(texture0)
    {}

    constexpr bool operator==(const Uniform& other) const {
        return f0 == other.f0 && f1 == other.f1 && f2 == other.f2 && f3 == other.f3 &&
               f4 == other.f4 && f5 == other.f5 && f6 == other.f6 && f7 == other.f7 &&
               texture0 == other.texture0;
    }

    constexpr bool operator!=(const Uniform& other) const {
        return *this != other;
    }
};

}
