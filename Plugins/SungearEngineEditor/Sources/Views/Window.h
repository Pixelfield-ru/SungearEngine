//
// Created by Ilya on 18.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_WINDOW_H
#define SUNGEARENGINEEDITOR_WINDOW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct Window : SGCore::ImGuiWrap::IView
    {
        bool begin() override;
        void end() override;
        virtual void postRenderBody() { };

        void onActiveChangedListener() override;

        bool m_isPopupWindow = false;
        bool m_enableDocking = false;

        ImGuiDockNodeFlags m_dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

    protected:
        ImGuiID m_dockspaceID;
        ImVec2 m_bodyRegionMax { };
        ImVec2 m_minSize { };
        ImVec2 m_size { };
    };
}

#endif //SUNGEARENGINEEDITOR_WINDOW_H
