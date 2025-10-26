include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/utils.cmake)
include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/platform.cmake)

get_filename_component(SG_BINARY_DIR_NAME ${CMAKE_BINARY_DIR} NAME)
message(STATUS "Binary Directory Name: ${SG_BINARY_DIR_NAME}")

if(NOT DEFINED SG_BUILD_PRESET)
    message(FATAL_ERROR "Your preset does not have 'SG_BUILD_PRESET' in 'cacheVariables' section.")
endif()

# =====================================================================================================
# setup cxx

if(SG_TARGET_OS_ANDROID)
    set(ANDROID_STL "c++_static")
    add_definitions(-DANDROID_STL="c++_static")
endif()

set(CMAKE_SHARED_LIBRARY_PREFIX "")

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
endif()

set(VCPKG_CXX_FLAGS_DEBUG "${VCPKG_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_DEBUG}")
set(VCPKG_CXX_FLAGS_RELEASE "${VCPKG_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_RELEASE}")
set(VCPKG_C_FLAGS_DEBUG "${VCPKG_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS_DEBUG}")
set(VCPKG_C_FLAGS_RELEASE "${VCPKG_C_FLAGS_RELEASE} ${CMAKE_C_FLAGS_RELEASE}")
set(VCPKG_CXX_FLAGS "${VCPKG_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(VCPKG_C_FLAGS "${VCPKG_C_FLAGS} ${CMAKE_C_FLAGS}")

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition -Wno-return-type")

if(POSIX OR APPLE)
    add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
    add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
elseif(MSVC)
    add_definitions(-DBOOST_STACKTRACE_USE_WINDBG)
endif ()
add_definitions(-DNOMINMAX)
add_definitions(-DANTLR4CPP_STATIC)
add_definitions(-DSG_BUILD_PRESET="${SG_BUILD_PRESET}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DSUNGEAR_DEBUG)
endif()

# =====================================================================================================

set(SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Sources")
set(SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/${SG_BINARY_DIR_NAME}/Sources/SGCore/SGCore.${SG_DL_EXT}")

list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Externals")

#[[set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_DYNAMIC_RUNTIME ON)]]
add_definitions(-DMSDFGEN_PUBLIC=)

list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Externals/msdf-atlas-gen")
list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Externals/msdf-atlas-gen/msdfgen")

list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Externals/rectpack2D/src")

find_package(Bullet REQUIRED)
list(APPEND SungearEngine_INCLUDE_DIRS "${BULLET_INCLUDE_DIR}")
list(APPEND SungearEngine_LIBS "${SG_BULLET_LIBS}")

find_package(pugixml CONFIG REQUIRED)
find_path(PUGIXML_INCLUDE_DIR pugixml.hpp)
list(APPEND SungearEngine_INCLUDE_DIRS "${PUGIXML_INCLUDE_DIR}")
list(APPEND SungearEngine_LIBS "pugixml")


find_package(Freetype REQUIRED)
list(APPEND SungearEngine_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}")
list(APPEND SungearEngine_LIBS "${FREETYPE_LIBRARIES}")

find_package(glm REQUIRED)
list(APPEND SungearEngine_LIBS "glm::glm")

find_package(assimp CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "assimp::assimp")

find_package(EnTT CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "EnTT::EnTT")

if(SG_TARGET_PLATFORM_PC)
    find_package(glfw3 CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "glfw")
endif()

find_package(imgui CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "imgui::imgui")

find_package(spdlog CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "spdlog::spdlog")

find_package(Stb REQUIRED)
list(APPEND SungearEngine_INCLUDE_DIRS "${Stb_INCLUDE_DIR}")

find_package(glad CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "glad::glad")

find_package(OpenAL CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "OpenAL::OpenAL")

include(FetchContent)
FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)
list(APPEND SungearEngine_LIBS "glaze::glaze")

find_package(lunasvg CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "lunasvg::lunasvg")

find_package(RapidJSON CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "rapidjson")

find_package(antlr4-runtime CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "antlr4_static")
list(APPEND SungearEngine_INCLUDE_DIRS "${ANTLR4_INCLUDE_DIR}")

find_package(gli CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "gli")

find_package(meshoptimizer CONFIG REQUIRED)
list(APPEND SungearEngine_LIBS "meshoptimizer::meshoptimizer")

find_package(JNI REQUIRED)
list(APPEND SungearEngine_INCLUDE_DIRS "${JNI_INCLUDE_DIRS}")
list(APPEND SungearEngine_LIBS "${JNI_LIBRARIES}")

if(SG_TARGET_OS_ANDROID)
    find_library(SG_ANDROID_LIB android)
    find_library(SG_EGL_LIB EGL)
    list(APPEND SungearEngine_LIBS "GLESv3 ${SG_ANDROID_LIB} ${SG_EGL_LIB}")

    message(STATUS "Android Lib: ${SG_ANDROID_LIB}")
    message(STATUS "EGL Lib: ${SG_EGL_LIB}")
endif()

if(SG_TARGET_OS_LINUX OR SG_TARGET_OS_DARWIN)
    find_library(Backtrace_LIBRARY backtrace)
    list(APPEND SungearEngine_LIBS "${CMAKE_DL_LIBS} ${Backtrace_LIBRARY} tbb")
endif()

list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}")