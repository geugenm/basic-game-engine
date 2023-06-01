#pragma once

#include <iostream>
#include <memory>
#include <mutex>

#include <sstream>
#include <utility>
#include <vector>

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

class component
{
public:
    explicit component(const char *name) : name_(name) {}

    virtual ~component() = default;

    virtual void initialize()
    {
        if (is_initialized_)
        {
            throw engine_error("Trying to reinitialize the engine.",
                               "initialize");
        }

        is_initialized_ = true;
    }

    virtual void render()
    {
        if (!is_initialized_)
        {
            throw engine_error(
                "The engine is not initialized. Use initialize(...) before "
                "calling render(...).",
                "render");
        }
    }
    virtual void destroy()
    {
        if (!is_initialized_)
        {
            throw engine_error("Trying to destroy already destroyed engine.",
                               "destroy");
        }

        is_initialized_ = false;
    }

    [[nodiscard]] const char *get_name() const
    {
        return name_;
    }

private:
    const char *name_;

    bool is_initialized_ = false;
};

using component_ptr = std::unique_ptr<component>;

class engine
{
public:
    virtual ~engine() = default;

    virtual void initialize()
    {
        if (is_initialized_)
        {
            throw engine_error("Trying to reinitialize the engine.",
                               "initialize");
        }

        is_initialized_ = true;
    }

    virtual void render()
    {
        if (!is_initialized_)
        {
            throw engine_error(
                "The engine is not initialized. Use initialize(...) before "
                "calling render(...).",
                "render");
        }

        for (auto const &component : components_)
        {
            component->render();
        }
    }

    virtual void destroy()
    {
        if (!is_initialized_)
        {
            throw engine_error("Trying to destroy already destroyed engine.",
                               "destroy");
        }

        for (auto const &component : components_)
        {
            component->destroy();
        }

        is_initialized_ = false;
    }

    void add_component(component_ptr component)
    {
        if (component == nullptr)
        {
            throw engine_error("Trying to add null component", "add_component");
        }
        component->initialize();
        components_.push_back(std::move(component));
    }

    [[nodiscard]] bool is_initialized() const
    {
        return is_initialized_;
    }

private:
    bool is_initialized_ = false;

    std::vector<component_ptr> components_;
};
} // namespace sdk
