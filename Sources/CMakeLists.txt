add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

add_subdirectory(SGUtils)
add_subdirectory(SGCore)
add_subdirectory(SGConsole)
add_subdirectory(SGEntry)

# installing targets
install(TARGETS SGUtils SGCore SGConsoleAPI)

#installing headers
file(GLOB_RECURSE SGCONSOLEAPI_HEADERS SGConsole/API/*.h)
file(GLOB_RECURSE SGCORE_HEADERS SGCore/*.h)
file(GLOB_RECURSE SGUTILS_HEADERS SGUtils/*.h)

message("core headers: " ${SGCORE_HEADERS})

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