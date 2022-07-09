cmake_minimum_required(VERSION 3.22)
project(macroMagic)

set(CMAKE_C_STANDARD 11)

add_executable(macroMagic main.c pp.h stack.h serialize.h)
add_executable(macroMagicCpp main.cpp pp.h stack.h serialize.h)
add_executable(tp tp.cpp)
