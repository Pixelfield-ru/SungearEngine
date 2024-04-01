cmake_minimum_required(VERSION 3.25)
project(SungearEngineEditor)

get_filename_component(VCPKG_CMAKE_ROOT ${CMAKE_TOOLCHAIN_FILE} DIRECTORY)
get_filename_component(VCPKG_CMAKE_ROOT ${VCPKG_CMAKE_ROOT} DIRECTORY)
get_filename_component(VCPKG_CMAKE_ROOT ${VCPKG_CMAKE_ROOT} DIRECTORY)

list(APPEND CMAKE_PREFIX_PATH $ENV{VCPKG_ROOT}/installed/x64-linux/share/sungearengine)
list(APPEND CMAKE_MODULE_PATH $ENV{VCPKG_ROOT}/installed/x64-linux/share)
list(APPEND CMAKE_PREFIX_PATH $ENV{VCPKG_ROOT}/installed/x64-linux/share)
list(APPEND CMAKE_PREFIX_PATH $ENV{VCPKG_ROOT}/installed/x64-linux/include)
list(APPEND CMAKE_PREFIX_PATH $ENV{VCPKG_ROOT}/installed/x64-linux/lib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
if(${CMAKE_COMPILER_IS_GNUCXX})
	set(CMAKE_CXX_FLAGS "-g -rdynamic -fno-pie -no-pie")
endif()

add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)

set(SG_INCLUDE_BULLET ON)
set(SG_INCLUDE_PUGIXML ON)
set(SG_INCLUDE_FREETYPE ON)
set(SG_INCLUDE_GLM ON)
set(SG_INCLUDE_ASSIMP ON)
set(SG_INCLUDE_ENTT ON)
set(SG_INCLUDE_GLFW ON)
set(SG_INCLUDE_IMGUI ON)
set(SG_INCLUDE_SPDLOG ON)
set(SG_INCLUDE_STB ON)
set(SG_INCLUDE_GLAD ON)

#set(SUNGEAR_ENGINE_DIR "/home/ilya/pixelfield/SungearEngine")
set(SUNGEAR_ENGINE_DIR "F:/Pixelfield/SungearEngine")
list(APPEND CMAKE_MODULE_PATH ${SUNGEAR_ENGINE_DIR})

find_package(glfw3 CONFIG REQUIRED)
find_package(SungearEngine CONFIG REQUIRED)

file(GLOB_RECURSE SGE_SOURCES Sources/*.cpp Sources/*.h)

#add_library(${PROJECT_NAME} SHARED ${SGE_SOURCES})
add_executable(${PROJECT_NAME} ${SGE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SungearEngine_INCLUDE_DIR} Sources)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw SungearEngine ${SungearEngine_LIBS})
