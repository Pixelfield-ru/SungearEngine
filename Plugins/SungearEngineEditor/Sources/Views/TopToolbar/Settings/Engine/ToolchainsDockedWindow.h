//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H

#include "Views/DockedWindow.h"
#include "Views/Popup.h"
#include "Views/Tree.h"
#include "SelectedToolchainDockedWindow.h"
#include "Settings/EngineSettings.h"

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

        // ref from EngineSettingsView
        SGCore::Ref<EngineSettings> m_engineSettingsRef;

        SGCore::Ref<SelectedToolchainDockedWindow> getSelectedToolchainDockedWindow() const noexcept;

        void refreshToolchainsList();

    private:
        Popup m_toolchainsVariantsPopup;
        Tree m_currentAddedToolchainsTree;

        SGCore::Ref<SelectedToolchainDockedWindow> m_selectedToolchainDockedWindow;
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINSDOCKEDWINDOW_H
