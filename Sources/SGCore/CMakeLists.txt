 cmake_minimum_required(VERSION 3.11)
 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")
 #SET(CMAKE_STATIC_LIBRARY_SUFFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 #add_subdirectory(${CMAKE_SOURCE_DIR} Externals/Assimp)

 find_package(pugixml CONFIG REQUIRED)

 find_path(PUGIXML_INCLUDE_DIR pugixml.hpp)
 message("PUGIXML_INCLUDE_DIR: " ${PUGIXML_INCLUDE_DIR})

 # STATIC FOR LINUX
 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES})

 include(GenerateExportHeader)
 generate_export_header(${PROJECT_NAME})

 find_package(Freetype REQUIRED)
 message("FREETYPE_FOUND: " ${FREETYPE_FOUND})
 message("FREETYPE_INCLUDE_DIRS: " ${FREETYPE_INCLUDE_DIRS})
 message("FREETYPE_LIBRARIES: " ${FREETYPE_LIBRARIES})

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
         ${BULLET_INCLUDE_DIR}
         ${PUGIXML_INCLUDE_DIR}
         ${FREETYPE_INCLUDE_DIRS}
         ${CMAKE_CURRENT_BINARY_DIR}

         #${CMAKE_SOURCE_DIR}/Externals/Vulkan/Include
         #"C:/VulkanSDK/1.3.250.1/Include"
         )

 # TODO: make for linux and mac
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

         ${BULLET_COLLISION_LIBRARY}
         ${BULLET_DYNAMICS_LIBRARY}
         ${BULLET_MATH_LIBRARY}

         ImGui

         pugixml

         ${FREETYPE_LIBRARIES}

         EnTT::EnTT
         SGUtils
         )

 IF(UNIX)
  target_link_libraries(${PROJECT_NAME} PUBLIC
          ${CMAKE_DL_LIBS}
          backtrace)
 ENDIF()

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

 if(MSVC)
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/SGCore.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/SGCore.dll
            ${CMAKE_BINARY_DIR}/Sources/SGEditor
            )
    ENDIF()
 endif()
