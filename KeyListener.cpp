//
// Created by ofir on 7/24/25.
//

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#include "KeyListener.hpp"


keylogger::KeyListener::KeyListener()
{
}

void keylogger::KeyListener::Run()
{
}

bool keylogger::KeyListener::IsInputDevice(const std::string& device_path)
{
    // Check if it's an event device
    if (device_path.find("event") == std::string::npos)
    {
        return false;
    }

    // Try to open the device to check if it's accessible
    const int fd = open(device_path.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        return false;
    }

    // Get device capabilities to determine if it's a keyboard or mouse
    unsigned long evbit = 0;
    ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
    close(fd);

    // Check if device has key or button events
    return (evbit & (1 << EV_KEY)) != 0;
}