
#ifndef KEYDECODER_HPP
#define KEYDECODER_HPP

#include <string>
#include <unordered_map>

#include "Structs.hpp"

namespace keylogger
{
class KeyDecoder
{
public:
    explicit KeyDecoder() = default;
    [[nodiscard]] std::string Decode(KeyEvent ev);

    KeyDecoder(const KeyDecoder& decoder) = delete;
    KeyDecoder(KeyDecoder&& decoder) = delete;
    KeyDecoder& operator=(const KeyDecoder& decoder) = delete;
    KeyDecoder& operator=(KeyDecoder&& decoder) = delete;
private:
    static std::string GetMouseButtonName(int code);
    [[nodiscard]] std::string GetKeyName(int code, bool shifted) const;
    std::string ProcessKeyEvent(KeyEvent ev);
    [[nodiscard]] std::string ProcessMouseEvent(KeyEvent ev);
    static const std::unordered_map<int, std::string> m_key_mappings;
    static const std::unordered_map<int, std::string> m_mouse_button_mappings;
    static const std::unordered_map<int, std::string> m_shifted_mappings;
    bool m_ctrl_pressed{};
    bool m_alt_pressed{};
    bool m_shift_pressed{};
    bool m_meta_pressed{};
    bool m_caps_lock{};
}; // class KeyDecoder
} // namespace keylogger

#endif //KEYDECODER_HPP
