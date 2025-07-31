//
// Created by ofir on 7/24/25.
//

#ifndef KEYLISTENER_HPP
#define KEYLISTENER_HPP

#include <poll.h>
#include <string>
#include <vector>

namespace keylogger
{

struct KeyEvent
{
    int code;
    int value;
};

class KeyListener
{
public:
    explicit KeyListener();
    ~KeyListener();
    KeyEvent Listen();
private:
    void FindInputDevices();
    static bool IsInputDevice(const std::string& device_path);
    std::vector<pollfd> m_fds;
}; // class KeyListener
} // namespace keylogger

#endif //KEYLISTENER_HPP
