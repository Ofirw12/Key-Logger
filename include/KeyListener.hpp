
#ifndef KEYLISTENER_HPP
#define KEYLISTENER_HPP

#include <poll.h>
#include <string>
#include <vector>

#include "Structs.hpp"

namespace keylogger
{
class KeyListener
{
public:
    explicit KeyListener();
    ~KeyListener();
    KeyEvent Listen();

    KeyListener(const KeyListener& listener) = delete;
    KeyListener& operator=(const KeyListener& listener) = delete;
    KeyListener(KeyListener&& listener) = delete;
    KeyListener& operator=(KeyListener&& listener) = delete;
private:
    void FindInputDevices();
    static bool IsInputDevice(const std::string& device_path);
    std::vector<pollfd> m_fds;
}; // class KeyListener
} // namespace keylogger

#endif //KEYLISTENER_HPP
