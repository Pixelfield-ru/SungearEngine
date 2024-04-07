cmake_minimum_required(VERSION 3.25)
project(ThreadingAPI)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_DEBUG "-O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES Sources/*.cpp Sources/*.h)

add_executable(ThreadingAPI ${SOURCES})
