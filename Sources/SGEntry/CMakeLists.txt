project(SGEntry)

file(GLOB_RECURSE SGENTRY_SOURCES *.cpp *.h)
list(FILTER SGENTRY_SOURCES EXCLUDE REGEX "CoreTest.cpp")
list(FILTER SGENTRY_SOURCES EXCLUDE REGEX "CoreTest.h")

set(SG_EXEC_NAME SungearEngine)

find_package(spdlog CONFIG REQUIRED)

add_executable(${SG_EXEC_NAME} ${SGENTRY_SOURCES})

target_include_directories(${SG_EXEC_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Sources
        ${CMAKE_SOURCE_DIR}/Externals
        ${CMAKE_SOURCE_DIR}/Externals/OpenGL/include
        )

target_link_libraries(${SG_EXEC_NAME} PRIVATE
        spdlog::spdlog
        SGCore
        )

copy_sgcore_dlls(${SG_EXEC_NAME})