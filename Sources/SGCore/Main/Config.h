//
// Created by ilya on 05.05.24.
//

#pragma once

#include <string>

#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"

namespace SGCore
{
    struct SGCORE_EXPORT LoadablePluginConfig
    {
        std::string m_pluginName;
        InterpolatedPath m_pluginPath;
        std::vector<std::string> m_pluginEntryArgs;
        std::string m_pluginCMakeBuildDir;

        bool m_enabled = true;
    };

    struct SGCORE_EXPORT Config
    {
        /// Plugins that will be loaded at start of SungearEngine executable.
        std::vector<LoadablePluginConfig> m_loadablePlugins;
        /// Contains path to engine. If not present then equals to ${enginePath}.
        InterpolatedPath m_enginePath = "${enginePath}";
    };
}
