 project(SGConsole)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCONSOLE_SOURCES ../SGConsole/*.cpp ../SGConsole/*.h)

 add_library(${PROJECT_NAME} SHARED ${SGCONSOLE_SOURCES})

 message(${SGCONSOLE_SOURCES})

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