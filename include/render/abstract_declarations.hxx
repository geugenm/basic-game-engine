#pragma once

#include "abstract_vertex.hxx"
#include <array>
#include <vector>

namespace AbstractEngine
{
template <typename T> using IVertices = std::vector<IVertex<T>>;
template <typename T, std::size_t N> using IVerticesFixed = std::array<IVertex<T>, N>;
}
