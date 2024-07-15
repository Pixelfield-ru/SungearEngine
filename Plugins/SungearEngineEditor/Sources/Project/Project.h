//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_PROJECT_H
#define SUNGEARENGINEEDITOR_PROJECT_H

#include <SGCore/PluginsSystem/PluginProject.h>
#include <functional>
#include <SGCore/PluginsSystem/DynamicLibrary.h>

namespace SGE
{
    struct Project
    {
        SGCore::PluginProject m_pluginProject;
        SGCore::DynamicLibrary m_editorHelper;
        std::function<void()> m_editorHelperEntryPoint;
        std::function<void()> m_editorHelperExitPoint;
    };
}

#endif //SUNGEARENGINEEDITOR_PROJECT_H
