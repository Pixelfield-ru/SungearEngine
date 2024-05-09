project(SGUtils)

SET(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE SGUTILS_SOURCES ../SGUtils/*.cpp ../SGUtils/*.h)

add_library(${PROJECT_NAME} SHARED ${SGUTILS_SOURCES})

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Externals>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Externals/boost/build/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sources>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        glm::glm
        spdlog::spdlog
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
