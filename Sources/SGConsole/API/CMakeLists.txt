 project(SGConsoleAPI)

 SET(CMAKE_SHARED_LIBRARY_PREFIX "")

 file(GLOB_RECURSE SGCONSOLEAPI_SOURCES ../API/*.cpp ../API/*.h)

 add_library(${PROJECT_NAME} SHARED ${SGCONSOLEAPI_SOURCES})

 #add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
 target_include_directories(${PROJECT_NAME} PRIVATE
         ${CMAKE_SOURCE_DIR}/Sources
         )

 target_link_libraries(${PROJECT_NAME}
         SGCore
         )

 # todo: make by file
 IF(EXISTS ${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}d.dll)
   file(COPY ${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}d.dll DESTINATION ${CMAKE_BINARY_DIR}/${SG_SOURCES_PROJ}/${SG_EDITOR_PROJ})
 ENDIF ()