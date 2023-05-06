#include "abstract_engine.hxx"


namespace Engine
{
    class EngineInstance : public AbstractEngine::EngineInstance {

    };
} // namespace Engine

AbstractEngine::EngineInstance* AbstractEngine::EngineInstance::create_instance()
{
    return new Engine::EngineInstance();
}
