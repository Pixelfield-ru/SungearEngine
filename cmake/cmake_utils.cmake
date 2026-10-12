include(CMakeParseArguments)

file(
    GLOB _my_cmake_files
    "${CMAKE_CURRENT_LIST_DIR}/files/*.cmake")
foreach(_it IN LISTS _my_cmake_files)
    include("${_it}")
endforeach()
unset(_it)
unset(_my_cmake_files)