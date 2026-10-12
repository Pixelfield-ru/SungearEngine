#
# https://jonathanhamberg.com/post/cmake-embedding-git-hash/
#

set(
    _handle_version_current_dir
    ${CMAKE_CURRENT_LIST_DIR})

function(handle_version)
    set(options)
    set(oneValueArgs
        NAME
        TARGET_NAME)
    set(multiValueArgs)
    cmake_parse_arguments(
        PARAM
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})
    string(
        TIMESTAMP
        build_date
        "%Y-%m-%dT%H:%M:%SZ"
        UTC)
    set(
        build_branch
        $ENV{pixelfield_build_branch})
    set(
        build_commit
        $ENV{pixelfield_build_commit})
    set(
        build_number
        $ENV{pixelfield_build_number})
    set(
        build_version
        $ENV{pixelfield_build_version})
    if(NOT DEFINED PARAM_NAME)
        set(PARAM_NAME ${PROJECT_NAME})
    endif()
    if(NOT DEFINED PARAM_TARGET_NAME)
        set(PARAM_TARGET_NAME ${PROJECT_NAME})
    endif()
    if(NOT TARGET ${PARAM_TARGET_NAME})
        message(FATAL_ERROR
            "handle_version: target '${PARAM_TARGET_NAME}' does not exist. "
            "Call handle_version() after add_library().")
    endif()
    set(tdir "${CMAKE_CURRENT_BINARY_DIR}/generated")
    file(MAKE_DIRECTORY "${tdir}/include/${LIBRARY_NAMESPACE}/${PARAM_NAME}")
    file(MAKE_DIRECTORY "${tdir}/sources")
    configure_file(
        ${_handle_version_current_dir}/version.h.in
        ${tdir}/include/${LIBRARY_NAMESPACE}/${PARAM_NAME}/version.h
        @ONLY)
    configure_file(
        ${_handle_version_current_dir}/version_thin.cpp.in
        ${tdir}/sources/version_thin.cpp
        @ONLY)
    configure_file(
        ${_handle_version_current_dir}/version_wide.cpp.in
        ${tdir}/sources/version_wide.cpp
        @ONLY)
    configure_file(
        ${_handle_version_current_dir}/version_u8.cpp.in
        ${tdir}/sources/version_u8.cpp
        @ONLY)
    target_sources(
        ${PARAM_TARGET_NAME}
        PRIVATE
            "${tdir}/include/${LIBRARY_NAMESPACE}/${PARAM_NAME}/version.h"
            "${tdir}/sources/version_thin.cpp"
            "${tdir}/sources/version_wide.cpp"
            "${tdir}/sources/version_u8.cpp")
    target_include_directories(
        ${PARAM_TARGET_NAME}
        PRIVATE
            "${tdir}/include")
endfunction()
