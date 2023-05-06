#pragma once

#include "abstract_declarations.hxx"

#include <stdexcept>

namespace AbstractEngine
{
template <typename T>
    requires std::is_arithmetic_v<T>
struct Triangle {
    constexpr static std::size_t _k_sides_amount = 3;

    IVerticesFixed<T, _k_sides_amount> _vertices = {T(), T(), T()};
};

} // namespace AbstractEngine
