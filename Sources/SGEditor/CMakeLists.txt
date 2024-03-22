project(SGEditor)

file(GLOB_RECURSE SGEDITOR_SOURCES ../SGEditor/*.cpp ../SGEditor/*.h)

find_package(spdlog CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SGEDITOR_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
        SGConsoleAPI # includes SGCore
        )

if (${SG_RESOURCES_COPY_MODELS})
        add_custom_target(SGEditorModelCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/models ${CMAKE_BINARY_DIR}/Sources/SGResources/models)

        add_dependencies(${PROJECT_NAME} SGEditorModelCopy)
endif ()

if (${SG_RESOURCES_COPY_SHADERS})
        add_custom_target(SGEditorShadersCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/shaders ${CMAKE_BINARY_DIR}/Sources/SGResources/shaders)

        add_dependencies(${PROJECT_NAME} SGEditorShadersCopy)
endif ()

if (${SG_RESOURCES_COPY_TEXTURES})
        add_custom_target(SGEditorTexturesCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/textures ${CMAKE_BINARY_DIR}/Sources/SGResources/textures)

        add_dependencies(${PROJECT_NAME} SGEditorTexturesCopy)
endif ()

if (${SG_RESOURCES_COPY_UI})
        add_custom_target(SGEditorUICopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/ui ${CMAKE_BINARY_DIR}/Sources/SGResources/ui)

        add_dependencies(${PROJECT_NAME} SGEditorUICopy)
endif ()

if (${SG_RESOURCES_COPY_FONTS})
        add_custom_target(SGEditorFontsCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/fonts ${CMAKE_BINARY_DIR}/Sources/SGResources/fonts)

        add_dependencies(${PROJECT_NAME} SGEditorFontsCopy)
endif ()
