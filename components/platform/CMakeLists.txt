get_filename_component(
    PROJECT_NAME_TEMP
    ${CMAKE_CURRENT_SOURCE_DIR}
    NAME
)

project(
    ${PROJECT_NAME_TEMP}
    VERSION 1.0
    LANGUAGES CXX
)

give_files(
    VAR headers_public
    DIRS "${PROJECT_SOURCE_DIR}/include"
    FILTERS "*.h"
)

give_files(
    VAR headers_private
    DIRS "${PROJECT_SOURCE_DIR}/sources"
    FILTERS "*.h"
)

give_files(
    VAR source_units
    DIRS "${PROJECT_SOURCE_DIR}/sources"
    FILTERS "*.cpp"
)

give_dirs(
    VAR include_dirs
    DIRS "${PROJECT_SOURCE_DIR}/sources"
)

add_library(
    ${PROJECT_NAME} STATIC
    ${source_units}
)

add_library(
    ${LIBRARY_NAMESPACE}::${PROJECT_NAME}
    ALIAS ${PROJECT_NAME}
)

handle_version()

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    glfw

    pixelfield::core
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${sources_include_dirs}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

install_component(
    TARGET_NAME ${PROJECT_NAME}
)