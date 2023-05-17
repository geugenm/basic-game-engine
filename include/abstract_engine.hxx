#pragma once

#include <memory>
#include <mutex>
#include <iostream>
#include <utility>

namespace Engine
{
template <typename Derived> class Instance
{
public:
    virtual ~Instance() = default;

    Instance(const Instance&)            = delete;
    Instance& operator=(const Instance&) = delete;

    static Instance& instance()
    {
        static std::once_flag init_flag;
        std::call_once(init_flag,
                       [&]() { instance_.reset(static_cast<Derived*>(create_instance())); });
        return *instance_;
    }

    template <typename... Args> void initialize(Args&&... args)
    {
        if (is_initialized_) {
            std::cerr << "WARNING: The engine is already initialized";
            return ;
        }
        static_cast<Derived*>(this)->initialize_impl(std::forward<Args>(args)...);
        is_initialized_ = true;
    }

    template <typename... Args> void render(Args&&... args)
    {
        if (!is_initialized_) {
            std::cerr << "WARNING: The engine is not initialized. Use initialize(...) before calling render(...)";
            return ;
        }
        static_cast<Derived*>(this)->render_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void destroy(Args&&... args)
    {
        if (!is_initialized_) {
            std::cerr << "WARNING: The engine is already destroyed";
            return;
        }

        static_cast<Derived*>(this)->destroy_impl(std::forward<Args>(args)...);
        is_initialized_ = false;
    }

    [[nodiscard]] bool is_initialized() const {
        return is_initialized_;
    }

protected:
    Instance() = default;

    static Instance* create_instance();

private:
    static std::unique_ptr<Instance> instance_;

    bool is_initialized_ = false;
};

template <typename Derived>
std::unique_ptr<Instance<Derived>> Instance<Derived>::instance_ = nullptr;
} // namespace Engine
