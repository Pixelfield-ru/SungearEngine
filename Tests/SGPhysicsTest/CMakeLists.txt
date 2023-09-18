 project(${SG_PHYS_TEST_PROJ})

 file(GLOB_RECURSE SGPHYSICSTEST_SOURCES ../SGPhysicsTest/*.cpp ../SGPhysicsTest/*.h)

 add_executable(${PROJECT_NAME} ${SGPHYSICSTEST_SOURCES})

 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/GLM
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         ${CMAKE_SOURCE_DIR}/Externals/bullet3/src
         ${Vulkan_INCLUDE_DIR}
         #${BULLET_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/Sources
         )

 target_link_libraries(${PROJECT_NAME} PRIVATE
         ${SG_CORE_PROJ}
         )
