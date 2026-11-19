function(_sgf_collect_targets directory result_name)
    set(collected "")
    foreach(property IN ITEMS IMPORTED_TARGETS BUILDSYSTEM_TARGETS)
        get_property(targets DIRECTORY "${directory}" PROPERTY "${property}")
        if(targets)
            list(APPEND collected ${targets})
        endif()
    endforeach()
    get_property(children DIRECTORY "${directory}" PROPERTY SUBDIRECTORIES)
    foreach(child IN LISTS children)
        _sgf_collect_targets("${child}" nested)
        if(nested)
            list(APPEND collected ${nested})
        endif()
    endforeach()
    set("${result_name}" "${collected}" PARENT_SCOPE)
endfunction()

# find_package, and remember which imported targets that call created.
# sgf_install_package() uses the table so a component does not repeat the package.
macro(sgf_find_package _sgf_fp_name)
    _sgf_collect_targets("${CMAKE_CURRENT_SOURCE_DIR}" _sgf_fp_before)
    find_package(${_sgf_fp_name} ${ARGN})
    _sgf_collect_targets("${CMAKE_CURRENT_SOURCE_DIR}" _sgf_fp_after)

    set(_sgf_fp_args ${ARGN})
    list(REMOVE_ITEM _sgf_fp_args REQUIRED QUIET)
    set(_sgf_fp_spec "${_sgf_fp_name}")
    if(_sgf_fp_args)
        list(JOIN _sgf_fp_args " " _sgf_fp_joined)
        string(APPEND _sgf_fp_spec " ${_sgf_fp_joined}")
    endif()
    set_property(GLOBAL PROPERTY "SGF_PACKAGE_SPEC_${_sgf_fp_name}" "${_sgf_fp_spec}")

    foreach(_sgf_fp_target IN LISTS _sgf_fp_after)
        if(NOT _sgf_fp_target IN_LIST _sgf_fp_before)
            string(REPLACE "::" "__" _sgf_fp_key "${_sgf_fp_target}")
            set_property(GLOBAL PROPERTY "SGF_TARGET_${_sgf_fp_key}_PACKAGE" "${_sgf_fp_name}")
        endif()
    endforeach()

    unset(_sgf_fp_before)
    unset(_sgf_fp_after)
    unset(_sgf_fp_args)
    unset(_sgf_fp_spec)
    unset(_sgf_fp_joined)
    unset(_sgf_fp_target)
    unset(_sgf_fp_key)
endmacro()

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
    set(multiValueArgs "")
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
    if(PARAMS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "install_component(${PARAMS_TARGET_NAME}) does not accept: ${PARAMS_UNPARSED_ARGUMENTS}. Link dependencies with target_link_libraries().")
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
endfunction()

function(_sgf_linked_target raw resolved_name)
    set(name "${raw}")
    set(guard 0)
    while(name MATCHES "^\\$<(LINK_ONLY|BUILD_INTERFACE|INSTALL_INTERFACE):(.*)>$")
        set(name "${CMAKE_MATCH_2}")
        math(EXPR guard "${guard} + 1")
        if(guard GREATER 8)
            break()
        endif()
    endwhile()
    if(name MATCHES "^\\$<" OR name MATCHES "^(debug|optimized|general)$")
        set(name "")
    endif()
    set("${resolved_name}" "${name}" PARENT_SCOPE)
endfunction()

function(sgf_install_package)
    get_property(components GLOBAL PROPERTY SGF_COMPONENTS)
    if(NOT components)
        message(FATAL_ERROR "sgf_install_package() found no components. Call install_component() first.")
    endif()

    set(dependencies "")
    foreach(component IN LISTS components)
        if(NOT TARGET "${component}")
            message(FATAL_ERROR "sgf_install_package() cannot find target '${component}'.")
        endif()

        set(link_items "")
        foreach(property IN ITEMS LINK_LIBRARIES INTERFACE_LINK_LIBRARIES)
            get_target_property(values "${component}" "${property}")
            if(values AND NOT values MATCHES "-NOTFOUND$")
                list(APPEND link_items ${values})
            endif()
        endforeach()

        set(component_depends "")
        set(package_names "")
        set(package_specs "")
        foreach(raw IN LISTS link_items)
            _sgf_linked_target("${raw}" linked)
            if(linked STREQUAL "")
                continue()
            endif()
            if(NOT TARGET "${linked}")
                message(FATAL_ERROR "sgf component '${component}' links '${raw}', which is not a target.")
            endif()

            set(resolved "${linked}")
            get_target_property(aliased "${linked}" ALIASED_TARGET)
            if(aliased AND NOT aliased MATCHES "-NOTFOUND$")
                set(resolved "${aliased}")
            endif()
            if(resolved STREQUAL component)
                continue()
            endif()
            if(resolved IN_LIST components)
                if(NOT resolved IN_LIST component_depends)
                    list(APPEND component_depends "${resolved}")
                endif()
                continue()
            endif()

            string(REPLACE "::" "__" key "${resolved}")
            get_property(package_name GLOBAL PROPERTY "SGF_TARGET_${key}_PACKAGE")
            if(NOT package_name)
                message(FATAL_ERROR "sgf component '${component}' links '${linked}', but that target was not created by sgf_find_package().")
            endif()
            if(NOT package_name IN_LIST package_names)
                get_property(package_spec GLOBAL PROPERTY "SGF_PACKAGE_SPEC_${package_name}")
                list(APPEND package_names "${package_name}")
                list(APPEND package_specs "${package_spec}")
            endif()
        endforeach()

        string(REPLACE ";" "|" packages_joined "${package_specs}")
        string(APPEND dependencies "set(_sgf_${component}_deps \"${component_depends}\")\n")
        string(APPEND dependencies "set(_sgf_${component}_packages \"${packages_joined}\")\n")
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
