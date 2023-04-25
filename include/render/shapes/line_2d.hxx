#pragma once

#include "render/abstract_renderer.hxx"
#include "render/textures/texture.hxx"

class LineRender2D : public IRender {
public:
    LineRender2D(const Texture& texture) {

    }

    void clear(const ColorRGB & color) override {

    }

private:
    std::unique_ptr<Texture> texture_;
};
