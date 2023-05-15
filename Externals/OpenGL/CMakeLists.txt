cmake_minimum_required(VERSION 3.25)
project(gl46)

add_library(gl46 STATIC include/glad/glad.h src/glad.c)

target_include_directories(gl46 PUBLIC include)