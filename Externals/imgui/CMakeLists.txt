project(ImGui)

set(IMGUI_SOURCES
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_glfw.cpp

        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
)

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCES})

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        ../imgui
        ../imgui/backends
        ../GLFW/include
        ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
