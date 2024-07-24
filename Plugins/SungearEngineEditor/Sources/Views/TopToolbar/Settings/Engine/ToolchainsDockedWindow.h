//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H

#include "Views/DockedWindow.h"
#include "Views/Popup.h"
#include "Views/Tree.h"

namespace SGE
{
    struct ToolchainsDockedWindow : DockedWindow
    {
        ToolchainsDockedWindow();
        ToolchainsDockedWindow(const ToolchainsDockedWindow&) = default;
        ToolchainsDockedWindow(ToolchainsDockedWindow&&) = default;

        void renderBody() final;

    private:
        Popup m_toolchainsVariantsPopup;
        Tree m_currentAddedToolchainsTree;
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
