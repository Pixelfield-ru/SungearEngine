 project(SGConsoleAPI)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")
 set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-allow-multiple-definition")

 file(GLOB_RECURSE SGCONSOLEAPI_SOURCES ../API/*.cpp ../API/*.h)

 add_library(${PROJECT_NAME} SHARED ${SGCONSOLEAPI_SOURCES})

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
         SGCore
         )

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}d.dll
         $<TARGET_FILE_DIR:SGEditor>
         )