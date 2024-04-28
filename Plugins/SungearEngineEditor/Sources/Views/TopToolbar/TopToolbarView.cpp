//
// Created by ilya on 27.03.24.
//

#include <imgui.h>
#include <nfd.h>

#include "TopToolbarView.h"

SGE::TopToolbarView::TopToolbarView()
{
    m_fileCreateDialog->setActive(false);
    addChild(m_fileCreateDialog);
}

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
            if(ImGui::MenuItem("File"))
            {
                m_fileCreateDialog->setActive(true);
            }
            
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