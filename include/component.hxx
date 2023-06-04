#pragma once

#include "render_object.hxx"

#include <memory>

namespace sdk
{
class component : public object
{
public:
    explicit component(const char *component_name = "component")
        : object(component_name)
    {
    }
};
} // namespace sdk