//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
#define SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H

#include "Views/TopToolbar/Settings/SettingsView.h"
#include "ToolchainsDockedWindow.h"
#include "Settings/EngineSettings.h"

namespace SGE
{
    struct EngineSettingsView : SettingsView
    {
        EngineSettingsView();
        EngineSettingsView(const EngineSettingsView&) = default;
        EngineSettingsView(EngineSettingsView&&) = default;

        void onActiveChangedListener() final;

        SGCore::Ref<EngineSettings> m_engineSettingsInstance = SGCore::MakeRef<EngineSettings>();

    protected:
        void onDock() final;

    private:
        SGCore::Ref<ToolchainsDockedWindow> m_toolchainsDockedWindow;
    };
}

#endif //SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
