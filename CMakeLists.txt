cmake_minimum_required(VERSION 3.31)
project(KeyLogger)

set(CMAKE_CXX_STANDARD 23)

add_library(keylistener SHARED KeyListener.cpp)
add_library(keydecoder SHARED KeyDecoder.cpp)
add_library(logger SHARED Logger.cpp)

add_executable(keylogger main.cpp)
target_link_libraries(keylogger keylistener keydecoder logger)