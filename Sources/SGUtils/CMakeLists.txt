project(${SG_UTILS_PROJ})

SET(CMAKE_SHARED_LIBRARY_PREFIX "")

file(GLOB_RECURSE SGUTILS_SOURCES ../SGUtils/*.cpp ../SGUtils/*.h)

add_library(${PROJECT_NAME} SHARED ${SGUTILS_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
        ${CMAKE_SOURCE_DIR}/Externals/GLM
        ${CMAKE_SOURCE_DIR}/Externals/spdlog/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        glm::glm
        ${spdlog_lib}
)

IF(EXISTS ${SG_UTILS_DLL})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${SG_UTILS_DLL}
            ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ}
    )

    IF(${SG_BUILD_TESTS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${SG_UTILS_DLL}
                ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ}
        )
    ENDIF()
ENDIF()
