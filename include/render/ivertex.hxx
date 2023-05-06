#pragma once

#include <type_traits>
#include <vector>

namespace AbstractEngine
{
template <typename T>
    requires std::is_arithmetic_v<T>
class IVertex
{
public:
    IVertex() : x_(static_cast<T>(0)), y_(static_cast<T>(0)) {}

    IVertex(const T& x, const T& y) : x_(x), y_(y) {}

    virtual ~IVertex() = default;

    [[nodiscard]] T get_x() const
    {
        return x_;
    }
    [[nodiscard]] T get_y() const
    {
        return y_;
    }

protected:
    virtual void set_x(const T& x)
    {
        x_ = x;
    }

    virtual void set_y(const T& y)
    {
        y_ = y;
    }

    [[nodiscard]] virtual T& access_x()
    {
        return x_;
    }

    [[nodiscard]] virtual T& access_y()
    {
        return y_;
    }

private:
    T x_;
    T y_;
};

} // namespace AbstractEngine

