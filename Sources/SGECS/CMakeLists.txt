project(${SG_ECS_PROJ})

SET(CMAKE_SHARED_LIBRARY_PREFIX "")

file(GLOB_RECURSE SGECS_SOURCES ../SGECS/*.cpp ../SGECS/*.h)

add_library(${PROJECT_NAME} SHARED ${SGECS_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        tsl::sparse_map

        ${SG_UTILS_PROJ}
)

IF(EXISTS ${SG_ECS_DLL})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${SG_ECS_DLL}
            ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ}
    )

    IF(${SG_BUILD_TESTS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${SG_ECS_DLL}
                ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ}
        )
    ENDIF()
ENDIF()
