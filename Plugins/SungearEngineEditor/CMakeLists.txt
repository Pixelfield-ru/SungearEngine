if(WIN32)
	set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")
endif()

cmake_minimum_required(VERSION 3.25)
project(SungearEngineEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-implicit-fallthrough -Wno-ignored-qualifiers -g -rdynamic -fno-pie -no-pie -Wno-error=deprecated-copy")

	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		add_definitions(-DSTBI_NO_SIMD)
		add_definitions(-DSTBIR_NO_SIMD)

		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	if(MSVC)
		add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
		add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
		add_compile_options("/bigobj")
	endif()

	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		if(MSVC)
			set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /Od")
			set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} /Od")
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
			set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
		endif ()
	endif()

	if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
		if(MSVC)
			set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /O2")
			set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} /O2")
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			set(CXX_COMPILER_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -O2")
			set(C_COMPILER_FLAGS "${CMAKE_C_FLAGS_DEBUG} -O2")
		endif ()
	endif()

	message("cxx compiler flags: ${CXX_COMPILER_FLAGS}, C compiler flags: ${C_COMPILER_FLAGS}")

	set(CMAKE_CXX_FLAGS ${CXX_COMPILER_FLAGS})
	set(CMAKE_C_FLAGS ${C_COMPILER_FLAGS})

	if(MSVC)
		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
		elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
			set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
		endif()

		set(MSVC_RUNTIME "static")
	endif()

	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(BUILD_SHARED_LIBS OFF)
endif ()

if(POSIX OR APPLE)
	add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)
	add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
elseif(MSVC)
	add_definitions(-DBOOST_STACKTRACE_USE_WINDBG)
endif ()
add_definitions(-DNOMINMAX)

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

message("CMAKE_MODULE_PATH=${CMAKE_PREFIX_PATH}")
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

file(GLOB_RECURSE SGE_SOURCES Sources/*.cpp Sources/*.h)
list(FILTER SGE_SOURCES EXCLUDE REGEX ".templates")

message("Bullet libs: ${SG_BULLET_LIBS}")

add_library(${PROJECT_NAME} SHARED ${SGE_SOURCES} Externals/ImGuizmo/ImGuizmo.h Externals/ImGuizmo/ImGuizmo.cpp)
# add_executable(${PROJECT_NAME} ${SGE_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC Sources ${SungearEngine_INCLUDE_DIRS} Externals)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS} unofficial::nativefiledialog::nfd)
