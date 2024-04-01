add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

add_subdirectory(SGUtils)
add_subdirectory(SGCore)
add_subdirectory(SGConsole)
add_subdirectory(SGEntry)

message("LIBDIR: " ${CMAKE_INSTALL_LIBDIR})

# installing targets
install(TARGETS SGUtils SGCore SGConsoleAPI
        EXPORT SungearEngineTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

export(
        EXPORT SungearEngineTargets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/SungearEngineTargets.cmake
)

install(
        EXPORT SungearEngineTargets
        FILE SungearEngineTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/SungearEngine/cmake
)

#installing headers
file(GLOB_RECURSE SGCONSOLEAPI_HEADERS SGConsole/API/*.h)
file(GLOB_RECURSE SGCORE_HEADERS SGCore/*.h)
file(GLOB_RECURSE SGUTILS_HEADERS SGUtils/*.h)

#message("core headers: " ${SGCORE_HEADERS})

install(DIRECTORY "SGConsole" # source directory
        DESTINATION "include/SungearEngine" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        PATTERN "*.hpp"
        )

install(DIRECTORY "SGCore" # source directory
        DESTINATION "include/SungearEngine" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        PATTERN "*.hpp"
        )

install(DIRECTORY "SGUtils" # source directory
        DESTINATION "include/SungearEngine" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        PATTERN "*.hpp"
        )

install(FILES "sgcore_export.h" # source directory
        DESTINATION "include/SungearEngine" # target directory
)

install(FILES "sgutils_export.h" # source directory
        DESTINATION "include/SungearEngine" # target directory
)