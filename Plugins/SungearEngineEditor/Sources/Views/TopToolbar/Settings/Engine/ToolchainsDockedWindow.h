//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H

#include "Views/DockedWindow.h"

namespace SGE
{
    struct ToolchainsDockedWindow : DockedWindow
    {
        void renderBody() final;
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
