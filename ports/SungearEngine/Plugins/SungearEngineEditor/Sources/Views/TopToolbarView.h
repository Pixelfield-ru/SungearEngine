//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
#define SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct TopToolbarView : public SGCore::ImGuiWrap::IView
    {
        bool begin() final;
        void renderBody() final;
        void end() final;
    };
}

#endif //SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
