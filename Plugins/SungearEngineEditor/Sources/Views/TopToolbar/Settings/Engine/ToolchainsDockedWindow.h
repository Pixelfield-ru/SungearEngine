//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H

#include "Views/DockedWindow.h"
#include "Views/Popup.h"
#include "Views/Tree.h"
#include "Toolchains/Toolchains.h"
#include "SelectedToolchainDockedWindow.h"

namespace SGE
{
    struct ToolchainsDockedWindow : DockedWindow
    {
        ToolchainsDockedWindow();
        ToolchainsDockedWindow(const ToolchainsDockedWindow&) = default;
        ToolchainsDockedWindow(ToolchainsDockedWindow&&) = default;

        // bool begin() final;
        void renderBody() final;
        // void end() final;

        Toolchains m_currentToolchains;

        SGCore::Ref<SelectedToolchainDockedWindow> getSelectedToolchainDockedWindow() const noexcept;

    private:
        Popup m_toolchainsVariantsPopup;
        Tree m_currentAddedToolchainsTree;

        SGCore::Ref<SelectedToolchainDockedWindow> m_selectedToolchainDockedWindow;

        void refreshToolchainsList();
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
