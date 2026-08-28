include(../../cmake/utils.cmake)

project(SGRHITest)

set(SGPROJECT_SOURCES Main.cpp)

add_executable(${PROJECT_NAME} ${SGPROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        SGCore
)

copy_sgcore_dlls(${PROJECT_NAME})
