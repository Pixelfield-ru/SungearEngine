include(../../cmake/utils.cmake)

project(SGPhysicsTest)

file(GLOB_RECURSE SGPHYSICSTEST_SOURCES Main.cpp Main.h)

set(SG_EXEC_NAME ${PROJECT_NAME})

find_package(spdlog CONFIG REQUIRED)

add_executable(${SG_EXEC_NAME} ${SGPHYSICSTEST_SOURCES})

target_include_directories(${SG_EXEC_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
)

target_link_libraries(${SG_EXEC_NAME} PUBLIC
        spdlog::spdlog
        SGCore
)

copy_sgcore_dlls(${SG_EXEC_NAME})
