#pragma once

#include "vertex.hxx"
#include <array>
#include <vector>

namespace AbstractEngine
{
template <typename T> using Vertices2D                     = std::vector<Vertex2D<T>>;
template <typename T, std::size_t N> using Vertices2DFixed = std::array<Vertex2D<T>, N>;

template <typename T> using Vertices3D                     = std::vector<Vertex3D<T>>;
template <typename T, std::size_t N> using Vertices3DFixed = std::array<Vertex3D<T>, N>;
} // namespace Engine
