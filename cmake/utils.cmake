set(SG_DL_EXT ???)

if(WIN32)
    set(SG_DL_EXT lib)
elseif(UNIX)
    set(SG_DL_EXT so)
endif ()

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    if(WIN32)
        set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY_DEBUG} ${BULLET_DYNAMICS_LIBRARY_DEBUG} ${BULLET_MATH_LIBRARY_DEBUG})
    elseif(UNIX)
        set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_MATH_LIBRARY})
    endif()
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(SG_BULLET_LIBS ${BULLET_COLLISION_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_MATH_LIBRARY})
endif()

function(copy_sgcore_dlls toTarget)
    #[[file(GLOB_RECURSE SGCORE_DLLS ${CMAKE_BINARY_DIR}/Sources/SGCore/*.dll)

    add_custom_command(
            TARGET ${toTarget}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SGCORE_DLLS} $<TARGET_FILE_DIR:${toTarget}>
    )]]
endfunction()