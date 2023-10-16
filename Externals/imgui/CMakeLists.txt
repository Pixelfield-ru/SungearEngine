project(ImGui)

set(IMGUI_DLL ${CMAKE_BINARY_DIR}/lib/lib${PROJECT_NAME}d.dll)

set(IMGUI_SOURCES
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        )

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ../imgui ../imgui/backends)

if(EXISTS ${IMGUI_DLL})
    file(COPY ${IMGUI_DLL} DESTINATION ${SG_PHYS_TEST_BIN})
    file(COPY ${IMGUI_DLL} DESTINATION ${SG_EDITOR_BIN})
endif()
