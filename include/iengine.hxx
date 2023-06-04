#pragma once

#include "render_object.hxx"
#include "component.hxx"

#include <easylogging++.h>

#include <iostream>
#include <memory>
#include <mutex>

#include <sstream>
#include <utility>
#include <vector>

INITIALIZE_EASYLOGGINGPP

namespace sdk
{
class iengine : public object
{
public:
    explicit iengine(const char *engine_name = "engine") : object(engine_name)
    {
    }

    ~iengine() override = default;

    void init()
    {
        object::initialize();

        for (auto const &component : components_)
        {
            component->initialize();
        }
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

    void add_component(std::unique_ptr<component> component)
    {
        if (component == nullptr)
        {
            throw engine_error("Trying to add null component", "add_component");
        }
        component->initialize();
        components_.push_back(std::move(component));
    }

    void add_component(component *component)
    {
        if (component == nullptr)
        {
            throw engine_error("Trying to add null component", "add_component");
        }
        component->initialize();
        components_.push_back(std::unique_ptr<sdk::component>(component));
    }

private:
    std::vector<std::unique_ptr<component>> components_;
};
} // namespace sdk
