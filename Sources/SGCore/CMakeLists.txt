 cmake_minimum_required(VERSION 3.11)
 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)
 list(FILTER SGCORE_SOURCES EXCLUDE REGEX ".references")

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
 find_package(OpenAL CONFIG REQUIRED)
 find_package(unofficial-lunasvg CONFIG REQUIRED)
 find_package(RapidJSON CONFIG REQUIRED)

 include(FetchContent)

 FetchContent_Declare(
         glaze
         GIT_REPOSITORY https://github.com/stephenberry/glaze.git
         GIT_TAG main
         GIT_SHALLOW TRUE
 )

 FetchContent_MakeAvailable(glaze)

 add_library(${PROJECT_NAME} SHARED ${SGCORE_SOURCES})

 target_precompile_headers(${PROJECT_NAME} PUBLIC pch.h)

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
         #$<BUILD_INTERFACE:${PUGIXML_INCLUDE_DIR}>
         $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIRS}>
         $<BUILD_INTERFACE:${Stb_INCLUDE_DIR}>
         )

 if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
  if(MSVC)
   set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY_DEBUG} ${BULLET_DYNAMICS_LIBRARY_DEBUG} ${BULLET_MATH_LIBRARY_DEBUG})
  elseif(UNIX)
   set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_MATH_LIBRARY})
  endif()
 elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
  set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_MATH_LIBRARY})
 endif()

 message("CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

 message("Bullet Include Dir: " ${BULLET_INCLUDE_DIRS})
 message("Bullet Libraries: " ${SG_BULLET_LIBS})

 # TODO: make for linux and mac
 target_link_libraries(${PROJECT_NAME} PUBLIC
         PNG::PNG
         BZip2::BZip2
         unofficial::brotli::brotlidec
         glm::glm
         assimp::assimp
         glfw
         imgui::imgui
         pugixml
         EnTT::EnTT
         spdlog::spdlog
         glad::glad
         OpenAL::OpenAL
         glaze::glaze
         unofficial::lunasvg::lunasvg
         rapidjson

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
