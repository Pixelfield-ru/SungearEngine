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

add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
add_definitions(-DNOMINMAX)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

file(GLOB_RECURSE SGE_SOURCES Sources/*.cpp Sources/*.h)
list(FILTER SGE_SOURCES EXCLUDE REGEX ".templates")

add_library(${PROJECT_NAME} SHARED ${SGE_SOURCES} Externals/ImGuizmo/ImGuizmo.h Externals/ImGuizmo/ImGuizmo.cpp)
# add_executable(${PROJECT_NAME} ${SGE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE Sources ${SungearEngine_INCLUDE_DIRS} Externals)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS} unofficial::nativefiledialog::nfd)
