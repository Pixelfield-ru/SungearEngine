//
// Created by Ilya on 10.07.2024.
//

#pragma once

#include <filesystem>
#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT PluginProject
    {
        std::filesystem::path m_parentPath;
        std::filesystem::path m_pluginPath;
        std::string m_name;
        std::string m_cxxStandard;
    };
}
