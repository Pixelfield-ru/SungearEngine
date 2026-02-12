include(cmake/set-toolchain.cmake)
include(cmake/platform.cmake)

cmake_minimum_required(VERSION 3.22)
project(SungearEngine)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DSUNGEAR_DEBUG)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "MinGW")
    list(APPEND CMAKE_CXX_FLAGS "-static")
endif()

add_definitions(-DSG_BUILD_PRESET="${SG_BUILD_PRESET}")

if(SG_TARGET_OS_LINUX OR SG_TARGET_OS_DARWIN)
    add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
    add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
elseif(SG_TARGET_OS_WINDOWS)
    add_definitions(-DBOOST_STACKTRACE_USE_WINDBG)
endif()
add_definitions(-DUSE_LIBPNG)
add_definitions(-DHC_NO_COMPRESSION)
add_definitions(-DNOMINMAX)
add_definitions(-DWIN32_LEAN_AND_MEAN)
# BECAUSE I CANT USE antlr4-runtime BECAUSE OF FUCKING MACRO 'ERROR' AND ENUM TYPE IN antlr4 ParseTreeType::ERROR in ParseTreeType.h file
add_definitions(-DNOGDI)

add_definitions(-DANTLR4CPP_STATIC)

if(SG_TARGET_OS_ANDROID)
    set(MSDFGEN_USE_SKIA OFF)
    set(MSDF_ATLAS_USE_SKIA OFF)
endif()
set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_DYNAMIC_RUNTIME ON)
#add_definitions(-DSPDLOG_WCHAR_TO_UTF8_SUPPORT)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("/bigobj")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()

    set(MSVC_RUNTIME "static")

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    # set(BUILD_SHARED_LIBS OFF)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-implicit-fallthrough -Wno-ignored-qualifiers -Wno-error=deprecated-copy")
endif()

if(SG_TARGET_OS_LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -g -fno-pie -no-pie")

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_definitions(-DSTBI_NO_SIMD)
        add_definitions(-DSTBIR_NO_SIMD)
    endif()
elseif(SG_TARGET_OS_WINDOWS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
elseif(SG_TARGET_OS_ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -g -fno-pie -fvisibility=default")
endif()

set(VCPKG_CXX_FLAGS_DEBUG "${VCPKG_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_DEBUG}")
set(VCPKG_CXX_FLAGS_RELEASE "${VCPKG_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_RELEASE}")
set(VCPKG_C_FLAGS_DEBUG "${VCPKG_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS_DEBUG}")
set(VCPKG_C_FLAGS_RELEASE "${VCPKG_C_FLAGS_RELEASE} ${CMAKE_C_FLAGS_RELEASE}")
set(VCPKG_CXX_FLAGS "${VCPKG_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(VCPKG_C_FLAGS "${VCPKG_C_FLAGS} ${CMAKE_C_FLAGS}")

message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-export-dynamic -Wl,-Bsymbolic")

message("CMAKE_SIZEOF_VOID_P: ${CMAKE_SIZEOF_VOID_P}")

message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "ANDROID_ABI: ${ANDROID_ABI}")
message(STATUS "ANDROID_PLATFORM: ${ANDROID_PLATFORM}")
message(STATUS "ANDROID_STL: ${ANDROID_STL}")

# packages --------------------------------------------------

find_package(Bullet REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(EnTT CONFIG REQUIRED)
if(SG_TARGET_PLATFORM_PC)
    find_package(glfw3 CONFIG REQUIRED)
endif()
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
# find_package(Stb REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(unofficial-brotli CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(lunasvg CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(antlr4-runtime CONFIG REQUIRED)
find_package(gli CONFIG REQUIRED)
find_package(meshoptimizer CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system asio)
find_package(JNI REQUIRED)
find_path(SOL2_INCLUDE_DIRS "sol/abort.hpp")
find_package(Lua REQUIRED)

message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")
message(STATUS "JNI_LIBRARIES: ${JNI_LIBRARIES}")

if(SG_TARGET_OS_ANDROID)
    find_library(SG_ANDROID_LIB android)
    find_library(SG_EGL_LIB EGL)
endif()

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS ON)
set(SG_BUILD_ENTRY ON)

# ----------------------------------
# including msdf-atlas-gen ========================================

add_subdirectory(Externals/msdf-atlas-gen)
add_subdirectory(Externals/rectpack2D)

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
