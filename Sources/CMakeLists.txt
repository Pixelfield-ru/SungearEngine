add_subdirectory(SGCore)
if(${SG_BUILD_ENTRY})
    add_subdirectory(SGEntry)
endif()

#[[
# installing sgcore
install(TARGETS SGCore
        EXPORT SungearEngineTargets
        ARCHIVE DESTINATION lib               # for .lib
        LIBRARY DESTINATION lib               # for .dll
        RUNTIME DESTINATION bin               # for .exe and .dll
        INCLUDES DESTINATION include/SGCore   # for headers
)

# installing dependencies
install(TARGETS msdf-atlas-gen msdfgen-full msdfgen-core msdfgen-ext
        EXPORT SungearEngineTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

# installing headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Sources/SGCore DESTINATION include/SGCore FILES_MATCHING PATTERN "*.h")

# generating config file
configure_package_config_file(
        ${CMAKE_SOURCE_DIR}/cmake/SungearEngineConfig.cmake.in  # template
        ${CMAKE_BINARY_DIR}/SungearEngineConfig.cmake
        INSTALL_DESTINATION lib/cmake/SungearEngine
)

# installing cmake file
install(EXPORT SungearEngineTargets
        FILE SungearEngineTargets.cmake
        NAMESPACE SungearEngine::
        DESTINATION lib/cmake/SungearEngine
)

# installing engine configuration
install(FILES ${CMAKE_BINARY_DIR}/SungearEngineConfig.cmake
        DESTINATION lib/cmake/SungearEngine
)]]
