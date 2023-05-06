#pragma once

#include "render/abstract_vertex.hxx"

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
