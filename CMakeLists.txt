cmake_minimum_required(VERSION 3.25)

project(SungearEngine)

add_definitions(-DSUNGEAR_DEBUG)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE ON)

#set(BUILD_SHARED_LIBS ON)

# projects --------------------
set(SG_SOURCES_PROJ Sources)
set(SG_CORE_PROJ SGCore)
set(SG_EDITOR_PROJ SGEditor)
set(SG_CONSOLE_PROJ SGConsole)
set(SG_CONSOLE_API_PROJ SGConsoleAPI)
set(SG_CONSOLE_APP_PROJ SGConsoleApp)

set(SG_TESTS_PROJ Tests)
set(SG_PHYS_TEST_PROJ SGPhysicsTest)

# libs ------------------------
set(SG_CORE_DLL ${CMAKE_BINARY_DIR}/lib/${SG_CORE_PROJ}d.dll)

set(SG_SOURCES_BIN ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ})
set(SG_TESTS_BIN ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ})
set(SG_PHYS_TEST_BIN ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ})
set(SG_EDITOR_BIN ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ})

# -----------------------------

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -Wl,-allow-multiple-definition")
set(CMAKE_BUILD_TYPE "Debug")

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS OFF)

set(SG_RESOURCES_COPY_MODELS OFF)
set(SG_RESOURCES_COPY_SHADERS ON)
set(SG_RESOURCES_COPY_TEXTURES OFF)

# ----------------------------------

include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})

add_subdirectory(syslibs)
add_subdirectory(Externals)
add_subdirectory(Sources)
if(${SG_BUILD_TESTS})
    add_subdirectory(Tests)
endif()
add_subdirectory(Resources)

# move all necessary dlls to target exes ------------------

if(EXISTS ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll DESTINATION ${SG_EDITOR_BIN})
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/lib/glfw3d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_EDITOR_BIN})
endif()