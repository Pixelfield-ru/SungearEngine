//
// Created by ilya on 05.05.24.
//

#ifndef SUNGEARENGINE_CONFIG_H
#define SUNGEARENGINE_CONFIG_H

#include <string>

#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"

namespace SGCore
{
    struct LoadablePluginConfig
    {
        std::string m_pluginName;
        InterpolatedPath m_pluginPath;
        std::vector<std::string> m_pluginEntryArgs;
        std::string m_pluginCMakeBuildDir;

        bool m_enabled = true;
    };

    struct Config
    {
        /// Plugins that will be loaded at start of SungearEngine executable.
        std::vector<LoadablePluginConfig> m_loadablePlugins;
        /// Contains path to engine. If not present then equals to ${enginePath}.
        InterpolatedPath m_enginePath = "${enginePath}";
    };
}

#endif // SUNGEARENGINE_CONFIG_H
