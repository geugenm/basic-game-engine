#pragma once

#include "render/color/color_rgb.hxx"
#include "render/shapes/position_2d.hxx"

#include <memory>
#include <vector>

struct IRender {
    virtual void clear(const ColorRGB& color)                               = 0;
    virtual void set_pixel(const Position2D& position, const ColorRGB& color) = 0;
    virtual std::vector<Position2D> pixels_positions(const Position2D& start,
                                                   const Position2D& end)     = 0;

    virtual ~IRender()                 = default;
    IRender()                          = default;
    IRender(const IRender&)            = delete;
    IRender& operator=(const IRender&) = delete;
    IRender(IRender&&)                 = delete;
    IRender& operator=(IRender&&)      = delete;
};

using render_ptr = std::unique_ptr<IRender>;
