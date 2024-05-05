//
// Created by ilya on 19.03.24.
//

#ifndef SUNGEARENGINE_IPLUGIN_H
#define SUNGEARENGINE_IPLUGIN_H

#include <vector>
#include <string>
#include <functional>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    /**
     * Plugin build type. Used in SGCore::PluginsManager.
     */
    enum PluginBuildType
    {
        PBT_DEBUG, /// Debug build.
        PBT_RELEASE /// Release build.
    };
    
    struct IPlugin
    {
        friend struct PluginsManager;
        
        /**
         * Virtual plugin destructor.
         * \note Please note that if you create your own plugin, then you must implement this destructor in the structure of your plugin.\n
         * In it, you must remove all resources from all collections located directly in the engine, since this process does not happen automatically.\n
         * For example: you must unsubscribe from all events you subscribed to; you must delete all assets that were loaded by the plugin using the standard SGCore::AssetManager instance (SGCore::AssetsManager::getInstance).
         */
        virtual ~IPlugin() = default;
        
        /**
         * Compares the names and versions of the current plugin and the {other} plugin.
         * \param other Plugin for comparison.
         * \return Are the plugins equal?
         */
        bool operator==(const IPlugin& other) const noexcept;
        /**
         * Compares the names and versions of the current plugin and the {other} plugin.
         * \param other Plugin for comparison.
         * \return Are the plugins not equal?
         */
        bool operator!=(const IPlugin& other) const noexcept;
        
        /**
         * Returns the local path on the device to the plugin folder.
         * \return Local path to the plugin.
         */
        [[nodiscard]] std::string getLocalPath() const noexcept;
        
        /**
         * \return Plugin name.
         */
        [[nodiscard]] std::string getName() const noexcept;
        /**
         * \return Plugin version.
         */
        [[nodiscard]] std::string getVersion() const noexcept;
        
    protected:
        /**
         * A function that is guaranteed to be called immediately after calling the plugin structure constructor.\n
         * It is called in SGCore::PluginsManager::loadPlugin and also in SGCore::PluginsManager::reloadPlugin.\n
         * If you create your own plugin, then you must implement this function in the structure of your plugin.\n
         * When calling SGCore::PluginsManager::createPluginProject, the implementation of this function is generated automatically.
         * \param args Arguments that will be passed to this function.\n
         * Using them you can make flags for the plugin.
         * \return String with an error when constructing the plugin.\n
         * In ANY implementation, the function SHOULD return an EMPTY string when loading the plugin WITHOUT ERROR.
         */
        virtual std::string onConstruct(const std::vector<std::string>& args) = 0;
        
        std::string m_name;
        std::string m_version;
        
        /**
         * Plugin local path on computer.
         */
        std::string m_path;
    };
}

#endif // SUNGEARENGINE_IPLUGIN_H
