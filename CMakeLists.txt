cmake_minimum_required(VERSION 3.25)
project(SungearEngine)

add_definitions(-DSUNGEAR_DEBUG)

set(CMAKE_CXX_STANDARD 23)

set(BUILD_SHARED_LIBS ON)

set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_BUILD_TYPE "Debug")

include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})

add_subdirectory(Externals)
add_subdirectory(Sources)
add_subdirectory(Resources)

message(CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR})