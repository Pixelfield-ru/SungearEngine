cmake_minimum_required(VERSION 3.25)
project(${pluginName}$)

set(CMAKE_CXX_STANDARD ${cxxStandard}$)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
if(${CMAKE_COMPILER_IS_GNUCXX})
    list(APPEND CMAKE_CXX_FLAGS "-g -rdynamic -fno-pie -no-pie -fno-gnu-unique -fexceptions -fnon-call-exceptions")
    message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
elseif(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /Od")

        set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} /Od")
    endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} /O2")

        set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_RELEASE} /O2")
    endif()

    foreach(CompilerFlag ${CXX_COMPILER_FLAGS})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    foreach(CompilerFlag ${C_COMPILER_FLAGS})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    message("CXX compiler flags: ${CXX_COMPILER_FLAGS}, C compiler flags: ${C_COMPILER_FLAGS}")

    set(CMAKE_CXX_FLAGS ${CXX_COMPILER_FLAGS})
    set(CMAKE_C_FLAGS ${C_COMPILER_FLAGS})

    set(MSVC_RUNTIME "dynamic")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
add_definitions(-DNOMINMAX)

file(GLOB_RECURSE SG_CURRENT_PLUGIN_GENERATED_FILES .SG_GENERATED/*.h .SG_GENERATED/.cpp)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

add_library(${PROJECT_NAME} SHARED Sources/PluginMain.h Sources/PluginMain.cpp Sources/${pluginName}$.h Sources/${pluginName}$.cpp ${SG_CURRENT_PLUGIN_GENERATED_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE Sources ${SungearEngine_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS})