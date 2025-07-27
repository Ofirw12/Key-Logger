cmake_minimum_required(VERSION 3.31)
project(KeyLogger)

set(CMAKE_CXX_STANDARD 23)

add_library(keylistener KeyListener.cpp)
add_library(keydecoder KeyDecoder.cpp)
add_library(logger Logger.cpp)

add_executable(keylogger main.cpp)
