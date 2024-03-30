//
// Created by ilya on 20.03.24.
//

#ifndef SUNGEARENGINE_PLUGINWRAP_H
#define SUNGEARENGINE_PLUGINWRAP_H

#include "DynamicLibrary.h"
#include "IPlugin.h"

namespace SGCore
{
    struct PluginWrap
    {
        friend struct PluginsManager;
        
        [[nodiscard]] auto getPluginLib() const noexcept
        {
            return m_pluginLib;
        }
        
        [[nodiscard]] auto getPlugin() const noexcept
        {
            return m_plugin;
        }
        
    private:
        Ref<DynamicLibrary> m_pluginLib;
        Ref<IPlugin> m_plugin;
    };
}

#endif //SUNGEARENGINE_PLUGINWRAP_H
