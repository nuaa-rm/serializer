cmake_minimum_required(VERSION 3.22)
project(serializer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD  11)

add_executable(serializer main.c pp.h stack.h serialize.h)
add_executable(serializerCpp main.cpp pp.h stack.h serialize.h)
add_executable(ringBuf exampleRingBuf.c ringBuffer.h)
add_executable(ringBufCpp exampleRingBuf.cpp ringBuffer.h)