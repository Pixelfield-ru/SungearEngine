if(${SG_BUILD_PRESET} STREQUAL "release-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/${pluginName}$/cmake-build-release/${pluginName}$.${SG_DL_EXT}")
elseif(${SG_BUILD_PRESET} STREQUAL "debug-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/${pluginName}$/cmake-build-debug/${pluginName}$.${SG_DL_EXT}")
endif()

if(${${pluginName}$_INCLUDE_EXTERNALS})
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/${pluginName}$/Externals")
endif()