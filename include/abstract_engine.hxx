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
    using runtime_error::runtime_error;

    explicit engine_error(const char *message, const char *function_name)
        : std::runtime_error(message), function_name_(function_name)
    {
    }

    explicit engine_error(const char *message, const char *function_name,
                          const char *object_name)
        : std::runtime_error(message), function_name_(function_name),
          object_name_(object_name)
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        std::ostringstream oss;

        if (object_name_ != nullptr)
        {
            oss << object_name_ << ".";
        }

        if (function_name_ != nullptr)
        {
            oss << function_name_ << " : ";
        }

        oss << std::runtime_error::what();

        info_ = oss.str();
        return info_.c_str();
    }

private:
    const char *function_name_ = nullptr;
    const char *object_name_   = nullptr;

    mutable std::string info_;
};

class object
{
public:
    explicit object(const char *name = "object") : name_(name) {}

    virtual ~object() = default;

    void initialize()
    {
        if (is_initialized_)
        {
            throw engine_error("Trying to reinitialize the component.",
                               "initialize");
        }

        initialize_impl();

        is_initialized_ = true;
    }

    void render()
    {
        if (!is_initialized_)
        {
            throw engine_error(
                "The object is not initialized. Use initialize(...) before "
                "calling render(...).",
                "render", get_name());
        }

        render_impl();
    }

    void destroy()
    {
        if (!is_initialized_)
        {
            throw engine_error("Trying to destroy already destroyed object.",
                               "destroy");
        }

        destroy_impl();

        is_initialized_ = false;
    }

    [[nodiscard]] const char *get_name() const
    {
        return name_;
    }

    [[nodiscard]] bool is_initialized() const
    {
        return is_initialized_;
    }

protected:
    virtual void initialize_impl() = 0;
    virtual void render_impl()     = 0;
    virtual void destroy_impl()    = 0;

private:
    const char *name_;

    bool is_initialized_ = false;
};

class component : public object
{
public:
    explicit component(const char *component_name = "component") : object(component_name) {}
};

using component_ptr = std::unique_ptr<component>;

class engine : public object
{
public:
    explicit engine(const char *engine_name = "engine") : object(engine_name) {}

    ~engine() override = default;

    void init() {
        object::initialize();
    }

    void render()
    {
        object::render();

        for (auto const &component : components_)
        {
            component->render();
        }
    }

    void destroy()
    {
        object::destroy();

        for (auto const &component : components_)
        {
            component->destroy();
        }
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

    [[nodiscard]] component * get_component(const size_t & index) {
        if (index >= components_.size()) {
            throw engine_error("index is larger than components amount");
        }

        return components_[index].get();
    }

private:
    std::vector<component_ptr> components_;
};
} // namespace sdk
