project(SGResources)

if(${SG_RESOURCES_COPY_MODELS})
    # MODELS copy for SOURCES
    add_custom_target(SGMainModelsCopy ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/models
            ${SG_SOURCES_BIN}/${PROJECT_NAME}/models
            COMMENT "Copying MODELS into SOURCES directory")

    add_dependencies(SGEditor SGMainModelsCopy)

    if(${SG_BUILD_TESTS})
        # MODELS copy for TESTS
        add_custom_target(SGTestsModelsCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/models
                ${SG_TESTS_BIN}/${PROJECT_NAME}/models
                COMMENT "Copying MODELS into TESTS directory")

        add_dependencies(SGEditor SGTestsModelsCopy)
    endif()
endif()

if(${SG_RESOURCES_COPY_SHADERS})
    # SHADERS copy for SOURCES
    add_custom_target(SGMainShadersCopy ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/shaders
            ${SG_SOURCES_BIN}/${PROJECT_NAME}/shaders
            COMMENT "Copying SHADERS into SOURCES directory")

    add_dependencies(SGEditor SGMainShadersCopy)

    if(${SG_BUILD_TESTS})
        # SHADERS copy for TESTS
        add_custom_target(SGTestsShadersCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/shaders
                ${SG_TESTS_BIN}/${PROJECT_NAME}/shaders
                COMMENT "Copying SHADERS into TESTS directory")

        add_dependencies(SGEditor SGTestsShadersCopy)
    endif()
endif()

if(${SG_RESOURCES_COPY_TEXTURES})
    # TEXTURES copy for SOURCES
    add_custom_target(SGMainTexturesCopy ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/textures
            ${SG_SOURCES_BIN}/${PROJECT_NAME}/textures
            COMMENT "Copying TEXTURES into SOURCES directory")

    add_dependencies(SGEditor SGMainTexturesCopy)

    if(${SG_BUILD_TESTS})
        # TEXTURES copy for TESTS
        add_custom_target(SGTestsTexturesCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/textures
                ${SG_TESTS_BIN}/${PROJECT_NAME}/textures
                COMMENT "Copying TEXTURES into TESTS directory")

        add_dependencies(SGEditor SGTestsTexturesCopy)
    endif()
endif()
