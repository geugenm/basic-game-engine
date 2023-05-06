#pragma once

#include "abstract_engine.hxx"

namespace Engine
{

class ShaderVertex final : public AbstractEngine::IVertex<float>
{
public:
    ShaderVertex() : AbstractEngine::IVertex<float>() {}

    ShaderVertex(const float& x, const float& y) : AbstractEngine::IVertex<float>(x, y) {}

    ~ShaderVertex() override = default;
};

} // namespace Engine
