 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 message(STATUS ${SGCORE_SOURCES})

 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES})

 message(${CMAKE_SOURCE_DIR}/Sources)

 #add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Sources
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         ${CMAKE_SOURCE_DIR}/Externals/Vulkan/Include
         )

 # TODO: make for linux and mac
 target_link_libraries(${PROJECT_NAME}
         glfw
         glm::glm
         gl46
         assimp
         wsock32
         ws2_32
         )

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}d.dll
         $<TARGET_FILE_DIR:SGEditor>
         )