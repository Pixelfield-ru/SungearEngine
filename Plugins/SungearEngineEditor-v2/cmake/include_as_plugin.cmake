if(${SG_BUILD_PRESET} STREQUAL "release-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor-v2/cmake-build-release/SungearEngineEditor-v2.${SG_DL_EXT}")
elseif(${SG_BUILD_PRESET} STREQUAL "debug-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor-v2/cmake-build-debug/SungearEngineEditor-v2.${SG_DL_EXT}")
endif()

if(${SungearEngineEditor-v2_INCLUDE_EXTERNALS})
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor-v2/Externals")
endif()