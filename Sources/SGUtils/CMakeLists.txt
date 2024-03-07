project(SGUtils)

SET(CMAKE_SHARED_LIBRARY_PREFIX "")

file(GLOB_RECURSE SGUTILS_SOURCES ../SGUtils/*.cpp ../SGUtils/*.h)

add_library(${PROJECT_NAME} SHARED ${SGUTILS_SOURCES})

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

message("sgutils bin: " ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
        ${CMAKE_SOURCE_DIR}/Externals/GLM
        ${CMAKE_SOURCE_DIR}/Externals/spdlog/include
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        glm::glm
        ${spdlog_lib}
)

if(MSVC)
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/SGUtils.dll)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/SGUtils.dll
                ${CMAKE_BINARY_DIR}/Sources/SGEditor
                )
    ENDIF()
endif()
