#pragma once

namespace sdk
{
enum class keyboard : int
{
    keyboard_None = 0,
    keyboard_Tab  = 512,
    keyboard_LeftArrow,
    keyboard_RightArrow,
    keyboard_UpArrow,
    keyboard_DownArrow,
    keyboard_PageUp,
    keyboard_PageDown,
    keyboard_Home,
    keyboard_End,
    keyboard_Insert,
    keyboard_Delete,
    keyboard_Backspace,
    keyboard_Space,
    keyboard_Enter,
    keyboard_Escape,
    keyboard_LeftCtrl,
    keyboard_LeftShift,
    keyboard_LeftAlt,
    keyboard_LeftSuper,
    keyboard_RightCtrl,
    keyboard_RightShift,
    keyboard_RightAlt,
    keyboard_RightSuper,
    keyboard_Menu,
    keyboard_0,
    keyboard_1,
    keyboard_2,
    keyboard_3,
    keyboard_4,
    keyboard_5,
    keyboard_6,
    keyboard_7,
    keyboard_8,
    keyboard_9,
    keyboard_A,
    keyboard_B,
    keyboard_C,
    keyboard_D,
    keyboard_E,
    keyboard_F,
    keyboard_G,
    keyboard_H,
    keyboard_I,
    keyboard_J,
    keyboard_K,
    keyboard_L,
    keyboard_M,
    keyboard_N,
    keyboard_O,
    keyboard_P,
    keyboard_Q,
    keyboard_R,
    keyboard_S,
    keyboard_T,
    keyboard_U,
    keyboard_V,
    keyboard_W,
    keyboard_X,
    keyboard_Y,
    keyboard_Z,
    keyboard_F1,
    keyboard_F2,
    keyboard_F3,
    keyboard_F4,
    keyboard_F5,
    keyboard_F6,
    keyboard_F7,
    keyboard_F8,
    keyboard_F9,
    keyboard_F10,
    keyboard_F11,
    keyboard_F12,
    keyboard_Apostrophe,   // '
    keyboard_Comma,        // ,
    keyboard_Minus,        // -
    keyboard_Period,       // .
    keyboard_Slash,        // /
    keyboard_Semicolon,    // ;
    keyboard_Equal,        // =
    keyboard_LeftBracket,  // [
    keyboard_Backslash,    // \ (this text inhibits multiline comment caused by
                           // backslash)
    keyboard_RightBracket, // ]
    keyboard_GraveAccent,  // `
    keyboard_CapsLock,
    keyboard_ScrollLock,
    keyboard_NumLock,
    keyboard_PrintScreen,
    keyboard_Pause,
    keyboard_Keypad0,
    keyboard_Keypad1,
    keyboard_Keypad2,
    keyboard_Keypad3,
    keyboard_Keypad4,
    keyboard_Keypad5,
    keyboard_Keypad6,
    keyboard_Keypad7,
    keyboard_Keypad8,
    keyboard_Keypad9,
    keyboard_KeypadDecimal,
    keyboard_KeypadDivide,
    keyboard_KeypadMultiply,
    keyboard_KeypadSubtract,
    keyboard_KeypadAdd,
    keyboard_KeypadEnter,
    keyboard_KeypadEqual
};

struct event
{
    keyboard _keyboard;
};
} // namespace sdk