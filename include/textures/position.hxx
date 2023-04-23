#pragma once

#include <cmath>
#include <random>

struct position {
    int32_t x = 0;
    int32_t y = 0;

    constexpr position() = default;

    constexpr position(int32_t x_, int32_t y_)
        : x(x_)
        , y(y_) { }

    constexpr double length() const { return std::sqrt(x * x + y * y); }

    friend constexpr position operator-(const position& left,
                                        const position& right) {
        return position(left.x - right.x, left.y - right.y);
    }

    friend constexpr bool operator==(const position& left,
                                     const position& right) {
        return left.x == right.x && left.y == right.y;
    }

    static position generate_random(const int & width, const int & height) {
        static std::random_device              rd;
        static std::mt19937                    gen(rd());
        std::uniform_int_distribution<int32_t> dist_x(0, width - 1);
        std::uniform_int_distribution<int32_t> dist_y(0, height - 1);
        return position(dist_x(gen), dist_y(gen));
    }
};
