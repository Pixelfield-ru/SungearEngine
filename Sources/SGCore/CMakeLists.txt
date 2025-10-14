 include(../../cmake/utils.cmake)

 cmake_minimum_required(VERSION 3.11)
 project(SGCore)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCORE_SOURCES ../SGCore/*.cpp ../SGCore/*.h)
 list(FILTER SGCORE_SOURCES EXCLUDE REGEX ".templates")

 include(FetchContent)

 FetchContent_Declare(
         glaze
         GIT_REPOSITORY https://github.com/stephenberry/glaze.git
         GIT_TAG main
         GIT_SHALLOW TRUE
 )

 FetchContent_MakeAvailable(glaze)

 # antlr4 for css ==================================================

 set(ANTLR4_CSS_GRAMMAR
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Externals/antlr4/css/css3Lexer.g4>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Externals/antlr4/css/css3Parser.g4>)

 set(ANTLR4_CSS_GENERATED_OUTPUT_DIR $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sources/SGCore/UI/ANTLR4CSS3Generated>)

 set(ANTLR4_JAR $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Externals/antlr4/antlr-4.13.1-complete.jar>)

 add_custom_command(
         OUTPUT
         ${ANTLR4_CSS_GENERATED_OUTPUT_DIR}/css3Parser.cpp
         COMMAND java -jar ${ANTLR4_JAR} -Dlanguage=Cpp -o ${ANTLR4_CSS_GENERATED_OUTPUT_DIR} -visitor ${ANTLR4_CSS_GRAMMAR}
         DEPENDS ${ANTLR4_CSS_GRAMMAR}
         COMMENT "Generating ANTLR4 parser and lexer"
         VERBATIM
 )

 #[[add_custom_target(ANTLR4CSS
         COMMAND java -jar ${ANTLR4_JAR} -Dlanguage=Cpp -o ${ANTLR4_CSS_GENERATED_OUTPUT_DIR} -visitor ${ANTLR4_CSS_GRAMMAR}
         SOURCES ${ANTLR4_CSS_GRAMMAR}
         DEPENDS ${ANTLR4_CSS_GRAMMAR}
         COMMENT "Generating ANTLR4 code from ${ANTLR4_CSS_GRAMMAR}"
         VERBATIM
 )]]

 add_custom_target(ANTLR4CSS
         DEPENDS
         ${ANTLR4_CSS_GENERATED_OUTPUT_DIR}/css3Parser.cpp
 )

 add_dependencies(ANTLR4CSS SGCore)

 # =================================================================

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
         ${ANTLR4_INCLUDE_DIR}
         )

 message("CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

 message("Bullet Include Dir: " ${BULLET_INCLUDE_DIRS})
 message("Bullet Libraries: " ${SG_BULLET_LIBS})

 target_link_libraries(${PROJECT_NAME} PUBLIC
         PNG::PNG
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
         lunasvg::lunasvg
         rapidjson
         msdf-atlas-gen
         gli
         meshoptimizer::meshoptimizer
         rectpack2D::rectpack2D
         Boost::system
         Boost::asio

         ${SG_BULLET_LIBS}
         ${FREETYPE_LIBRARIES}
 )

 if(UNIX OR APPLE)
  find_library(Backtrace_LIBRARY backtrace)

  target_link_libraries(${PROJECT_NAME} PUBLIC
          ${CMAKE_DL_LIBS}
          ${Backtrace_LIBRARY}
          tbb
          antlr4_static)
 else()
  target_link_libraries(${PROJECT_NAME} PUBLIC antlr4_static)
 endif()

 #target_link_libraries(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++)

 #[[IF(WIN32)
     target_link_libraries(${PROJECT_NAME} PRIVATE
             wsock32
             ws2_32)
 ENDIF()]]

#[[ target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Externals/Vulkan/Lib)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} vulkan-1)
else()
  target_link_libraries(${PROJECT_NAME} vulkan)
endif()]]
