#pragma once

#include <memory>
#include <mutex>

namespace Engine
{
class Instance
{
public:
    virtual ~Instance() = default;

    Instance(const Instance&)            = delete;
    Instance& operator=(const Instance&) = delete;

    static Instance& instance()
    {
        static std::once_flag init_flag;
        std::call_once(init_flag, [&]() { instance_.reset(create_instance()); });
        return *instance_;
    }

    virtual void initialize(){};

    virtual void render(){};

    virtual void destroy(){};

protected:
    Instance() = default;

    static Instance* create_instance();

private:
    static std::unique_ptr<Instance> instance_;
};

std::unique_ptr<Instance> Instance::instance_ = nullptr;
} // namespace Engine
