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
        ${CMAKE_SOURCE_DIR}/Externals/spdlog/include
        )

target_link_libraries(${PROJECT_NAME}
        SGConsoleAPI # includes SGCore
        )

if (${SG_RESOURCES_COPY_MODELS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/models ${CMAKE_BINARY_DIR}/Sources/SGResources/models)
endif ()

if (${SG_RESOURCES_COPY_SHADERS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/shaders ${CMAKE_BINARY_DIR}/Sources/SGResources/shaders)
endif ()

if (${SG_RESOURCES_COPY_TEXTURES})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/textures ${CMAKE_BINARY_DIR}/Sources/SGResources/textures)
endif ()