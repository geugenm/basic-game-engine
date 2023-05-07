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
        static_cast<Derived*>(this)->initialize_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void render(Args&&... args)
    {
        static_cast<Derived*>(this)->render_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void destroy(Args&&... args)
    {
        static_cast<Derived*>(this)->destroy_impl(std::forward<Args>(args)...);
    }

protected:
    Instance() = default;

    static Instance* create_instance();

private:
    static std::unique_ptr<Instance> instance_;
};

template <typename Derived>
std::unique_ptr<Instance<Derived>> Instance<Derived>::instance_ = nullptr;
} // namespace Engine
