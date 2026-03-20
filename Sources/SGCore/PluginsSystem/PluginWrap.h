//
// Created by ilya on 20.03.24.
//

#pragma once

#include "DynamicLibrary.h"
#include "IPlugin.h"

namespace SGCore
{
    /**
     * A wrapper structure containing an SGCore::IPlugin structure object and the dynamic library (SGCore::DynamicLibrary) from which the plugin was loaded.
     */
    struct SGCORE_EXPORT PluginWrap
    {
        friend struct PluginsManager;
        
        /**
         * \return Dynamic library (SGCore::DynamicLibrary) of the plugin (SGCore::IPlugin).
         */
        [[nodiscard]] auto getPluginLib() const noexcept
        {
            return m_pluginLib;
        }
        
        /**
         * \return Plugin (SGCore::IPlugin).
         */
        [[nodiscard]] auto getPlugin() const noexcept
        {
            return m_plugin;
        }
        
    private:
        Ref<DynamicLibrary> m_pluginLib;
        Ref<IPlugin> m_plugin;
    };
}
