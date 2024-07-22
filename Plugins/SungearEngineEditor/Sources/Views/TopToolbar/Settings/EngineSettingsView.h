//
// Created by Ilya on 22.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
#define SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H

#include "SettingsView.h"

namespace SGE
{
    struct EngineSettingsView : SettingsView
    {
        EngineSettingsView();
        EngineSettingsView(const EngineSettingsView&) = default;
        EngineSettingsView(EngineSettingsView&&) = default;
    };
}

#endif //SUNGEARENGINEEDITOR_ENGINESETTINGSVIEW_H
