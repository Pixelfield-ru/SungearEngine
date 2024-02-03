cmake_minimum_required(VERSION 3.22)

project(SungearEngine)

add_definitions(-DSUNGEAR_DEBUG)
#add_link_options("-fuse-ld=lld")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE ON)

#find_package(LLVM REQUIRED CONFIG)


#set(CMAKE_LINKER "C:\\Program Files\\LLVM\\bin\\lld-link.exe")
#set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

#set(CMAKE_LINKER lld-link CACHE STRING "C:\\Program Files\\LLVM\\bin\\lld.exe")
#set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> /out:<TARGET>" CACHE STRING "")

message("ToolChain CMAKE_CXX_LINK_EXECUTABLE: ${CMAKE_CXX_LINK_EXECUTABLE}")

#SET(CMAKE_CXX_LINK_EXECUTABLE "C:\\Program Files\\LLVM\\bin\\lld.exe")

#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#[[set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")]]

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
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

# FOR LINUX
#set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition")

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS OFF)

set(SG_RESOURCES_COPY_MODELS OFF)
set(SG_RESOURCES_COPY_SHADERS OFF)
set(SG_RESOURCES_COPY_TEXTURES OFF)

# ----------------------------------

#include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})

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

if(MSVC AND EXISTS ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/assimp-vc143-mtd.dll)
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/assimp-vc143-mtd.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/assimp-vc143-mtd.dll DESTINATION ${SG_EDITOR_BIN})
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/lib/glfw3d.dll)
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${CMAKE_BINARY_DIR}/lib/glfw3d.dll DESTINATION ${SG_EDITOR_BIN})
endif()