//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H

#include "Views/DockedWindow.h"
#include "Views/Popup.h"

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
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
