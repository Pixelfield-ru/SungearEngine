set(SG_DL_EXT ???)

if(MSVC)
    set(SG_DL_EXT lib)
elseif(${CMAKE_COMPILER_IS_GNUCXX})
    set(SG_DL_EXT so)
endif ()

function(copy_sgcore_dlls toTarget)
    #[[file(GLOB_RECURSE SGCORE_DLLS ${CMAKE_BINARY_DIR}/Sources/SGCore/*.dll)

    add_custom_command(
            TARGET ${toTarget}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SGCORE_DLLS} $<TARGET_FILE_DIR:${toTarget}>
    )]]
endfunction()