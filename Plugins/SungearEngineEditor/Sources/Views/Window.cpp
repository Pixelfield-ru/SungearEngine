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
    // body min size
    auto bodyMinSize = m_bodyMinSize;
    bodyMinSize.y += m_headerSize.y;
    bodyMinSize.y += m_footerSize.y;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, bodyMinSize);

    if(m_isPopupWindow)
    {
        ImGui::OpenPopup(m_name.getName().c_str());
    }

    if(m_lastFooterSize.y < m_footerSize.y)
    {
        ImGui::SetNextWindowSize({ m_size.x, m_size.y + (m_footerSize.y - m_lastFooterSize.y) });
    }
    else if(m_lastFooterSize.y > m_footerSize.y)
    {
        ImGui::SetNextWindowSize({ m_size.x, m_size.y - (m_lastFooterSize.y - m_footerSize.y) });
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
    ImGui::BeginChild(ImGui::GetID((m_name.getName() + "Footer").c_str()), ImVec2(ImGui::GetContentRegionAvail().x, 0),
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

    m_headerSize = ImGui::GetWindowContentRegionMax();
    m_headerSize.x -= ImGui::GetWindowContentRegionMin().x;
    m_headerSize.y -= ImGui::GetWindowContentRegionMin().y;

    m_headerSize.x += ImGui::GetScrollMaxX();
    m_headerSize.y += ImGui::GetScrollMaxY();

    m_headerSize.y += 8 * 2;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_bodyPadding);
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

    const auto regionAvail = ImGui::GetContentRegionAvail();

    float buttonsTotalWidth = 0.0f;
    for(const auto& btn : m_buttons)
    {
        buttonsTotalWidth += btn.m_currentSize.x;
    }

    footerRender();

    // drawing buttons ==========================================
    // ==========================================================

    ImGui::SetCursorPosX(regionAvail.x - buttonsTotalWidth);

    for(auto& btn : m_buttons)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, btn.m_rounding);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, btn.m_padding);

        ImGui::PushStyleColor(ImGuiCol_Button, btn.m_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btn.m_hoveredColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, btn.m_activeColor);

        ImGui::PushStyleColor(ImGuiCol_Border, btn.m_borderColor);
        ImGui::PushStyleColor(ImGuiCol_BorderShadow, btn.m_borderShadowColor);

        ImGui::SetCursorPosY(ImGui::GetStyle().WindowPadding.y);

        if(ImGui::Button(btn.m_text.c_str(), btn.m_size) || (btn.isFastClicked && btn.isFastClicked(btn)))
        {
            btn.onClicked(btn);
        }

        btn.m_currentSize = ImGui::GetItemRectSize();

        ImGui::PopStyleVar(2);

        ImGui::PopStyleColor(5);

        ImGui::SameLine();
    }

    // ImGui::NewLine();

    m_lastFooterSize = m_footerSize;

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

void SGE::Window::addButton(const SGE::Button& button) noexcept
{
    removeButton(button.m_name);

    m_buttons.push_back(button);
}

SGE::Button* SGE::Window::getButton(const std::string& name) noexcept
{
    auto foundIt = std::find_if(m_buttons.begin(), m_buttons.end(), [&name](const Button& button) {
        return name == button.m_name;
    });

    return foundIt != m_buttons.end() ? &*foundIt : nullptr;
}

void SGE::Window::removeButton(const std::string& name) noexcept
{
    std::erase_if(m_buttons, [&name](const Button& button) {
        return name == button.m_name;
    });
}
