#pragma once

#include "ivertex.hxx"
#include <vector>
#include <array>

namespace AbstractEngine
{
template <typename T> using IVertices = std::vector<IVertex<T>>;
template <typename T, std::size_t N> using IVerticesFixed = std::array<IVertex<T>, N>;
}
