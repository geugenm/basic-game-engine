#pragma once

#include <iostream>
#include <memory>
#include <mutex>

#include <sstream>
#include <utility>

namespace sdk
{
class engine_error final : public std::runtime_error
{
public:
    explicit engine_error(const char *message)
        : std::runtime_error(message), function_("unknown")
    {
    }

    explicit engine_error(const char *message, const char *function)
        : std::runtime_error(message), function_(function)
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        std::ostringstream oss;
        oss << function_ << " : " << std::runtime_error::what();
        info_ = oss.str();
        return info_.c_str();
    }

private:
    const char *function_;
    mutable std::string info_;
};

template <typename Derived> class engine
{
public:
    virtual ~engine() = default;

    template <typename... Args> void initialize(Args &&...args)
    {
        if (is_initialized_)
        {
            throw engine_error("Trying to reinitialize the engine.",
                               "initialize");
        }
        static_cast<Derived *>(this)->initialize_impl(
            std::forward<Args>(args)...);

        is_initialized_ = true;
    }

    template <typename... Args> void render(Args &&...args)
    {
        if (!is_initialized_)
        {
            throw engine_error(
                "The engine is not initialized. Use initialize(...) before "
                "calling render(...).",
                "render");
        }
        static_cast<Derived *>(this)->render_impl(std::forward<Args>(args)...);
    }

    template <typename... Args> void destroy(Args &&...args)
    {
        if (!is_initialized_)
        {
            throw engine_error("Trying to destroy already destroyed engine.",
                               "destroy");
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

template <typename Derived> engine<Derived> *create_instance();
} // namespace sdk
