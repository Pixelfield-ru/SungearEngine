//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_EXPLORER_H
#define SUNGEARENGINEEDITOR_EXPLORER_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct Explorer : SGCore::ImGuiWrap::IView
    {
        virtual void renderBody() override;
    };
}

#endif //SUNGEARENGINEEDITOR_EXPLORER_H
