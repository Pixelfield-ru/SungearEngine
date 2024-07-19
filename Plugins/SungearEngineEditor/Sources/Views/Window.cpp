//
// Created by Ilya on 18.07.2024.
//

#include <SGCore/Memory/Assets/SVGImage.h>
#include "Window.h"
#include "ImGuiUtils.h"
#include "Resources.h"
#include "Styles/StylesManager.h"

bool SGE::Window::begin()
{
    if(!isActive()) return false;

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();

    ImGui::SetNextWindowPos(ImVec2(center.x - m_size.x / 2.0f, center.y - m_size.y / 2.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, m_minSize);

    if(m_isPopupWindow && isAppearing())
    {
        ImGui::OpenPopup(m_name.getName().c_str());

        std::printf("dfsdfsdfsdfsdf\n");
    }

    if(m_isPopupWindow)
    {
        ImGui::BeginPopupModal(m_name.getName().c_str(), nullptr,
                               ImGuiWindowFlags_NoCollapse |
                               ImGuiWindowFlags_NoTitleBar |
                               ImGuiWindowFlags_NoSavedSettings);
    }
    else
    {
        ImGui::Begin(m_name.getName().c_str(), nullptr,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoSavedSettings);
    }

    // ImGui::PopStyleVar();

    m_size = ImGui::GetWindowSize();

    /*if(m_bodyRegionMax.x > m_size.x)
    {
        ImGui::SetWindowSize({ m_bodyRegionMax.x, ImGui::GetWindowSize().y });
    }
    if(m_bodyRegionMax.y + m_headerRegionMax.y > m_size.y)
    {
        ImGui::SetWindowSize({ ImGui::GetWindowSize().x, m_bodyRegionMax.y + m_headerRegionMax.y });
    }*/

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 8));
    auto wndContentRegionAvail = ImGui::GetContentRegionAvail();
    ImGui::BeginChild(ImGui::GetID("DialogName"), ImVec2(ImGui::GetContentRegionAvail().x, 0),
                      ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 8));

    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text(m_name.getName().c_str());
    }

    {
        ImGui::SameLine();

        auto crossIcon = StylesManager::getCurrentStyle()->m_crossIcon
                ->getSpecialization(26, 26)
                ->getTexture();

        ImGui::SetCursorPosX(wndContentRegionAvail.x - 33);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
        if (ImGuiUtils::ImageButton(crossIcon->getTextureNativeHandler(),
                                    ImVec2(crossIcon->getWidth() + 6, crossIcon->getHeight() + 6),
                                    ImVec2(crossIcon->getWidth(), crossIcon->getHeight()),
                                    0,
                                    false,
                                    ImVec2(-1, -1),
                                    ImVec4(1.0, 0.0, 0.0, 1.0)).m_isLMBClicked)
        {
            setActive(false);
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
    }

    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().WindowPadding.x - 2);
        ImGui::Separator();
    }

    ImGui::PopStyleVar();

    ImGui::EndChild();

    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 8));

    ImGui::BeginChild(ImGui::GetID((m_name.getName() + "_Body").c_str()), ImVec2(0, 0),
                      ImGuiChildFlags_AlwaysUseWindowPadding,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                      ImGuiWindowFlags_NoSavedSettings);

    /*ImGui::BeginChild(ImGui::GetID((m_name.getName() + "_Body").c_str()), ImVec2(0, 0),
                      ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);*/

    if(m_enableDocking)
    {
        m_dockspaceID = ImGui::GetID((m_name.getName() + "_BodyDockspace").c_str());

        ImGui::DockSpace(m_dockspaceID, ImVec2(0.0f, 0.0f), m_dockspaceFlags);
    }

    return true;
}

void SGE::Window::end()
{
    m_bodyRegionMax = ImGui::GetWindowContentRegionMax();
    m_bodyRegionMax.x += ImGui::GetScrollMaxX();
    m_bodyRegionMax.y += ImGui::GetScrollMaxY();
    m_bodyRegionMax.x += 7;
    m_bodyRegionMax.y += 8;

    ImGui::EndChild();

    postRenderBody();

    /*if(m_lastBodyRegionMax.x > m_bodyRegionMax.x)
    {
        ImGui::SetWindowSize({ m_bodyRegionMax.x, ImGui::GetWindowSize().y });
        m_size.x = m_bodyRegionMax.x;
    }
    if(m_lastBodyRegionMax.y > m_bodyRegionMax.y)
    {
        ImGui::SetWindowSize({ ImGui::GetWindowSize().x, m_bodyRegionMax.y + m_headerRegionMax.y });
        m_size.y = m_bodyRegionMax.y + m_headerRegionMax.y;
    }*/

    ImGui::PopStyleVar(2);

    if(m_isPopupWindow)
    {
        ImGui::EndPopup();
    }
    else
    {
        ImGui::End();
    }

    ImGui::PopStyleVar(3);
}

void SGE::Window::onActiveChangedListener()
{
}
