cmake_minimum_required(VERSION 3.25)
project(${pluginName}$)
set(CMAKE_CXX_STANDARD ${cxxStandard}$)

set(SungearEngineEditor_INCLUDE_EXTERNALS ON)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)
include($ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor/cmake/include_as_plugin.cmake)

file(GLOB_RECURSE SG_CURRENT_PLUGIN_GENERATED_FILES .SG_GENERATED/*.h .SG_GENERATED/*.cpp)

add_library(${PROJECT_NAME} SHARED Sources/PluginMain.h Sources/PluginMain.cpp Sources/${pluginName}$.h Sources/${pluginName}$.cpp ${SG_CURRENT_PLUGIN_GENERATED_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE Sources ${SungearEngine_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS})