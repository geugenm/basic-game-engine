#pragma once

#include "ivertex.hxx"

namespace AbstractEngine
{
template <typename T> using IVertices = std::vector<IVertex<T>>;
}
