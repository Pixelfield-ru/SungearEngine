cmake_minimum_required(VERSION 3.25)
project(SungearEngineEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
if(${CMAKE_COMPILER_IS_GNUCXX})
	list(APPEND CMAKE_CXX_FLAGS "-g -rdynamic -fno-pie -no-pie -fno-gnu-unique -fexceptions -fnon-call-exceptions")
	message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
elseif(MSVC)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /Od")

		set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} /Od")
	endif()

	if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
		set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} /O2")

		set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_RELEASE} /O2")
	endif()

	foreach(CompilerFlag ${CXX_COMPILER_FLAGS})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()

	foreach(CompilerFlag ${C_COMPILER_FLAGS})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()

	message("cxx compiler flags: ${CXX_COMPILER_FLAGS}, C compiler flags: ${C_COMPILER_FLAGS}")

	set(CMAKE_CXX_FLAGS ${CXX_COMPILER_FLAGS})
	set(CMAKE_C_FLAGS ${C_COMPILER_FLAGS})

	set(MSVC_RUNTIME "dynamic")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
set(SG_BUILD_TYPE "Release")

add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
add_definitions(-DNOMINMAX)

#[[set(SG_INCLUDE_BULLET ON)
set(SG_INCLUDE_PUGIXML OFF)
set(SG_INCLUDE_FREETYPE OFF)
set(SG_INCLUDE_GLM ON)
set(SG_INCLUDE_ASSIMP OFF)
set(SG_INCLUDE_ENTT ON)
set(SG_INCLUDE_GLFW ON)
set(SG_INCLUDE_IMGUI ON)
set(SG_INCLUDE_SPDLOG ON)
set(SG_INCLUDE_STB ON)
set(SG_INCLUDE_GLAD ON)
set(SG_INCLUDE_OPENAL ON)
set(SG_INCLUDE_GLAZE ON)
set(SG_INCLUDE_LUNASVG ON)
set(SG_INCLUDE_RAPIDJSON ON)]]

find_package(unofficial-nativefiledialog CONFIG REQUIRED)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

file(GLOB_RECURSE SGE_SOURCES Sources/*.cpp Sources/*.h)

add_library(${PROJECT_NAME} SHARED ${SGE_SOURCES})
#add_executable(${PROJECT_NAME} ${SGE_SOURCES})

message("sun: " ${SungearEngine_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE Sources ${SungearEngine_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS} unofficial::nativefiledialog::nfd)
