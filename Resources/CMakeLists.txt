project(SGResources)

file(COPY ${PROJECT_SOURCE_DIR}/${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/Tests)

add_custom_target(SGResourcesCopy ALL
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}
                ${CMAKE_BINARY_DIR}/Sources

                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}
                ${CMAKE_BINARY_DIR}/Tests
                COMMENT "Copying resources into TESTS AND SOURCES directory")

add_dependencies(SGEditor SGResourcesCopy)
add_dependencies(SGPhysicsTest SGResourcesCopy)


# copying resources to binary file
#file(COPY ${PROJECT_SOURCE_DIR}/${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/Sources)
#file(COPY ${PROJECT_SOURCE_DIR}/${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/Tests)