 project(SGConsoleApp)

 file(GLOB_RECURSE SGCONSOLEAPP_SOURCES ../App/*.cpp ../App/*.h)

 #add_library(${PROJECT_NAME} SHARED ${SGCONSOLE_SOURCES})
 add_executable(${PROJECT_NAME} ${SGCONSOLEAPP_SOURCES})

 #add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Sources
         )

 target_link_libraries(${PROJECT_NAME}
         SGConsoleAPI
         )