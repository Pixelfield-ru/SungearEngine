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

    if(m_isPopupWindow)
    {
        ImGui::OpenPopup(m_name.getName().c_str());
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

    m_size = ImGui::GetWindowSize();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 8));
    auto wndContentRegionAvail = ImGui::GetContentRegionAvail();
    ImGui::BeginChild(ImGui::GetID((m_name.getName() + "DialogName").c_str()), ImVec2(ImGui::GetContentRegionAvail().x, 0),
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
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 8));
    ImGui::BeginChild(ImGui::GetID((m_name.getName() + "_Body").c_str()),
                      ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - m_footerSize.y),
                      ImGuiChildFlags_AlwaysUseWindowPadding,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                      ImGuiWindowFlags_NoSavedSettings);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 8));

    if (m_enableDocking)
    {
        m_dockspaceID = ImGui::GetID((m_name.getName() + "_BodyDockspace").c_str());

        ImGui::DockSpace(m_dockspaceID, ImVec2(0.0f, 0.0f), m_dockspaceFlags);
    }

    return true;
}

void SGE::Window::end()
{
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleVar();

    postRenderBody();

    ImGui::Separator();

    auto footerPadding = ImVec2(7, 8);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, footerPadding);

    ImGui::BeginChild(ImGui::GetID((m_name.getName() + "_Footer").c_str()), ImVec2(0, 0),
                      ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                      ImGuiWindowFlags_NoSavedSettings);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 8));

    footerRender();

    m_footerSize = ImGui::GetWindowContentRegionMax();
    m_footerSize.x -= ImGui::GetWindowContentRegionMin().x;
    m_footerSize.y -= ImGui::GetWindowContentRegionMin().y;

    m_footerSize.x += ImGui::GetScrollMaxX();
    m_footerSize.y += ImGui::GetScrollMaxY();

    m_footerSize.x += footerPadding.x * 2;
    m_footerSize.y += footerPadding.y * 2;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleVar();

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
