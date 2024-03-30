//
// Created by ilya on 19.03.24.
//

#ifndef SUNGEARENGINE_PLUGINSMANAGER_H
#define SUNGEARENGINE_PLUGINSMANAGER_H

#include <unordered_map>
#include <vector>

#include "sgcore_export.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/Utils.h"
#include "PluginWrap.h"
#include "IPlugin.h"

namespace SGCore
{
    struct SGCORE_EXPORT PluginsManager
    {
        SG_NOINLINE static auto& getPlugins() noexcept
        {
            return s_plugins;
        }
        
        static std::string createPluginProject(const std::string& projectPath,
                                               const std::string& pluginName,
                                               const std::string& cxxStandard);
        
        static Ref<PluginWrap> loadPlugin(const std::string& pluginName,
                                          const std::string& pluginVersion,
                                          const std::string& localPluginPath,
                                          const std::vector<std::string>& entryArgs,
                                          PluginBuildType pluginBuildType);
        
        static Ref<PluginWrap> reloadPlugin(const std::string& pluginName,
                                            const std::string& pluginVersion,
                                            const std::vector<std::string>& entryArgs,
                                            PluginBuildType pluginBuildType);
        
    private:
        static inline std::vector<Ref<PluginWrap>> s_plugins;
    };
}

#endif //SUNGEARENGINE_PLUGINSMANAGER_H
