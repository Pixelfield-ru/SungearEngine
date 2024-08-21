//
// Created by Ilya on 21.08.2024.
//

#ifndef SUNGEARENGINEEDITOR_LOGSWINDOW_H
#define SUNGEARENGINEEDITOR_LOGSWINDOW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct LogsWindow : public SGCore::ImGuiWrap::IView
    {
        bool begin() final;
        void renderBody() final;
        void end() final;
    };
}

#endif //SUNGEARENGINEEDITOR_LOGSWINDOW_H
