//
// Created by ilya on 27.03.24.
//

#include <imgui.h>
#include <nfd.h>

#include "TopToolbarView.h"

SGE::TopToolbarView::TopToolbarView()
{
    m_fileCreateDialog->setActive(false);
    m_projectCreateDialog->setActive(false);
    
    addChild(m_fileCreateDialog);
    addChild(m_projectCreateDialog);
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
                m_fileCreateDialog->m_mode = FileOpenMode::CREATE;
                m_fileCreateDialog->setActive(true);
            }
            
            if(ImGui::MenuItem("Project"))
            {
                m_projectCreateDialog->m_mode = FileOpenMode::CREATE;
                m_projectCreateDialog->setActive(true);
            }
            
            ImGui::EndMenu();
        }
        
        if(ImGui::BeginMenu("Open..."))
        {
            if(ImGui::MenuItem("File"))
            {
                m_fileCreateDialog->m_mode = FileOpenMode::OPEN;
                m_fileCreateDialog->setActive(true);
            }
            
            if(ImGui::MenuItem("Project"))
            {
                m_projectCreateDialog->m_mode = FileOpenMode::OPEN;
                m_projectCreateDialog->setActive(true);
            }
            
            ImGui::EndMenu();
        }

        ImGui::Separator();
        
        if(ImGui::MenuItem("Engine Settings"))
        {
        
        }
        
        ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
}

void SGE::TopToolbarView::end()
{

}