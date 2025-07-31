
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

    Logger(const Logger& logger) = delete;
    Logger& operator=(const Logger& logger) = delete;
    Logger(Logger&& logger) = delete;
    Logger& operator=(Logger&& logger) = delete;

private:
    std::ofstream m_file;
}; // class Logger
} // namespace keylogger

#endif //LOGGER_HPP
