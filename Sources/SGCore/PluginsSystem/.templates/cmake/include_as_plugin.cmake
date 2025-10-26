get_filename_component(SG_BINARY_DIR_NAME ${CMAKE_BINARY_DIR} NAME)
list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/${pluginName}$/${SG_BINARY_DIR_NAME}/${pluginName}$.${SG_DL_EXT}")

if(${${pluginName}$_INCLUDE_EXTERNALS})
    list(APPEND SungearEngine_INCLUDE_DIRS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/${pluginName}$/Externals")
endif()