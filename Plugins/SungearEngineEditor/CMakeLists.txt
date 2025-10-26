cmake_minimum_required(VERSION 3.25)
project(SungearEngineEditor)

set(CMAKE_CXX_STANDARD 23)

add_definitions(-DUSE_IMGUI_API)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

message(STATUS "CMAKE_MODULE_PATH=${CMAKE_PREFIX_PATH}")
find_package(nfd CONFIG REQUIRED)

file(GLOB_RECURSE SGE_SOURCES Sources/*.cpp Sources/*.h)
list(FILTER SGE_SOURCES EXCLUDE REGEX ".templates")

message("Bullet libs: ${SG_BULLET_LIBS}")

add_library(${PROJECT_NAME} SHARED ${SGE_SOURCES} Externals/ImGuizmo/ImGuizmo.h Externals/ImGuizmo/ImGuizmo.cpp)
# add_executable(${PROJECT_NAME} ${SGE_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC Sources ${SungearEngine_INCLUDE_DIRS} Externals)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS} nfd::nfd)
