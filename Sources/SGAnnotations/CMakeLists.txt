cmake_minimum_required(VERSION 3.22)
project(SGAnnotationsUtil)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} Annotations.h AnnotationsProcessor.h Entry.cpp Utils.h)