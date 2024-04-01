set(SG_DL_EXT ???)

if(MSVC)
    set(SG_DL_EXT lib)
elseif(${CMAKE_COMPILER_IS_GNUCXX})
    set(SG_DL_EXT so)
endif ()

set(SungearEngine_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include")
#list(APPEND SungearEngine_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/cmake-build-release/Sources/SGCore")
#list(APPEND SungearEngine_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/cmake-build-release/Sources/SGUtils")
#list(APPEND SungearEngine_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/Externals")

set(SungearEngine_LIBS "${PACKAGE_PREFIX_DIR}/libs/SGCore.${SG_DL_EXT}")

if(${SG_INCLUDE_BULLET})
    find_package(Bullet REQUIRED)

    list(APPEND SungearEngine_INCLUDE_DIRS "${BULLET_INCLUDE_DIR}")
    list(APPEND SungearEngine_LIBS "${BULLET_COLLISION_LIBRARY}")
    list(APPEND SungearEngine_LIBS "${BULLET_DYNAMICS_LIBRARY}")
    list(APPEND SungearEngine_LIBS "${BULLET_MATH_LIBRARY}")
endif()

if(${SG_INCLUDE_PUGIXML})
    find_package(pugixml CONFIG REQUIRED)
    find_path(PUGIXML_INCLUDE_DIR pugixml.hpp)

    list(APPEND SungearEngine_INCLUDE_DIRS "${PUGIXML_INCLUDE_DIR}")
    list(APPEND SungearEngine_LIBS "pugixml")
endif()

if(${SG_INCLUDE_FREETYPE})
    find_package(Freetype REQUIRED)

    list(APPEND SungearEngine_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}")
    list(APPEND SungearEngine_LIBS "${FREETYPE_LIBRARIES}")
endif()

if(${SG_INCLUDE_GLM})
    find_package(glm REQUIRED)
    list(APPEND SungearEngine_LIBS "glm::glm")
endif()

if(${SG_INCLUDE_ASSIMP})
    find_package(assimp CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "assimp::assimp")
endif()

if(${SG_INCLUDE_ENTT})
    find_package(EnTT CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "EnTT::EnTT")
endif()

if(${SG_INCLUDE_GLFW})
    find_package(glfw3 CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "glfw")
endif()

if(${SG_INCLUDE_IMGUI})
    find_package(imgui CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "imgui::imgui")
endif()

if(${SG_INCLUDE_SPDLOG})
    find_package(spdlog CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "spdlog::spdlog")
endif()

if(${SG_INCLUDE_STB})
    find_package(Stb REQUIRED)
    list(APPEND SungearEngine_INCLUDE_DIRS "${Stb_INCLUDE_DIR}")
endif()

if(${SG_INCLUDE_GLAD})
    find_package(glad CONFIG REQUIRED)
    list(APPEND SungearEngine_LIBS "glad::glad")
endif()
