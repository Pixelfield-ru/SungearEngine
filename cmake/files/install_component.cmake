function(install_component_validate)
    if((NOT DEFINED LIBRARY_NAMESPACE)
        OR (NOT LIBRARY_NAMESPACE))
        message(FATAL_ERROR [==[

    Variable 'LIBRARY_NAMESPACE' is empty or not set.

    This variable is required by install_component() to correctly
    namespace exported targets.

    Define it in your root CMakeLists.txt:

        set(LIBRARY_NAMESPACE <your_namespace_prefix>)

    ]==])
    endif()
endfunction()

function(install_component)
    set(options
        NO_HEADERS)
    set(oneValueArgs
        INSTALL_DIR_REGEX
        TARGET_NAME)
    set(multiValueArgs
        DEPENDS)
    cmake_parse_arguments(
        PARAMS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})
    install_component_validate()

    if(NOT PARAMS_TARGET_NAME)
        message(FATAL_ERROR "install_component() requires TARGET_NAME.")
    endif()

    string(
        REPLACE "-" "::"
        LIBRARY_NAMESPACE_COLON
        ${LIBRARY_NAMESPACE})

    if(NOT PARAMS_NO_HEADERS)
        set(tdir "${PROJECT_SOURCE_DIR}")
        if(EXISTS "${tdir}/include/")
            install(
                DIRECTORY "${tdir}/include/"
                DESTINATION include
                COMPONENT ${PARAMS_TARGET_NAME})
        endif()
        if(DEFINED PARAMS_INSTALL_DIR_REGEX)
            install(
                DIRECTORY "${tdir}/include/"
                DESTINATION include
                COMPONENT ${PARAMS_TARGET_NAME}
                REGEX ${PARAMS_INSTALL_DIR_REGEX}
                EXCLUDE)
        endif()
    endif()

    install(
        TARGETS ${PARAMS_TARGET_NAME}
        EXPORT ${LIBRARY_NAMESPACE}-${PARAMS_TARGET_NAME}-targets
        COMPONENT ${PARAMS_TARGET_NAME})
    install(
        EXPORT ${LIBRARY_NAMESPACE}-${PARAMS_TARGET_NAME}-targets
        FILE ${LIBRARY_NAMESPACE}-${PARAMS_TARGET_NAME}-targets.cmake
        NAMESPACE ${LIBRARY_NAMESPACE_COLON}::
        DESTINATION lib/cmake/${LIBRARY_NAMESPACE}
        COMPONENT ${PARAMS_TARGET_NAME})

    set_property(GLOBAL APPEND PROPERTY SGF_COMPONENTS "${PARAMS_TARGET_NAME}")
    set_property(GLOBAL PROPERTY SGF_COMPONENT_${PARAMS_TARGET_NAME}_DEPENDS "${PARAMS_DEPENDS}")
endfunction()

function(sgf_install_package)
    get_property(components GLOBAL PROPERTY SGF_COMPONENTS)
    if(NOT components)
        message(FATAL_ERROR "sgf_install_package() found no components. Call install_component() first.")
    endif()

    set(dependencies "")
    foreach(component IN LISTS components)
        get_property(component_depends GLOBAL PROPERTY SGF_COMPONENT_${component}_DEPENDS)
        string(APPEND dependencies "set(_sgf_${component}_deps \"${component_depends}\")\n")
    endforeach()

    set(SGF_COMPONENTS "${components}")
    set(SGF_COMPONENT_DEPENDENCIES "${dependencies}")

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sgfConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/sgfConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/${LIBRARY_NAMESPACE}")
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/sgfConfig.cmake"
        DESTINATION "lib/cmake/${LIBRARY_NAMESPACE}")
endfunction()
