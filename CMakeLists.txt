cmake_minimum_required(VERSION 3.25)

project(SungearEngine)

add_definitions(-DSUNGEAR_DEBUG)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE ON)

#set(BUILD_SHARED_LIBS ON)

# projects --------------------
SET(SG_SOURCES_PROJ Sources)
SET(SG_CORE_PROJ SGCore)
SET(SG_EDITOR_PROJ SGEditor)
SET(SG_CONSOLE_PROJ SGConsole)
SET(SG_CONSOLE_API_PROJ SGConsoleAPI)
SET(SG_CONSOLE_APP_PROJ SGConsoleApp)

SET(SG_TESTS_PROJ Tests)
SET(SG_PHYS_TEST_PROJ SGPhysicsTest)

# libs ------------------------
SET(SG_CORE_DLL ${CMAKE_BINARY_DIR}/lib/${SG_CORE_PROJ}d.dll)

SET(SG_PHYS_TEST_BIN ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ})
SET(SG_EDITOR_BIN ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ})

# -----------------------------

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -Wl,-allow-multiple-definition")
set(CMAKE_BUILD_TYPE "Debug")

include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})

add_subdirectory(syslibs)
add_subdirectory(Externals)
add_subdirectory(Sources)
add_subdirectory(Tests)
add_subdirectory(Resources)

# move all necessary dlls to target exes ------------------

IF(EXISTS ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll DESTINATION ${SG_EDITOR_BIN})
ENDIF ()

IF(EXISTS ${CMAKE_BINARY_DIR}/lib/glfw3d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_EDITOR_BIN})
ENDIF ()