if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")
endif()

cmake_minimum_required(VERSION 3.25)
project(${pluginName}$)

set(CMAKE_CXX_STANDARD ${cxxStandard}$)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-implicit-fallthrough -Wno-ignored-qualifiers -g -rdynamic -fno-pie -no-pie -Wno-error=deprecated-copy")

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_definitions(-DSTBI_NO_SIMD)
        add_definitions(-DSTBIR_NO_SIMD)

        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if(MSVC)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("/bigobj")
    endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        if(MSVC)
            set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /Od")
            set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} /Od")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
            set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
        endif ()
    endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        if(MSVC)
            set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /O2")
            set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} /O2")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -O2")
            set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} -O2")
        endif ()
    endif()

    message("cxx compiler flags: ${CXX_COMPILER_FLAGS}, C compiler flags: ${C_COMPILER_FLAGS}")

    set(CMAKE_CXX_FLAGS ${CXX_COMPILER_FLAGS})
    set(CMAKE_C_FLAGS ${C_COMPILER_FLAGS})

    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
        set(MSVC_RUNTIME "static")
    endif()

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(BUILD_SHARED_LIBS OFF)
endif ()

if(POSIX OR APPLE)
    add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
    add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
elseif(MSVC)
    add_definitions(-DBOOST_STACKTRACE_USE_WINDBG)
endif ()
add_definitions(-DNOMINMAX)

set(SungearEngineEditor_INCLUDE_EXTERNALS ON)

file(GLOB_RECURSE SG_CURRENT_PLUGIN_GENERATED_FILES .SG_GENERATED/*.h .SG_GENERATED/*.cpp)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)
include($ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor/cmake/include_as_plugin.cmake)

add_library(${PROJECT_NAME} SHARED Sources/PluginMain.h Sources/PluginMain.cpp Sources/${pluginName}$.h Sources/${pluginName}$.cpp ${SG_CURRENT_PLUGIN_GENERATED_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE Sources ${SungearEngine_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS})