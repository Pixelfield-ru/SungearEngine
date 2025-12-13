include(../../cmake/utils.cmake)

project(SGModelDrawTest)

set(SGPROJECT_SOURCES Main.cpp Main.h)

find_package(spdlog CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SGPROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
        SGCore
)

copy_sgcore_dlls(${PROJECT_NAME})