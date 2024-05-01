//
// Created by ilya on 30.04.24.
//

#include <imgui.h>
#include <imgui_internal.h>

#include "Explorer.h"
#include "SungearEngineEditor.h"

void SGE::Explorer::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoDockingOverOther;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("Explorer");
    
    auto childFrameID = ImGui::GetID("ExplorerDockSpace");
    ImGui::DockSpace(childFrameID);

    ImGui::End();
    
    ImGui::PopStyleVar(2);
}
