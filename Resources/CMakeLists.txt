project(SGResources)

# copying resources to binary file
add_custom_target(SGResourcesCopy ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/Sources
        COMMENT "Copying resources into binary directory")

add_dependencies(SGEditor SGResourcesCopy)

# does not working --------------------------------
#set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
#message(sdfsdf =${CMAKE_INSTALL_PREFIX})

#install(DIRECTORY ../SGResources DESTINATION Sources)