 cmake_minimum_required(VERSION 3.25)
 project(NativeCore)

 set(CMAKE_CXX_STANDARD 23)

 set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR})

 set(BUILD_SHARED_LIBS ON)

 # jni --------------------------
 find_package(JNI REQUIRED)
 find_package(Java REQUIRED)
 include(UseJava)

 if (JNI_FOUND)
     message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
     message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
 endif ()
 # -------------------------------

 set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
 set(CMAKE_BUILD_TYPE "Debug")

 set(glm_DIR libs/glm/cmake/glm)
 find_package(glm REQUIRED)

 set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
 set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
 set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

 message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

 file(GLOB_RECURSE sources "Sources/*.cpp")

 message(STATUS ${sources})

 include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})

 add_subdirectory(libs/glfw)
 add_subdirectory(libs/graphics/ogl/v46)
 add_subdirectory(libs/assimp)

 add_library(NativeCore SHARED ${sources})
 target_link_libraries(NativeCore glfw)
 target_link_libraries(NativeCore glm::glm)
 target_link_libraries(NativeCore gl46)
 target_link_libraries(NativeCore assimp)

 add_executable(NativeCoreTest ${sources})
 target_link_libraries(NativeCoreTest glfw)
 target_link_libraries(NativeCoreTest glm::glm)
 target_link_libraries(NativeCoreTest gl46)
 target_link_libraries(NativeCoreTest assimp)
