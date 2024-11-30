//
// Created by ilya on 27.03.24.
//

#include <imgui.h>
#include <nfd.h>

#include "TopToolbarView.h"
#include "SungearEngineEditor.h"
#include "ImGuiUtils.h"

SGE::TopToolbarView::TopToolbarView()
{
    m_fileCreateDialog->setActive(false);
    m_projectCreateDialog->setActive(false);
    m_engineSettingsView->setActive(false);

    addChild(m_fileCreateDialog);
    addChild(m_projectCreateDialog);
    addChild(m_engineSettingsView);

    m_engineIcon = StylesManager::getCurrentStyle()->m_mingwIcon
            ->getSpecialization(20, 20)
            ->getTexture();

    m_moreIcon = StylesManager::getCurrentStyle()->m_moreIcon
            ->getSpecialization(24, 24)
            ->getTexture();
}

bool SGE::TopToolbarView::begin()
{
    return true;
}

void SGE::TopToolbarView::renderBody()
{
    auto mainView = SungearEngineEditor::getInstance()->getMainView();

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther |
            ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.0f, 10.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 15.0f, 0 });
    ImGui::Begin(m_name.getName().c_str(), nullptr,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoResize);

    // drawing engine icon
    ImGui::Image(m_engineIcon->getTextureNativeHandler(), { (float) m_engineIcon->getWidth(), (float) m_engineIcon->getHeight() });

    ImGui::SameLine();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
    // drawing icon that will open bars
    ImGuiUtils::ImageButton(m_moreIcon->getTextureNativeHandler(),
                            { 27.0f, 27.0f },
                            { 24.0f, 24.0f });

    ImGui::SameLine();

    if(ImGui::Button("File"))
    {
        m_popup.setOpened(true);

        /*if(ImGui::BeginMenu("New..."))
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

        ImGui::EndMenu();*/
    }

    m_popup.draw();

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void SGE::TopToolbarView::end()
{
    IView::end();
}