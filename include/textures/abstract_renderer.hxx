#pragma once

#include "color.hxx"
#include "position.hxx"

#include <memory>
#include <vector>

struct IRenderer {
    virtual void clear(const Color& color)                               = 0;
    virtual void set_pixel(const Position& position, const Color& color) = 0;
    virtual std::vector<Position> pixels_positions(const Position& start,
                                                   const Position& end)  = 0;

    virtual ~IRenderer()                   = default;
    IRenderer()                            = default;
    IRenderer(const IRenderer&)            = delete;
    IRenderer& operator=(const IRenderer&) = delete;
    IRenderer(IRenderer&&)                 = delete;
    IRenderer& operator=(IRenderer&&)      = delete;
};

using render_ptr = std::unique_ptr<IRenderer>;
