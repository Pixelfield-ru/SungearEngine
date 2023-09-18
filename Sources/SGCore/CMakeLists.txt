 project(${SG_CORE_PROJ})

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES})

 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/GLM
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         ${CMAKE_SOURCE_DIR}/Externals/bullet3/src
         ${Vulkan_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/Sources
         #${BULLET_INCLUDE_DIR}

         #${CMAKE_SOURCE_DIR}/Externals/Vulkan/Include
         #"C:/VulkanSDK/1.3.250.1/Include"
         )

 # TODO: make for linux and mЫac
 target_link_libraries(${PROJECT_NAME} PUBLIC
         glfw
         glm::glm
         gl46
         ${Vulkan_LIBRARIES}
         assimp
         wsock32
         ws2_32

         LinearMath
         Bullet3Common
         BulletDynamics
         BulletCollision
         BulletInverseDynamics
         BulletSoftBody
         )

#[[ target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Externals/Vulkan/Lib)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} vulkan-1)
else()
  target_link_libraries(${PROJECT_NAME} vulkan)
endif()]]

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         ${SG_CORE_DLL}
         ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ}
         )

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         ${SG_CORE_DLL}
         ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ}
         )

 #[[IF(EXISTS ${SG_CORE_DLL})
    file(COPY ${SG_CORE_DLL} DESTINATION ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ})
    file(COPY ${SG_CORE_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
 ENDIF ()]]
