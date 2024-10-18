include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/utils.cmake)

add_definitions(-DSG_BUILD_PRESET="${SG_BUILD_PRESET}")

if(NOT DEFINED SG_BUILD_PRESET)
    message(FATAL_ERROR "Your preset does not have 'SG_BUILD_PRESET' in 'cacheVariables' section.")
endif()

if(${SG_BUILD_TYPE} STREQUAL "")
    set(SG_BUILD_TYPE "Release")
endif()

if(${SG_BUILD_TYPE} STREQUAL "Release")
    set(SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Sources")
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/cmake-build-release/Sources/SGCore")
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Externals")

    set(SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/cmake-build-release/Sources/SGCore/SGCore.${SG_DL_EXT}")
elseif(${SG_BUILD_TYPE} STREQUAL "Debug")
    set(SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Sources")
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/cmake-build-debug/Sources/SGCore")
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Externals")

    set(SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/cmake-build-debug/Sources/SGCore/SGCore.${SG_DL_EXT}")
endif()

#if(${SG_INCLUDE_BULLET})
    find_package(Bullet REQUIRED)

    list(APPEND SungearEngine_INCLUDE_DIRS "${BULLET_INCLUDE_DIR}")
    list(APPEND SungearEngine_LIBS "${SG_BULLET_LIBS}")
#endif()

#if(${SG_INCLUDE_PUGIXML})
    find_package(pugixml CONFIG REQUIRED)
    find_path(PUGIXML_INCLUDE_DIR pugixml.hpp)

    list(APPEND SungearEngine_INCLUDE_DIRS "${PUGIXML_INCLUDE_DIR}")
    list(APPEND SungearEngine_LIBS "pugixml")
#endif()

#if(${SG_INCLUDE_FREETYPE})
    find_package(Freetype REQUIRED)

    list(APPEND SungearEngine_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}")
    list(APPEND SungearEngine_LIBS "${FREETYPE_LIBRARIES}")
#endif()

#if(${SG_INCLUDE_GLM})
    find_package(glm REQUIRED)
    list(APPEND SungearEngine_LIBS "glm::glm")
#endif()

#if(${SG_INCLUDE_ASSIMP})
    find_package(assimp CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "assimp::assimp")
#endif()

#if(${SG_INCLUDE_ENTT})
    find_package(EnTT CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "EnTT::EnTT")
#endif()

#if(${SG_INCLUDE_GLFW})
    find_package(glfw3 CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "glfw")
#endif()

#if(${SG_INCLUDE_IMGUI})
    find_package(imgui CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "imgui::imgui")
#endif()

#if(${SG_INCLUDE_SPDLOG})
    find_package(spdlog CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "spdlog::spdlog")
#endif()

#if(${SG_INCLUDE_STB})
    find_package(Stb REQUIRED)
    list(APPEND SungearEngine_INCLUDE_DIRS "${Stb_INCLUDE_DIR}")
#endif()

#if(${SG_INCLUDE_GLAD})
    find_package(glad CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "glad::glad")
#endif()

#if(${SG_INCLUDE_OPENAL})
    find_package(OpenAL CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "OpenAL::OpenAL")
#endif()

#if(${SG_INCLUDE_GLAZE})
    include(FetchContent)

    FetchContent_Declare(
            glaze
            GIT_REPOSITORY https://github.com/stephenberry/glaze.git
            GIT_TAG main
            GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(glaze)

    list(APPEND SungearEngine_LIBS "glaze::glaze")
#endif()

#if(${SG_INCLUDE_LUNASVG})
    find_package(unofficial-lunasvg CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "unofficial::lunasvg::lunasvg")
#endif()

#if(${SG_INCLUDE_RAPIDJSON})
    find_package(RapidJSON CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "rapidjson")
#endif()

if(${CMAKE_COMPILER_IS_GNUCXX})
    find_library(Backtrace_LIBRARY backtrace)

    list(APPEND SungearEngine_LIBS "${Backtrace_LIBRARY}")
endif()

list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}")
