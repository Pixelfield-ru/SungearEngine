cmake_minimum_required(VERSION 3.22)

project(SungearEngine)

#set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")

#message("CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR}")

add_definitions(-DSUNGEAR_DEBUG)
add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
#add_definitions(-DBOOST_STACKTRACE_LINK)
add_definitions(-DHC_NO_COMPRESSION)
add_definitions(-DNOMINMAX)
#add_definitions(-DCMAKE_TOOLCHAIN_FILE="C:\\Users\\Ilya\\.clion-vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_TOOLCHAIN_FILE "C:\\Users\\Ilya\\.clion-vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")

message("ToolChain CMAKE_CXX_LINK_EXECUTABLE: ${CMAKE_CXX_LINK_EXECUTABLE}")

IF(UNIX)
    add_compile_options(-fPIC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-implicit-fallthrough -Wno-ignored-qualifiers -g -rdynamic -fno-pie -no-pie")
    set(CMAKE_CXX_FLAGS_DEBUG "-O1")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    #[[set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage --coverage")
    set(GCOV_CFLAGS "-fprofile-arcs -ftest-coverage --coverage")
    set(GCOV_LDFLAGS "-fprofile-arcs -ftest-coverage")
    set(GCOV_LIBS "lgcov")]]
elseif(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    set(MSVC_RUNTIME "dynamic")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS FALSE)
endif ()

# multiple definition because of bullet library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition -Wno-return-type")
#set(CMAKE_STATIC_LINKER_FLAGS "-lgcov")

# options --------------------------

# TODO: make SG_BUILD_EDITOR
set(SG_BUILD_TESTS OFF)

set(SG_RESOURCES_COPY_MODELS ON)
set(SG_RESOURCES_COPY_SHADERS ON)
set(SG_RESOURCES_COPY_TEXTURES ON)
set(SG_RESOURCES_COPY_UI ON)
set(SG_RESOURCES_COPY_FONTS ON)

# ----------------------------------
add_subdirectory(Externals)
add_subdirectory(Sources)
if(${SG_BUILD_TESTS})
    add_subdirectory(Tests)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

message("Bullet Include Dir: " ${BULLET_INCLUDE_DIR})
message("Bullet Libraries: " ${BULLET_COLLISION_LIBRARY} ", " ${BULLET_DYNAMICS_LIBRARY} ", " ${BULLET_MATH_LIBRARY})

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
endif ()