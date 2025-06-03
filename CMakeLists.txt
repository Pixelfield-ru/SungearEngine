set(CMAKE_INSTALL_PREFIX $ENV{SUNGEAR_SOURCES_ROOT}/installed)

include(cmake/set-toolchain.cmake)

if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")
endif()

cmake_minimum_required(VERSION 3.22)
project(SungearEngine)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DSUNGEAR_DEBUG)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MinGW")
    list(APPEND CMAKE_CXX_FLAGS "-static")
endif()

add_definitions(-DSG_BUILD_PRESET="${SG_BUILD_PRESET}")

#set(ZLIB_ROOT ${CMAKE_SOURCE_DIR}/Externals/zlib)
#set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/Externals/zlib)

if(POSIX OR APPLE)
    add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
    add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
elseif(MSVC)
    add_definitions(-DBOOST_STACKTRACE_USE_WINDBG)
endif ()
add_definitions(-DUSE_LIBPNG)
add_definitions(-DHC_NO_COMPRESSION)
add_definitions(-DNOMINMAX)
add_definitions(-DWIN32_LEAN_AND_MEAN)
# BECAUSE I CANT USE antlr4-runtime BECAUSE OF FUCKING MACRO 'ERROR' AND ENUM TYPE IN antlr4 ParseTreeType::ERROR in ParseTreeType.h file
add_definitions(-DNOGDI)
add_definitions(-DBT_ENABLE_PROFILE)
#add_definitions(-DSPDLOG_WCHAR_TO_UTF8_SUPPORT)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

# set(BUILD_SHARED_LIBS OFF)

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
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Od")

            #[[set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /fsanitize=address")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Od /fsanitize=address")]]

            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
        endif ()
    endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        if(MSVC)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
        endif ()
    endif()

    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc")
    endif()

    set(VCPKG_CXX_FLAGS_DEBUG "${VCPKG_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_DEBUG}")
    set(VCPKG_C_FLAGS_DEBUG "${VCPKG_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS_DEBUG}")
    set(VCPKG_CXX_FLAGS "${VCPKG_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
    set(VCPKG_C_FLAGS "${VCPKG_C_FLAGS} ${CMAKE_C_FLAGS}")

    message("cxx compiler flags: ${CMAKE_CXX_FLAGS}, cxx compiler flags debug: ${CMAKE_CXX_FLAGS_DEBUG}, C compiler flags: ${CMAKE_C_FLAGS}, C compiler flags debug: ${CMAKE_C_FLAGS_DEBUG}")

    if(MSVC)
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
        elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
        endif()

        set(MSVC_RUNTIME "static")
    endif()

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(BUILD_SHARED_LIBS OFF)
endif ()

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition -Wno-return-type")

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS ON)
set(SG_BUILD_ENTRY ON)

# ----------------------------------
# including msdf-atlas-gen ========================================

set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_DYNAMIC_RUNTIME ON)

add_subdirectory(Externals/msdf-atlas-gen)

# =================================================================

add_subdirectory(Sources)

if(${SG_BUILD_TESTS})
    add_subdirectory(Tests)
endif()

add_custom_target(auto_install ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Auto-install target triggered"
        COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix ${CMAKE_INSTALL_PREFIX}
        COMMENT "Automatically installing SungearEngine after project build..."
)