if(${SG_BUILD_PRESET} STREQUAL "release-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor/cmake-build-release/SungearEngineEditor.${SG_DL_EXT}")
elseif(${SG_BUILD_PRESET} STREQUAL "debug-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor/cmake-build-debug/SungearEngineEditor.${SG_DL_EXT}")
endif()

if(${SungearEngineEditor_INCLUDE_EXTERNALS})
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/SungearEngineEditor/Externals")
endif()