#pragma once

#include "engine_error.hxx"

namespace sdk
{
class object
{
public:
    explicit object(const char *name = "object") : name_(name) {}

    virtual ~object() = default;

    void initialize()
    {
        if (is_initialized_)
        {
            return;
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
                "sdk::object::render", get_name());
        }

        render_impl();
    }

    void destroy()
    {
        if (!is_initialized_)
        {
            throw engine_error("Trying to destroy already destroyed object.",
                               "sdk::object::destroy");
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
} // namespace sdk