#pragma once

#include "component.hxx"
#include "logger.hxx"
#include "render_object.hxx"

#include <iostream>
#include <memory>
#include <mutex>

#include <sstream>
#include <utility>
#include <vector>

namespace sdk
{
class iengine : public object
{
public:
    explicit iengine(const char *engine_name = "engine") : object(engine_name)
    {
    }

    ~iengine() override = default;

    void initialize()
    {
        if (is_initialized())
        {
            LOG(WARNING) << "Reinitializing engine and all its components";
        }

        object::initialize();

        for (auto const &component : components_)
        {
            component->initialize();
            LOG(INFO) << "Component '" << component->get_name()
                      << "' initialized";
        }

        LOG(INFO) << "Engine initialized";
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
            LOG(INFO) << "Component '" << component->get_name()
                      << "' destroyed";
        }

        LOG(INFO) << "Engine destroyed";
    }

    void handle_event(const event &event)
    {
        object::handle_event(event);

        for (auto const &component : components_)
        {
            component->handle_event(event);
            LOG(INFO) << "Component '" << component->get_name()
                      << "' handled event";
        }
    }

    void add_component(component *component)
    {
        if (component == nullptr)
        {
            throw engine_error("Trying to add null component", "add_component");
        }
        component->initialize();

        components_.push_back(std::unique_ptr<sdk::component>(component));

        LOG(INFO) << "Component '" << component->get_name() << "' added";
    }

private:
    std::vector<std::unique_ptr<component>> components_;
};
} // namespace sdk
