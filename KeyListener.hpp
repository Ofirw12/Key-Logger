//
// Created by ofir on 7/24/25.
//

#ifndef KEYLISTENER_HPP
#define KEYLISTENER_HPP

#include <poll.h>
#include <vector>
#include <string>

namespace keylogger
{
class KeyListener
{
public:
    explicit KeyListener();
    void Run();
    bool IsInputDevice(const std::string& device_path);
private:
    std::vector<pollfd> m_fds;

}; // class KeyListener
} // namespace keylogger

#endif //KEYLISTENER_HPP
