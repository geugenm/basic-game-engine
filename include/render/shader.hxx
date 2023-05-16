#pragma once

#include <utility>

namespace AbstractEngine {
template <typename Derived> class IShader
{
public:
    virtual ~IShader() = default;

    template <typename... Args> void initialize(Args&&... args)
    {
        if (!is_initialized_) {
            static_cast<Derived*>(this)->initialize_impl(std::forward<Args>(args)...);
            is_initialized_ = true;
        }
        else {
            std::cerr << "The shader is already initialized";
        }
    }

    template <typename... Args> void destroy(Args&&... args)
    {
        if (is_initialized_) {
            static_cast<Derived*>(this)->destroy_impl(std::forward<Args>(args)...);
            is_initialized_ = false;
        }
        else {
            std::cerr << "Can't destroy uninitialized shader";
        }

    }

    template <typename... Args> void reload(Args&&... args)
    {
        if (is_initialized_) {
            static_cast<Derived*>(this)->reload_impl(std::forward<Args>(args)...);
        }
        else {
            std::cerr << "Can't reload uninitialized shader";
        }
    }

    template <typename... Args> void set_uniform(Args&&... args)
    {
        if (is_initialized_) {
            static_cast<Derived*>(this)->set_uniform_impl(std::forward<Args>(args)...);
        }
        else {
            std::cerr << "Can't set uniforms of uninitialized shader";
        }
    }

    [[nodiscard]] bool is_initialized() const {
        return is_initialized_;
    }

protected:
    template <typename... Args> void compile(Args&&... args)
    {
        static_cast<Derived*>(this)->render_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void link(Args&&... args)
    {
        static_cast<Derived*>(this)->link_impl(std::forward<Args>(args)...);
    }

private:
    bool is_initialized_ = false;
};
}
