cmake_minimum_required(VERSION 3.22)

project(SungearEngine)

add_definitions(-DSUNGEAR_DEBUG)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE ON)

message("ToolChain CMAKE_CXX_LINK_EXECUTABLE: ${CMAKE_CXX_LINK_EXECUTABLE}")

add_compile_options(-fPIC)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-implicit-fallthrough -Wno-ignored-qualifiers")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(MSVC)
    set(MSVC_RUNTIME "dynamic")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

# projects --------------------
set(SG_SOURCES_PROJ Sources)
set(SG_CORE_PROJ SGCore)
set(SG_EDITOR_PROJ SGEditor)
set(SG_CONSOLE_PROJ SGConsole)
set(SG_CONSOLE_API_PROJ SGConsoleAPI)
set(SG_CONSOLE_APP_PROJ SGConsoleApp)
set(SG_UTILS_PROJ SGUtils)

set(SG_TESTS_PROJ Tests)
set(SG_PHYS_TEST_PROJ SGPhysicsTest)

# libs ------------------------
# TODO: MAKE SG_CORE_STATIC OR SHARED
set(SG_CORE_DLL ${CMAKE_BINARY_DIR}/lib/${SG_CORE_PROJ}d.dll)
set(SG_UTILS_DLL ${CMAKE_BINARY_DIR}/lib/${SG_UTILS_PROJ}d.dll)

set(SG_SOURCES_BIN ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ})
set(SG_TESTS_BIN ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ})
set(SG_PHYS_TEST_BIN ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ})
set(SG_EDITOR_BIN ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ})

# -----------------------------

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition -Wno-return-type")

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS OFF)

set(SG_RESOURCES_COPY_MODELS ON)
set(SG_RESOURCES_COPY_SHADERS ON)
set(SG_RESOURCES_COPY_TEXTURES ON)

# ----------------------------------

add_subdirectory(syslibs)
add_subdirectory(Externals)
add_subdirectory(Sources)
if(${SG_BUILD_TESTS})
    add_subdirectory(Tests)
endif()

# move all necessary dlls to target exes ------------------

if(EXISTS ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll DESTINATION ${SG_EDITOR_BIN})
endif()

if(MSVC AND EXISTS ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/assimp-vc143-mtd.dll)
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/assimp-vc143-mtd.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/assimp-vc143-mtd.dll DESTINATION ${SG_EDITOR_BIN})
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/lib/glfw3d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_EDITOR_BIN})
endif()