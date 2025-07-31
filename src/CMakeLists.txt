cmake_minimum_required(VERSION 3.31)

add_library(keylistener SHARED KeyListener.cpp)
add_library(keydecoder SHARED KeyDecoder.cpp)
add_library(logger SHARED Logger.cpp)