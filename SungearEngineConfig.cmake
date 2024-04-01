# SungearEngineConfig.cmake.in

####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was SungearEngineConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

find_dependency(Bullet)
find_dependency(pugixml CONFIG)
find_dependency(glm CONFIG)
find_dependency(assimp CONFIG)
find_dependency(Freetype)
find_dependency(EnTT CONFIG)
find_dependency(glfw3 CONFIG)
find_dependency(imgui CONFIG)
find_dependency(spdlog CONFIG)
find_dependency(Stb MODULE)
find_dependency(glad CONFIG)
find_dependency(PNG)
find_dependency(BZip2)
find_dependency(unofficial-brotli CONFIG)

# Here we could use `` as reference
# to variable 'INCLUDE_DIR' set in the CMakeLists.txt.
set_and_check(SungearEngine_INCLUDE_DIR "${PACKAGE_PREFIX_DIR}/include/SungearEngine")
include("${CMAKE_CURRENT_LIST_DIR}/SungearEngineTargets.cmake")

get_property(SG_VCPKG_INCLUDE_DIR TARGET SGCore PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
