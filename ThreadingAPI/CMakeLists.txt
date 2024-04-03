cmake_minimum_required(VERSION 3.27)
project(ThreadingAPI)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES Sources/*.cpp Sources/*.h)

add_executable(ThreadingAPI ${SOURCES})
