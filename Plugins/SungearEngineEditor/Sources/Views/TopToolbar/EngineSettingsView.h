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
        void renderBody() final;
        void postRenderBody() final;

    private:
        bool m_firstTime = true;
    };
}

#endif //SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
