message("preset: ${SG_BUILD_PRESET}, host: ${CMAKE_HOST_SYSTEM_NAME}, target: ${CMAKE_SYSTEM_NAME}")

# CHECK IF WE ARE USING NON-HOST PRESET
if(NOT (${SG_BUILD_PRESET} STREQUAL "release-host") AND NOT (${SG_BUILD_PRESET} STREQUAL "debug-host"))
    if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" AND ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        include(cmake/linux-win_x86_64-toolchain.cmake)
    else()
        set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
    endif()
else()
    set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
endif()