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

add_executable(
    ${PROJECT_NAME}
    ${source_units}
)

handle_version()

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    glm::glm

    sgf::runtime
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${sources_include_dirs}
        ${Stb_INCLUDE_DIR}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
)

install_component(
    TARGET_NAME ${PROJECT_NAME}
)