set(SungearEngine_INCLUDE_DIRS "${SUNGEAR_ENGINE_DIR}/Sources")
list(APPEND SungearEngine_INCLUDE_DIRS "${SUNGEAR_ENGINE_DIR}/cmake-build-release/Sources/SGCore")
list(APPEND SungearEngine_INCLUDE_DIRS "${SUNGEAR_ENGINE_DIR}/cmake-build-release/Sources/SGUtils")
list(APPEND SungearEngine_INCLUDE_DIRS "${SUNGEAR_ENGINE_DIR}/Externals")

set(SungearEngine_LIBS "${SUNGEAR_ENGINE_DIR}/cmake-build-release/Sources/SGCore/SGCore.so")
list(APPEND SungearEngine_LIBS "${SUNGEAR_ENGINE_DIR}/cmake-build-release/Sources/SGUtils/SGUtils.so")
list(APPEND SungearEngine_LIBS "${SUNGEAR_ENGINE_DIR}/cmake-build-release/Sources/SGConsole/API/SGConsoleAPI.so")

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
endif()