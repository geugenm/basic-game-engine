#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <opengl_functions.hxx>

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <ostream>
#include <vector>

#include <SDL3/SDL.h>

#include "sprite.hxx"

namespace sdk
{
struct sdl_render_context final
{
    SDL_Window *_window    = nullptr;
    SDL_GLContext _context = nullptr;

    [[nodiscard]] bool is_initialized() const
    {
        return _window != nullptr && _context != nullptr;
    }

    [[nodiscard]] int get_width() const
    {
        int width;
        if (SDL_GetWindowSize(_window, &width, nullptr) != 0)
        {
            throw std::invalid_argument(SDL_GetError());
        }
        return width;
    }

    [[nodiscard]] int get_height() const
    {
        int height;
        if (SDL_GetWindowSize(_window, nullptr, &height) != 0)
        {
            throw std::invalid_argument(SDL_GetError());
        }
        return height;
    }
};

struct sprite_animation final
{
    std::uint64_t _current_frame{};
    std::uint64_t _rows{};
    std::uint64_t _columns{};

    [[nodiscard]] static sprite_animation create_new_animation(std::size_t rows,
                                                               std::size_t cols)
    {
        if (rows == 0 || cols == 0)
        {
            throw std::invalid_argument(
                "Can't create sprite animation: `rows={}` or "
                "`cols={}` is 0");
        }

        sprite_animation new_animation{
            ._current_frame = 0,
            ._rows          = rows,
            ._columns       = cols,
        };

        return new_animation;
    }

    [[nodiscard]] bool is_properly_initialized() const
    {
        if (_rows == 0 || _columns == 0)
        {

            std::cout << "WARNING: sprite animation is not properly "
                         "initialized: `rows={}` or "
                         "`cols={}` is 0"
                      << std::endl;
            return false;
        }

        return true;
    }
};

enum class game_states
{
    paused,
    played,
    in_menu,
    exited,
};
} // namespace sdk