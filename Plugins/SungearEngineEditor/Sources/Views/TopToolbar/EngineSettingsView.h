//
// Created by Ilya on 17.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
#define SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "Views/Window.h"

namespace SGE
{
    struct EngineSettingsView : Window
    {
        EngineSettingsView();
        EngineSettingsView(const EngineSettingsView&) = default;
        EngineSettingsView(EngineSettingsView&&) = default;

        void renderBody() final;
        void postRenderBody() final;

    private:
        ImVec2 m_currentTreeSize { };

        bool m_firstTime = true;
    };
}

#endif //SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
