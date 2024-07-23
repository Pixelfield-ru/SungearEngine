//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_DOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_DOCKEDWINDOW_H

#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct DockedWindow : SGCore::ImGuiWrap::IView
    {
        ImVec2 m_minSize { };

        ImVec2 m_padding = ImGui::GetStyle().WindowPadding;
        ImVec2 m_itemsSpacing = ImGui::GetStyle().ItemSpacing;

        bool begin() override;
        void end() override;

        void split(
                ImGuiDir splitDir,
                float sizeRatioForNodeAtDir,
                DockedWindow* outDockedWindowAtDir,
                DockedWindow* outDockedWindowAtOppositeDir);

        ImGuiID m_thisDockNodeID { };

    private:
        ImVec2 m_currentSize { };
    };
}

#endif //SUNGEARENGINEEDITOR_DOCKEDWINDOW_H
