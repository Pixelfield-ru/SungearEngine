//
// Created by Ilya on 10.07.2024.
//

#ifndef SUNGEARENGINE_PLUGINPROJECT_H
#define SUNGEARENGINE_PLUGINPROJECT_H

#include <filesystem>
#include "DynamicLibrary.h"

namespace SGCore
{
    struct PluginProject
    {
        std::filesystem::path m_parentPath;
        std::filesystem::path m_pluginPath;
        std::string m_name;
        std::string m_cxxStandard;
    };
}

#endif //SUNGEARENGINE_PLUGINPROJECT_H
