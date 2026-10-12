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
    set(multiValueArgs)
    cmake_parse_arguments(
        PARAMS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})
    install_component_validate()
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
        EXPORT ${LIBRARY_NAMESPACE}-${PROJECT_NAME}Config
        COMPONENT ${PARAMS_TARGET_NAME})
    install(
        EXPORT ${LIBRARY_NAMESPACE}-${PROJECT_NAME}Config
        NAMESPACE ${LIBRARY_NAMESPACE_COLON}::${PROJECT_NAME}::
        DESTINATION lib/cmake/${LIBRARY_NAMESPACE}-${PROJECT_NAME}
        COMPONENT ${PARAMS_TARGET_NAME})
endfunction()
