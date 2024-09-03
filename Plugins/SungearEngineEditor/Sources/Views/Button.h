//
// Created by Ilya on 20.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_BUTTON_H
#define SUNGEARENGINEEDITOR_BUTTON_H

#include <string>
#include <functional>
#include <imgui.h>
#include <SGCore/ImGuiWrap/Views/IView.h>
#include "Styles/StylesManager.h"

namespace SGE
{
    struct Button
    {
        std::string m_text;
        std::string m_name = m_text;

        std::function<bool(Button& self)> isFastClicked;
        std::function<void(Button& self, SGCore::ImGuiWrap::IView* parentView)> onClicked;

        ImVec4 m_color = StylesManager::getCurrentStyle()->m_initialColors[ImGuiCol_Button];
        ImVec4 m_hoveredColor = StylesManager::getCurrentStyle()->m_initialColors[ImGuiCol_ButtonHovered];
        ImVec4 m_activeColor = StylesManager::getCurrentStyle()->m_initialColors[ImGuiCol_ButtonActive];

        ImVec4 m_borderColor = StylesManager::getCurrentStyle()->m_initialColors[ImGuiCol_Border];
        ImVec4 m_borderShadowColor = StylesManager::getCurrentStyle()->m_initialColors[ImGuiCol_BorderShadow];

        ImVec2 m_padding = { 7, 5 };
        ImVec2 m_size = { 0, 0 };
        ImVec2 m_currentSize = { 0, 0 }; // DO NOT SPECIFY (NO EFFECT WHEN SPECIFIED)

        float m_rounding = 3.0f;
    };
}

#endif //SUNGEARENGINEEDITOR_BUTTON_H
