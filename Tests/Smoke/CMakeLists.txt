include(../../cmake/utils.cmake)

project(SGSmokeTest)

set(SGPROJECT_SOURCES Main.cpp SmokeApp.cpp SmokeApp.h ImageCompare.cpp ImageCompare.h RenderDocCapture.cpp RenderDocCapture.h)

find_package(Stb REQUIRED)

add_executable(${PROJECT_NAME} ${SGPROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        SGCore
)

copy_sgcore_dlls(${PROJECT_NAME})
