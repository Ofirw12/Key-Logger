cmake_minimum_required(VERSION 3.31)
project(KeyLogger)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_subdirectory(src)
add_subdirectory(include)

add_executable(keylogger main.cpp)
target_link_libraries(keylogger keylistener keydecoder logger)