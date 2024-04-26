project(SGEntry)

file(GLOB_RECURSE SGENTRY_SOURCES *.cpp *.h)

find_package(spdlog CONFIG REQUIRED)

set(SG_EXEC_NAME SungearEngine)

add_executable(${SG_EXEC_NAME} ${SGENTRY_SOURCES})

target_include_directories(${SG_EXEC_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
        )

target_link_libraries(${SG_EXEC_NAME} PUBLIC
        spdlog::spdlog
        SGConsoleAPI # includes SGCore
        )

# ========== installing standard plugins ===========================

message("cur bin dir: " ${CMAKE_CURRENT_BINARY_DIR})

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        add_custom_command(TARGET ${SG_EXEC_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/Plugins/SungearEngineEditor/cmake-build-debug/SungearEngineEditor.so
                ${CMAKE_CURRENT_BINARY_DIR}/Plugins/SungearEngineEditor/cmake-build-debug/SungearEngineEditor.so)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
        add_custom_command(TARGET ${SG_EXEC_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/Plugins/SungearEngineEditor/cmake-build-release/SungearEngineEditor.so
                ${CMAKE_CURRENT_BINARY_DIR}/Plugins/SungearEngineEditor/cmake-build-release/SungearEngineEditor.so)
endif()

# ==================================================================

if (${SG_RESOURCES_COPY_MODELS})
        add_custom_target(${SG_EXEC_NAME}ModelSCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/models ${CMAKE_BINARY_DIR}/Sources/SGResources/models)

        add_dependencies(${SG_EXEC_NAME} ${SG_EXEC_NAME}ModelSCopy)
endif ()

if (${SG_RESOURCES_COPY_SHADERS})
        add_custom_target(${SG_EXEC_NAME}ShadersCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/shaders ${CMAKE_BINARY_DIR}/Sources/SGResources/shaders)

        add_dependencies(${SG_EXEC_NAME} ${SG_EXEC_NAME}ShadersCopy)
endif ()

if (${SG_RESOURCES_COPY_TEXTURES})
        add_custom_target(${SG_EXEC_NAME}TexturesCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/textures ${CMAKE_BINARY_DIR}/Sources/SGResources/textures)

        add_dependencies(${SG_EXEC_NAME} ${SG_EXEC_NAME}TexturesCopy)
endif ()

if (${SG_RESOURCES_COPY_UI})
        add_custom_target(${SG_EXEC_NAME}UICopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/ui ${CMAKE_BINARY_DIR}/Sources/SGResources/ui)

        add_dependencies(${SG_EXEC_NAME} ${SG_EXEC_NAME}UICopy)
endif ()

if (${SG_RESOURCES_COPY_FONTS})
        add_custom_target(${SG_EXEC_NAME}FontsCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Resources/SGResources/fonts ${CMAKE_BINARY_DIR}/Sources/SGResources/fonts)

        add_dependencies(${SG_EXEC_NAME} ${SG_EXEC_NAME}FontsCopy)
endif ()
