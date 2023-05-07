#pragma once

#include <type_traits>

namespace AbstractEngine
{
template <typename T>
    requires std::is_arithmetic_v<T>
struct Vertex2D
{
    T _x;
    T _y;
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct Vertex3D
{
    T _x;
    T _y;
    T _z;
};

} // namespace Engine
