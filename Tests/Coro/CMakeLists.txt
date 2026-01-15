include(../../cmake/utils.cmake)

project(SGCoroTest)

set(SGPROJECT_SOURCES Main.cpp Main.h)

find_package(spdlog CONFIG REQUIRED)
# find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SGPROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
        SGCore
        # GTest::gtest
        # GTest::gtest_main
)

copy_sgcore_dlls(${PROJECT_NAME})