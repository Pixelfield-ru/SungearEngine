//
// Created by ilya on 27.03.24.
//

#include <imgui.h>

#include "TopToolbarView.h"

bool SGE::TopToolbarView::begin()
{
    return true;
}

void SGE::TopToolbarView::renderBody()
{
    ImGui::BeginMainMenuBar();
    
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::BeginMenu("New..."))
        {
            ImGui::EndMenu();
        }
        
        if(ImGui::MenuItem("Open..."))
        {
        
        }

        ImGui::Separator();
        
        if(ImGui::MenuItem("Settings"))
        {
        
        }
        
        ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
}

void SGE::TopToolbarView::end()
{

}
