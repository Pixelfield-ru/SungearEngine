//
// Created by ilya on 27.03.24.
//

#include <imgui.h>
#include <nfd.h>

#include "TopToolbarView.h"
#include "SungearEngineEditor.h"

SGE::TopToolbarView::TopToolbarView()
{
    m_fileCreateDialog->setActive(false);
    m_projectCreateDialog->setActive(false);
    m_engineSettingsView->setActive(false);

    addChild(m_fileCreateDialog);
    addChild(m_projectCreateDialog);
    addChild(m_engineSettingsView);
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
            m_engineSettingsView->setActive(true);
        }
        
        ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
}

void SGE::TopToolbarView::end()
{

}