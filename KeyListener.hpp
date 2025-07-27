//
// Created by ofir on 7/24/25.
//

#ifndef KEYLISTENER_HPP
#define KEYLISTENER_HPP

#include <poll.h>
#include <string>
#include <vector>

#include "KeyDecoder.hpp"

namespace keylogger
{
class KeyListener
{
public:
    explicit KeyListener(KeyDecoder& decoder);
    void Run();
    bool IsInputDevice(const std::string& device_path);
private:
    std::vector<pollfd> m_fds;
    KeyDecoder& m_keyDecoder;

}; // class KeyListener
} // namespace keylogger

#endif //KEYLISTENER_HPP
