//
// Created by Ilya on 17.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_SETTINGSVIEW_H
#define SUNGEARENGINEEDITOR_SETTINGSVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "Views/Window.h"
#include "Views/Tree.h"
#include "Views/DockedWindow.h"

namespace SGE
{
    struct SettingsView : Window
    {
        SettingsView();
        SettingsView(const SettingsView&) = default;
        SettingsView(SettingsView&&) = default;

        bool begin() override;
        void postRender() override;

        std::function<void()> onSettingsContentDraw;

        bool m_isSettingsContentViewContainsDockedWindow = false;

        virtual void onOKPressed() noexcept { };
        virtual void onApplyPressed() noexcept { };
        virtual void onCancelPressed() noexcept { };

    protected:
        virtual void onDock() { };

        Tree m_settingsTree;

        SGCore::Ref<DockedWindow> m_settingsTreeDockedWindow;
        SGCore::Ref<DockedWindow> m_settingsContentDockedWindow;

        bool m_firstTime = true;
    };
}

#endif //SUNGEARENGINEEDITOR_SETTINGSVIEW_H
