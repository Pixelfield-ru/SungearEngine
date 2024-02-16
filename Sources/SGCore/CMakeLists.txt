 cmake_minimum_required(VERSION 3.11)
 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")
 #SET(CMAKE_STATIC_LIBRARY_SUFFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 # STATIC FOR LINUX
 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES}
         Utils/SGSL/ShaderAnalyzedFile.cpp)

 include(GenerateExportHeader)
 #generate_export_header(${PROJECT_NAME})

 #[[GENERATE_EXPORT_HEADER(${PROJECT_NAME}           
    BASE_NAME ${PROJECT_NAME}
    EXPORT_MACRO_NAME SGCORE_EXPORTS
    EXPORT_FILE_NAME ${SGCORE_SOURCES}
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)]]

 target_include_directories(${PROJECT_NAME} PUBLIC
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/GLM
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         ${CMAKE_SOURCE_DIR}/Externals/bullet3/src
         ${Vulkan_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/Sources
         ${CMAKE_SOURCE_DIR}/Externals/fmt/include
         ${CMAKE_SOURCE_DIR}/Externals/spdlog/include

         #${BULLET_INCLUDE_DIR}

         #${CMAKE_SOURCE_DIR}/Externals/Vulkan/Include
         #"C:/VulkanSDK/1.3.250.1/Include"
         )

 # TODO: make for linux and mЫac
 target_link_libraries(${PROJECT_NAME} PUBLIC
         glfw
         glm::glm
         gl46
         #${Vulkan_LIBRARIES}
         assimp

         gl46
         #LinearMath
         #Bullet3Common
         #BulletDynamics
         #BulletCollision
         #BulletInverseDynamics
         #BulletSoftBody

         ImGui

         EnTT::EnTT
         SGUtils
         )

 #target_link_libraries(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++)

 IF(WIN32)
     target_link_libraries(${PROJECT_NAME} PUBLIC
             wsock32
             ws2_32)
 ENDIF()

#[[ target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Externals/Vulkan/Lib)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} vulkan-1)
else()
  target_link_libraries(${PROJECT_NAME} vulkan)
endif()]]

 # TODO: DO THIS ONLY WHEN CORE BUILD TYPE IS DYNAMIC LIBRARY
 IF(EXISTS ${SG_CORE_DLL})
 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
         ${SG_CORE_DLL}
         ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ}
         )

 IF(${SG_BUILD_TESTS})
     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E copy
             ${SG_CORE_DLL}
             ${CMAKE_BINARY_DIR}/${SG_TESTS_PROJ}/${SG_PHYS_TEST_PROJ}
             )
 ENDIF()
 ENDIF()
