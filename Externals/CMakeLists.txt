project(Externals)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# -------------------------------

#add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

# -------------------------------

# use jni -----------------------
# future java bindings
find_package(JNI REQUIRED)
find_package(Java REQUIRED)
include(UseJava)

if (JNI_FOUND)
    message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif ()
# -------------------------------

# local externals ---------------
add_subdirectory(Assimp Assimp)
add_subdirectory(GLFW GLFW)
add_subdirectory(OpenGL OpenGL)
add_subdirectory(GLM glm)
#add_subdirectory(STB stb)
# -------------------------------