#pragma once

#include <utility>

namespace AbstractEngine {
template <typename Derived> class IShader
{
public:
    virtual ~IShader() = default;

    template <typename... Args> void initialize(Args&&... args)
    {
        static_cast<Derived*>(this)->initialize_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void compile(Args&&... args)
    {
        static_cast<Derived*>(this)->render_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void destroy(Args&&... args)
    {
        static_cast<Derived*>(this)->destroy_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void reload(Args&&... args)
    {
        auto* derived = static_cast<Derived*>(this);
        derived->destroy_impl(std::forward<Args>(args)...);
        derived->initialize_impl(std::forward<Args>(args)...);
    }
};
}
