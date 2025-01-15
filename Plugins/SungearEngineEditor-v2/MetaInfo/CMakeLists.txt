cmake_minimum_required(VERSION 3.25)
project(MetaInfo)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} SHARED Meta.h Meta.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{SUNGEAR_SOURCES_ROOT}/Sources")