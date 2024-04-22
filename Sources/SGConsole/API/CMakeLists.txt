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