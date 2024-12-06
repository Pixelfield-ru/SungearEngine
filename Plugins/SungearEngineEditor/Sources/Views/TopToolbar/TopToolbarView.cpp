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
    m_fileButtonPopup = Popup {
            "ToolbarFileActions",
            {
                    {
                            .m_text = "New...",
                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                       18)->getTexture(),
                            .m_elements = {
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Project",
                                            .m_ID = "New/Project",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "File",
                                            .m_ID = "New/File",
                                            .m_icon = StylesManager::getCurrentStyle()->m_linesIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                            }
                    },
                    {
                            .m_text = "Open...",
                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                       18)->getTexture(),
                            .m_drawSeparatorAfter = true,
                            .m_elements = {
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "Project",
                                            .m_ID = "Open/Project",
                                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                                    SGCore::MakeRef<PopupElement>(PopupElement {
                                            .m_text = "File",
                                            .m_ID = "Open/File",
                                            .m_icon = StylesManager::getCurrentStyle()->m_linesIcon->getSpecialization(18,
                                                                                                                       18)->getTexture()
                                    }),
                            }
                    },
                    {
                            .m_text = "Engine Settings",
                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                       18)->getTexture()
                    }
            }
    };

    m_helpButtonPopup = Popup {
            "ToolbarHelpActions",
            {
                    {
                            .m_text = "About",
                            .m_icon = StylesManager::getCurrentStyle()->m_dummyIcon->getSpecialization(18,
                                                                                                       18)->getTexture()
                    }
            }
    };

    m_fileButtonPopup.onElementClicked += [this](const SGCore::Ref<PopupElement>& element) {
        if(element->m_ID == "Open/Project")
        {
            m_projectCreateDialog->m_mode = FileOpenMode::OPEN;
            m_projectCreateDialog->setActive(true);
        }
        else if(element->m_ID == "Open/File")
        {
            m_fileCreateDialog->m_mode = FileOpenMode::OPEN;
            m_fileCreateDialog->m_name = "Open File";
            m_fileCreateDialog->m_disallowPathSpecifying = false;
            m_fileCreateDialog->setActive(true);
        }
        else if(element->m_ID == "New/Project")
        {
            m_projectCreateDialog->m_mode = FileOpenMode::CREATE;
            m_projectCreateDialog->setActive(true);
        }
        else if(element->m_ID == "New/File")
        {
            m_fileCreateDialog->m_mode = FileOpenMode::CREATE;
            m_fileCreateDialog->m_name = "New File";
            m_fileCreateDialog->m_disallowPathSpecifying = false;
            m_fileCreateDialog->setActive(true);
        }
        else if(element->m_ID == "Engine Settings")
        {
            m_engineSettingsView->setActive(true);
        }
    };

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

    m_fileButtonPopup.m_isCustomPosition = true;
    m_helpButtonPopup.m_isCustomPosition = true;

    m_fileButtonPopup.m_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
    m_helpButtonPopup.m_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
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

    if(!m_isAnyButtonHovered)
    {
        m_areaToHighlightPos = { 0, 0 };
        m_areaToHighlightSize = { 0, 0 };
    }

    m_isAnyButtonHovered = false;

    ImGui::GetWindowDrawList()->AddRectFilled(m_areaToHighlightPos,
                                              {m_areaToHighlightPos.x + m_areaToHighlightSize.x,
                                               m_areaToHighlightPos.y + m_areaToHighlightSize.y},
                                              ImGui::ColorConvertFloat4ToU32({0.3f, 0.3f, 0.3f, 0.3f}));

    // drawing engine icon
    ImGui::Image(m_engineIcon->getTextureNativeHandler(), { (float) m_engineIcon->getWidth(), (float) m_engineIcon->getHeight() });

    ImGui::SameLine();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
    // drawing icon that will open bars
    if(!m_showToolsButtons && ImGuiUtils::ImageButton(m_moreIcon->getTextureNativeHandler(),
                               {27.0f, 27.0f},
                               {24.0f, 24.0f}
    ).m_isLMBClicked)
    {
        m_showToolsButtons = true;
    }

    ImGui::SameLine();

    if(m_showToolsButtons)
    {
        // pushing style variables for buttons
        ImGui::PushStyleColor(ImGuiCol_Border, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        // ====================

        drawButtonWithPopup("File", m_fileButtonPopup);
        drawButtonWithPopup("Help", m_helpButtonPopup);

        // ====================
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(1);
    }

    if(!ImGui::IsWindowFocused() && !m_isAnyButtonHovered)
    {
        m_showToolsButtons = false;
    }

    // drawing popups
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 0 });
    m_fileButtonPopup.draw();
    m_helpButtonPopup.draw();
    ImGui::PopStyleVar(1);

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void SGE::TopToolbarView::end()
{
    IView::end();
}

void SGE::TopToolbarView::drawButtonWithPopup(const std::string& name, SGE::Popup& buttonPopup) noexcept
{
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    ImVec2 popupPos = ImGui::GetCursorScreenPos();
    popupPos.y += ImGui::GetWindowHeight() - popupPos.y;

    if(buttonPopup.isOpened())
    {
        m_isAnyButtonHovered = true;
        m_areaToHighlightPos = ImGui::GetCursorPos();
    }

    ImGui::Button(name.c_str(), ImVec2(0, ImGui::GetWindowHeight()));
    ImGui::SameLine();

    if(buttonPopup.isOpened())
    {
        m_areaToHighlightSize = ImGui::GetItemRectSize();
    }

    if(ImGui::IsItemHovered())
    {
        buttonPopup.m_customPosition = popupPos;
        buttonPopup.setOpened(true);
    }
}
