 project(SGConsoleApp)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCONSOLEAPP_SOURCES ../App/*.cpp ../App/*.h)

 #add_library(${PROJECT_NAME} SHARED ${SGCONSOLE_SOURCES})
 add_executable(${PROJECT_NAME} ${SGCONSOLEAPP_SOURCES})

 message(${SGCONSOLEAPP_SOURCES})

 #add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Sources
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         )

 target_link_libraries(${PROJECT_NAME}
         SGConsoleAPI
         )

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.exe
         $<TARGET_FILE_DIR:SGEditor>
         )