//
// Created by ofir on 7/24/25.
//

#include "../include/Logger.hpp"

#include <ctime>

keylogger::Logger::Logger(const std::string& filename) : m_file(filename, std::ios::app)
{
    if (!m_file)
    {
        throw std::runtime_error("Failed to open log file");
    }
    Log("=== Input logging session started ===");
}

keylogger::Logger::~Logger()
{
    Log("=== Input logging session ended ===");
}

void keylogger::Logger::Log(const std::string& message)
{
    // Get current time
    const std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // Write to log file
    m_file << "[" << timestamp << "] " << message << std::endl;
    m_file.flush();
}