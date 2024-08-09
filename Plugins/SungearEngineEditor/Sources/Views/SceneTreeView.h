//
// Created by ilya on 23.06.24.
//

#ifndef SUNGEARENGINEEDITOR_SCENETREEVIEW_H
#define SUNGEARENGINEEDITOR_SCENETREEVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct SceneTreeView : public SGCore::ImGuiWrap::IView
    {
        bool begin() final;
        void renderBody() final;
        void end() final;
    };
}

#endif //SUNGEARENGINEEDITOR_SCENETREEVIEW_H
