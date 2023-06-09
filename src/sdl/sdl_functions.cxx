#include "sdl_functions.hxx"

#include <glad/glad.h>
#include <opengl_functions.hxx>

namespace sdl_subsdk
{
void init_sdl()
{
    constexpr Uint32 flags = SDL_INIT_VIDEO;
    if (SDL_Init(flags) != 0)
    {
        throw std::runtime_error(
            "Failed to init SDL (SDL_INIT_VIDEO flag given)");
    }
}

SDL_Window *get_new_sdl_window(const char *window_title,
                               const int &window_width,
                               const int &window_height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    constexpr Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    SDL_Window *window =
        SDL_CreateWindow(window_title, window_width, window_height, flags);
    if (!window)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to init SDL _window with OpenGL.");
    }

    return window;
}

SDL_GLContext get_new_sdl_gl_context(SDL_Window *window)
{
    if (window == nullptr)
    {
        throw std::invalid_argument(
            "Can't create _context from uninitialized SDL _window.");
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::invalid_argument("Failed to create OpenGL _context.");
    }

    return context;
}

void load_opengl_functions()
{
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to load OpenGL functions with glad: "
                  << glGetError() << std::endl;
        throw std::runtime_error("Can't load opengl functions.");
    }
}
void init_opengl()
{
    load_opengl_functions();

    if (!opengl_subsdk::is_opengl_version_supported())
    {
        throw std::runtime_error("OpenGL version is not supported.");
    }
}

sdk::keyboard sdl_key_to_sdk_key(const int &keycode)
{
    using namespace sdk;

    switch (keycode)
    {
        using enum sdk::keyboard;
        case SDLK_TAB:
            return keyboard_Tab;
        case SDLK_LEFT:
            return keyboard_LeftArrow;
        case SDLK_RIGHT:
            return keyboard_RightArrow;
        case SDLK_UP:
            return keyboard_UpArrow;
        case SDLK_DOWN:
            return keyboard_DownArrow;
        case SDLK_PAGEUP:
            return keyboard_PageUp;
        case SDLK_PAGEDOWN:
            return keyboard_PageDown;
        case SDLK_HOME:
            return keyboard_Home;
        case SDLK_END:
            return keyboard_End;
        case SDLK_INSERT:
            return keyboard_Insert;
        case SDLK_DELETE:
            return keyboard_Delete;
        case SDLK_BACKSPACE:
            return keyboard_Backspace;
        case SDLK_SPACE:
            return keyboard_Space;
        case SDLK_RETURN:
            return keyboard_Enter;
        case SDLK_ESCAPE:
            return keyboard_Escape;
        case SDLK_QUOTE:
            return keyboard_Apostrophe;
        case SDLK_COMMA:
            return keyboard_Comma;
        case SDLK_MINUS:
            return keyboard_Minus;
        case SDLK_PERIOD:
            return keyboard_Period;
        case SDLK_SLASH:
            return keyboard_Slash;
        case SDLK_SEMICOLON:
            return keyboard_Semicolon;
        case SDLK_EQUALS:
            return keyboard_Equal;
        case SDLK_LEFTBRACKET:
            return keyboard_LeftBracket;
        case SDLK_BACKSLASH:
            return keyboard_Backslash;
        case SDLK_RIGHTBRACKET:
            return keyboard_RightBracket;
        case SDLK_BACKQUOTE:
            return keyboard_GraveAccent;
        case SDLK_CAPSLOCK:
            return keyboard_CapsLock;
        case SDLK_SCROLLLOCK:
            return keyboard_ScrollLock;
        case SDLK_NUMLOCKCLEAR:
            return keyboard_NumLock;
        case SDLK_PRINTSCREEN:
            return keyboard_PrintScreen;
        case SDLK_PAUSE:
            return keyboard_Pause;
        case SDLK_KP_0:
            return keyboard_Keypad0;
        case SDLK_KP_1:
            return keyboard_Keypad1;
        case SDLK_KP_2:
            return keyboard_Keypad2;
        case SDLK_KP_3:
            return keyboard_Keypad3;
        case SDLK_KP_4:
            return keyboard_Keypad4;
        case SDLK_KP_5:
            return keyboard_Keypad5;
        case SDLK_KP_6:
            return keyboard_Keypad6;
        case SDLK_KP_7:
            return keyboard_Keypad7;
        case SDLK_KP_8:
            return keyboard_Keypad8;
        case SDLK_KP_9:
            return keyboard_Keypad9;
        case SDLK_KP_PERIOD:
            return keyboard_KeypadDecimal;
        case SDLK_KP_DIVIDE:
            return keyboard_KeypadDivide;
        case SDLK_KP_MULTIPLY:
            return keyboard_KeypadMultiply;
        case SDLK_KP_MINUS:
            return keyboard_KeypadSubtract;
        case SDLK_KP_PLUS:
            return keyboard_KeypadAdd;
        case SDLK_KP_ENTER:
            return keyboard_KeypadEnter;
        case SDLK_KP_EQUALS:
            return keyboard_KeypadEqual;
        case SDLK_LCTRL:
            return keyboard_LeftCtrl;
        case SDLK_LSHIFT:
            return keyboard_LeftShift;
        case SDLK_LALT:
            return keyboard_LeftAlt;
        case SDLK_LGUI:
            return keyboard_LeftSuper;
        case SDLK_RCTRL:
            return keyboard_RightCtrl;
        case SDLK_RSHIFT:
            return keyboard_RightShift;
        case SDLK_RALT:
            return keyboard_RightAlt;
        case SDLK_RGUI:
            return keyboard_RightSuper;
        case SDLK_APPLICATION:
            return keyboard_Menu;
        case SDLK_0:
            return keyboard_0;
        case SDLK_1:
            return keyboard_1;
        case SDLK_2:
            return keyboard_2;
        case SDLK_3:
            return keyboard_3;
        case SDLK_4:
            return keyboard_4;
        case SDLK_5:
            return keyboard_5;
        case SDLK_6:
            return keyboard_6;
        case SDLK_7:
            return keyboard_7;
        case SDLK_8:
            return keyboard_8;
        case SDLK_9:
            return keyboard_9;
        case SDLK_a:
            return keyboard_A;
        case SDLK_b:
            return keyboard_B;
        case SDLK_c:
            return keyboard_C;
        case SDLK_d:
            return keyboard_D;
        case SDLK_e:
            return keyboard_E;
        case SDLK_f:
            return keyboard_F;
        case SDLK_g:
            return keyboard_G;
        case SDLK_h:
            return keyboard_H;
        case SDLK_i:
            return keyboard_I;
        case SDLK_j:
            return keyboard_J;
        case SDLK_k:
            return keyboard_K;
        case SDLK_l:
            return keyboard_L;
        case SDLK_m:
            return keyboard_M;
        case SDLK_n:
            return keyboard_N;
        case SDLK_o:
            return keyboard_O;
        case SDLK_p:
            return keyboard_P;
        case SDLK_q:
            return keyboard_Q;
        case SDLK_r:
            return keyboard_R;
        case SDLK_s:
            return keyboard_S;
        case SDLK_t:
            return keyboard_T;
        case SDLK_u:
            return keyboard_U;
        case SDLK_v:
            return keyboard_V;
        case SDLK_w:
            return keyboard_W;
        case SDLK_x:
            return keyboard_X;
        case SDLK_y:
            return keyboard_Y;
        case SDLK_z:
            return keyboard_Z;
        case SDLK_F1:
            return keyboard_F1;
        case SDLK_F2:
            return keyboard_F2;
        case SDLK_F3:
            return keyboard_F3;
        case SDLK_F4:
            return keyboard_F4;
        case SDLK_F5:
            return keyboard_F5;
        case SDLK_F6:
            return keyboard_F6;
        case SDLK_F7:
            return keyboard_F7;
        case SDLK_F8:
            return keyboard_F8;
        case SDLK_F9:
            return keyboard_F9;
        case SDLK_F10:
            return keyboard_F10;
        case SDLK_F11:
            return keyboard_F11;
        case SDLK_F12:
            return keyboard_F12;
        default:
            return keyboard_None;
    }
}
} // namespace sdl_subsdk