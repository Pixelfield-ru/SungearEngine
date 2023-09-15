project(SGEditor)

file(GLOB_RECURSE SGEDITOR_SOURCES ../SGEditor/*.cpp ../SGEditor/*.h)

add_executable(${PROJECT_NAME} ${SGEDITOR_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
        ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
        ${CMAKE_SOURCE_DIR}/Externals/stb
        ${CMAKE_SOURCE_DIR}/Externals/GLM
        )

target_link_libraries(${PROJECT_NAME}
        SGConsoleAPI # includes SGCore
        )
