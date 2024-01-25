project(Externals)

#[[set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)]]

# -------------------------------

#add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

# -------------------------------

# use jni -----------------------
# future java bindings
#[[find_package(JNI REQUIRED)
find_package(Java REQUIRED)
include(UseJava)

if (JNI_FOUND)
    message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif ()]]
# -------------------------------

# local externals ---------------
add_subdirectory(Assimp)
add_subdirectory(GLFW)
add_subdirectory(OpenGL)
add_subdirectory(GLM)
add_subdirectory(imgui)
#add_subdirectory(fmt)

add_subdirectory(bullet3)

add_subdirectory(spdlog)

add_subdirectory(sparse_map)

SET(BULLET_LINEAR_MATH_DLL ${CMAKE_BINARY_DIR}/lib/libLinearMathd.dll)
SET(BULLET_COLLISION_DLL ${CMAKE_BINARY_DIR}/lib/libBulletCollisiond.dll)
SET(BULLET3_COMMON_DLL ${CMAKE_BINARY_DIR}/lib/libBullet3Commond.dll)
SET(BULLET_DYNAMICS_DLL ${CMAKE_BINARY_DIR}/lib/libBulletDynamicsd.dll)
SET(BULLET_GEOMETRY_DLL ${CMAKE_BINARY_DIR}/lib/libBulletGeometryd.dll)
SET(BULLET_INVERSE_DYNAMICS_DLL ${CMAKE_BINARY_DIR}/lib/libBulletInverseDynamicsd.dll)
SET(BULLET_SOFT_BODY_DLL ${CMAKE_BINARY_DIR}/lib/libBulletSoftBodyd.dll)

#[[SET(EXTERNALS_DLLS_PROJ ExternalsDlls)

add_custom_target(${EXTERNALS_DLLS_PROJ})

message(${SG_EDITOR_BIN})

add_custom_command(TARGET ${EXTERNALS_DLLS_PROJ} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll
        ${SG_EDITOR_BIN}
)

add_custom_command(TARGET ${EXTERNALS_DLLS_PROJ} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Externals/Assimp/bin/libassimp-5d.dll
        ${SG_PHYS_TEST_BIN}
)]]

#[[IF(EXISTS ${BULLET_LINEAR_MATH_DLL})
    file(COPY ${BULLET_LINEAR_MATH_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET_LINEAR_MATH_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()

IF(EXISTS ${BULLET_COLLISION_DLL})
    file(COPY ${BULLET_COLLISION_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET_COLLISION_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()

IF(EXISTS ${BULLET3_COMMON_DLL})
    file(COPY ${BULLET3_COMMON_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET3_COMMON_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()

IF(EXISTS ${BULLET_DYNAMICS_DLL})
    file(COPY ${BULLET_DYNAMICS_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET_DYNAMICS_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()

IF(EXISTS ${BULLET_GEOMETRY_DLL})
    file(COPY ${BULLET_GEOMETRY_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET_GEOMETRY_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()

IF(EXISTS ${BULLET_INVERSE_DYNAMICS_DLL})
    file(COPY ${BULLET_INVERSE_DYNAMICS_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET_INVERSE_DYNAMICS_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()

IF(EXISTS ${BULLET_SOFT_BODY_DLL})
    file(COPY ${BULLET_SOFT_BODY_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${BULLET_SOFT_BODY_DLL} DESTINATION ${SG_EDITOR_BIN})
ENDIF()]]
# -------------------------------