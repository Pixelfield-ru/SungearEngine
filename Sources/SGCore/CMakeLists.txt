 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 message(STATUS ${SGCORE_SOURCES})

 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES})

 message(${CMAKE_SOURCE_DIR}/Sources)

 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Sources
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         )

 target_link_libraries(${PROJECT_NAME}
         glfw
         glm::glm
         gl46
         assimp
         )