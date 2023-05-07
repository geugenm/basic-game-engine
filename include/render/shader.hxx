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
        static_cast<Derived*>(this)->reload_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void set_uniform(Args&&... args)
    {
        static_cast<Derived*>(this)->set_uniform_impl(std::forward<Args>(args)...);
    }
};
}
