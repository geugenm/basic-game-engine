#pragma once

#include <memory>
#include <mutex>

#include "render/abstract_declarations.hxx"
#include "render/abstract_triangle.hxx"

namespace AbstractEngine
{

class EngineInstance
{
public:
    virtual ~EngineInstance() = default;

    EngineInstance(const EngineInstance&)            = delete;
    EngineInstance& operator=(const EngineInstance&) = delete;

    static EngineInstance& instance()
    {
        static std::once_flag init_flag;
        std::call_once(init_flag, [&]() { instance_.reset(create_instance()); });
        return *instance_;
    }

protected:
    EngineInstance() = default;

    static EngineInstance* create_instance();

private:
    static std::unique_ptr<EngineInstance> instance_;
};

std::unique_ptr<EngineInstance> EngineInstance::instance_ = nullptr;
}
