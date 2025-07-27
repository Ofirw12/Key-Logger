//
// Created by ofir on 7/24/25.
//

#ifndef KEYDECODER_HPP
#define KEYDECODER_HPP
#include <string>
#include <unordered_map>

#include "Logger.hpp"

namespace keylogger
{
class KeyDecoder
{
public:
    explicit KeyDecoder(Logger& logger);
    [[nodiscard]] std::string GetKeyName(int code, bool shifted) const;
    static std::string GetMouseButtonName(int code);
    void ProcessMouseEvent(int code, int value) const;
    void ProcessKeyEvent(int code, int value);
private:
    static const std::unordered_map<int, std::string> m_key_mappings;
    static const std::unordered_map<int, std::string> m_mouse_button_mappings;
    static const std::unordered_map<int, std::string> m_shifted_mappings;
    bool m_ctrl_pressed{};
    bool m_alt_pressed{};
    bool m_shift_pressed{};
    bool m_meta_pressed{};
    bool m_caps_lock{};
    Logger& m_logger;
}; // class KeyDecoder
} // namespace keylogger

#endif //KEYDECODER_HPP
