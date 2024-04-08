 cmake_minimum_required(VERSION 3.11)
 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")
 #SET(CMAKE_STATIC_LIBRARY_SUFFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)

 #add_subdirectory(${CMAKE_SOURCE_DIR} Externals/Assimp)

 find_package(Bullet REQUIRED)
 find_package(pugixml CONFIG REQUIRED)
 find_package(glm CONFIG REQUIRED)
 find_package(assimp CONFIG REQUIRED)
 find_package(Freetype REQUIRED)
 find_package(EnTT CONFIG REQUIRED)
 find_package(glfw3 CONFIG REQUIRED)
 find_package(imgui CONFIG REQUIRED)
 find_package(spdlog CONFIG REQUIRED)
 find_package(Stb REQUIRED)
 find_package(glad CONFIG REQUIRED)
 find_package(PNG REQUIRED)
 find_package(BZip2 REQUIRED)
 find_package(unofficial-brotli CONFIG REQUIRED)

 find_path(PUGIXML_INCLUDE_DIR pugixml.hpp)
 message("PUGIXML_INCLUDE_DIR: " ${PUGIXML_INCLUDE_DIR})

 # STATIC FOR LINUX
 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES})

 include(GenerateExportHeader)
 generate_export_header(${PROJECT_NAME})

 message("FREETYPE_FOUND: " ${FREETYPE_FOUND})
 message("FREETYPE_INCLUDE_DIRS: " ${FREETYPE_INCLUDE_DIRS})
 message("FREETYPE_LIBRARIES: " ${FREETYPE_LIBRARIES})

 target_include_directories(${PROJECT_NAME} PUBLIC
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Externals>
         $<BUILD_INTERFACE:${Vulkan_INCLUDE_DIR}>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sources>
         $<BUILD_INTERFACE:${BULLET_INCLUDE_DIR}>
         $<BUILD_INTERFACE:${PUGIXML_INCLUDE_DIR}>
         $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIRS}>
         $<BUILD_INTERFACE:${Stb_INCLUDE_DIR}>

         #${CMAKE_SOURCE_DIR}/Externals/Vulkan/Include
         #"C:/VulkanSDK/1.3.250.1/Include"
         )

 if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
  set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY_DEBUG} ${BULLET_DYNAMICS_LIBRARY_DEBUG} ${BULLET_MATH_LIBRARY_DEBUG})
 elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
  set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_MATH_LIBRARY})
 endif()

 # TODO: make for linux and mac
 target_link_libraries(${PROJECT_NAME} PUBLIC
         PNG::PNG
         BZip2::BZip2
         unofficial::brotli::brotlidec
         unofficial::brotli::brotlienc
         glm::glm
         assimp::assimp
         glfw
         imgui::imgui
         pugixml
         EnTT::EnTT
         spdlog::spdlog
         glad::glad
         SGUtils
         ${SG_BULLET_LIBS}
         ${FREETYPE_LIBRARIES}
         )

 if(${CMAKE_COMPILER_IS_GNUCXX})
  target_link_libraries(${PROJECT_NAME} PUBLIC
          ${CMAKE_DL_LIBS}
          backtrace)
 endif()

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
