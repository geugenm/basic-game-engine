#pragma once

#include <iostream>
#include <tuple>

template <typename... Ts>
struct Uniform {
    std::tuple<Ts...> values;

    Uniform(Ts... args) : values(std::make_tuple(args...)) {}
};
