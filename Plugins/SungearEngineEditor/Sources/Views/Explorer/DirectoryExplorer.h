//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
#define SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H

#include <SGCore/ImGuiWrap/Views/IView.h>

namespace SGE
{
    struct DirectoryExplorer : SGCore::ImGuiWrap::IView
    {
        void renderBody() override;
    };
}

#endif //SUNGEARENGINEEDITOR_DIRECTORYEXPLORER_H
