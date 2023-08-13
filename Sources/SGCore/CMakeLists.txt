 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES} Patterns/Singleton.h Graphics/API/GL/GL4/GL4FrameBuffer.cpp Graphics/API/GL/GL4/GL4FrameBuffer.h Graphics/API/IFrameBufferAttachment.h Graphics/API/GL/GLFrameBufferAttachment.h ECS/Rendering/DirectionalLightComponent.h ECS/Rendering/ShadowsCasterComponent.h ECS/Rendering/ShadowsCasterSystem.cpp ECS/Rendering/ShadowsCasterSystem.h Utils/Defines.h ECS/Rendering/IRenderingComponent.h ECS/Rendering/RenderingComponentsSystem.cpp ECS/Rendering/RenderingComponentsSystem.h Graphics/API/Vulkan/VkFrameBuffer.cpp Graphics/API/Vulkan/VkFrameBuffer.h Patterns/Marker.h Memory/Collections/BitVector.cpp Memory/Collections/BitVector.h ECS/Rendering/ILightComponent.h ECS/Rendering/DirectionalLightsSystem.cpp ECS/Rendering/DirectionalLightsSystem.h ECS/Rendering/SkyboxComponent.h ECS/Rendering/SkyboxRenderingSystem.cpp ECS/Rendering/SkyboxRenderingSystem.h)

 #add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         ${Vulkan_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/Sources
         #${CMAKE_SOURCE_DIR}/Externals/Vulkan/Include
         #"C:/VulkanSDK/1.3.250.1/Include"
         )

 # TODO: make for linux and mЫac
 target_link_libraries(${PROJECT_NAME}
         glfw
         glm::glm
         gl46
         ${Vulkan_LIBRARIES}
         assimp
         wsock32
         ws2_32
         )

#[[ target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Externals/Vulkan/Lib)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} vulkan-1)
else()
  target_link_libraries(${PROJECT_NAME} vulkan)
endif()]]

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}d.dll
         $<TARGET_FILE_DIR:SGEditor>
         )