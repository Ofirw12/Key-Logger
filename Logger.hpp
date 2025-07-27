

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>

namespace keylogger
{
class Logger
{
public:
    explicit Logger(const std::string& filename = "keystrokes.log");
    ~Logger();
    void Log(const std::string& message);
private:
    std::ofstream m_file;
}; // class Logger
} // namespace keylogger

#endif //LOGGER_HPP
