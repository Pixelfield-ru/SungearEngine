//
// Created by ilya on 30.04.24.
//

#include <imgui.h>
#include <imgui_internal.h>

#include "DirectoryExplorer.h"
#include "Views/MainView.h"
#include "DirectoriesTreeExplorer.h"

void SGE::DirectoryExplorer::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("DirectoryExplorer");
    // =================================================
    
    if(std::filesystem::exists(m_currentPath) && std::filesystem::is_directory(m_currentPath))
    {
        ImGui::Text(m_currentPath.c_str());
        
        for(auto it = std::filesystem::directory_iterator(m_currentPath);
            it != std::filesystem::directory_iterator(); ++it)
        {
            const auto& curFile = *it;
            
            bool isDirectory = std::filesystem::is_directory(curFile);
            
            
        }
    }
    
    // =================================================
    ImGui::End();
    
    ImGui::PopStyleVar();
}
