 project(${SG_CONSOLE_API_PROJ})

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCONSOLEAPI_SOURCES ../API/*.cpp ../API/*.h)

 add_library(${PROJECT_NAME} SHARED ${SGCONSOLEAPI_SOURCES})

 #add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Sources
         ${CMAKE_SOURCE_DIR}/Externals
         ${CMAKE_SOURCE_DIR}/Externals/Assimp/include
         ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
         ${CMAKE_SOURCE_DIR}/Externals/GLFW/include
         ${CMAKE_SOURCE_DIR}/Externals/stb
         )

 target_link_libraries(${PROJECT_NAME}
         ${SG_CORE_PROJ}
         )

 # todo: make by file
 IF(EXISTS ${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}d.dll)
   file(COPY ${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}d.dll DESTINATION ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ})
 ENDIF ()