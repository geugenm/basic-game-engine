#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <utility>

namespace Engine
{
template <typename Derived> class Instance
{
public:
    virtual ~Instance() = default;

    template <typename... Args> void initialize(Args &&...args)
    {
        if (is_initialized_)
        {
            throw std::runtime_error("Trying to reinitialize the engine.");
        }
        static_cast<Derived *>(this)->initialize_impl(
            std::forward<Args>(args)...);
        is_initialized_ = true;
    }

    template <typename... Args> void render(Args &&...args)
    {
        if (!is_initialized_)
        {
            throw std::runtime_error(
                "The engine is not initialized. Use initialize(...) before "
                "calling render(...).");
        }
        static_cast<Derived *>(this)->render_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void destroy(Args &&...args)
    {
        if (!is_initialized_)
        {
            throw std::runtime_error(
                "Trying to destroy already destroyed engine.");
        }

        static_cast<Derived *>(this)->destroy_impl(std::forward<Args>(args)...);
        is_initialized_ = false;
    }

    [[nodiscard]] bool is_initialized() const
    {
        return is_initialized_;
    }

private:
    bool is_initialized_ = false;
};

template <typename Derived> Instance<Derived> *create_instance();
} // namespace Engine
