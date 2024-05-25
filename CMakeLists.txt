include(cmake/set-toolchain.cmake)

cmake_minimum_required(VERSION 3.22)
project(SungearEngine)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DSUNGEAR_DEBUG)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MinGW")
    list(APPEND CMAKE_CXX_FLAGS "-static")
endif()

#set(ZLIB_ROOT ${CMAKE_SOURCE_DIR}/Externals/zlib)
#set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/Externals/zlib)

add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DUSE_LIBPNG)
add_definitions(-DHC_NO_COMPRESSION)
add_definitions(-DNOMINMAX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE ON)

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-implicit-fallthrough -Wno-ignored-qualifiers -g -rdynamic -fno-pie -no-pie -Wno-error=deprecated-copy")
    set(CMAKE_CXX_FLAGS_DEBUG "-O1")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
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

    message("cxx compiler flags: ${CXX_COMPILER_FLAGS}, C compiler flags: ${C_COMPILER_FLAGS}")

    set(CMAKE_CXX_FLAGS ${CXX_COMPILER_FLAGS})
    set(CMAKE_C_FLAGS ${C_COMPILER_FLAGS})

    set(MSVC_RUNTIME "dynamic")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(BUILD_SHARED_LIBS TRUE)
endif ()

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition -Wno-return-type")

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS OFF)

set(SG_RESOURCES_COPY_MODELS ON)
set(SG_RESOURCES_COPY_SHADERS ON)
set(SG_RESOURCES_COPY_TEXTURES ON)
set(SG_RESOURCES_COPY_UI ON)
set(SG_RESOURCES_COPY_FONTS ON)

# ----------------------------------
add_subdirectory(Sources)

if(${SG_BUILD_TESTS})
    add_subdirectory(Tests)
endif()
