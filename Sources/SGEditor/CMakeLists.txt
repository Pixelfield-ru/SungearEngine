project(SGEditor)

file(GLOB_RECURSE SGEDITOR_SOURCES ../SGEditor/*.cpp ../SGEditor/*.h)

add_executable(${PROJECT_NAME} ${SGEDITOR_SOURCES})

message(${CMAKE_SOURCE_DIR}/Sources)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
        ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
        ${CMAKE_SOURCE_DIR}/Externals/stb
        )

target_link_libraries(${PROJECT_NAME}
        SGConsole
        )

# move all necessary dlls to target exe
add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE_DIR:SGCore>/SGCored.dll
        $<TARGET_FILE_DIR:SGConsole>/SGConsoled.dll
        ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll
        ${CMAKE_BINARY_DIR}/Externals/GLFW/src/glfw3d.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )