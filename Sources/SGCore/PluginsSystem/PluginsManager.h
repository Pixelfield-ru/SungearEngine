//
// Created by ilya on 19.03.24.
//

#ifndef SUNGEARENGINE_PLUGINSMANAGER_H
#define SUNGEARENGINE_PLUGINSMANAGER_H

#include <unordered_map>
#include <vector>

#include "sgcore_export.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Utils.h"
#include "PluginWrap.h"
#include "IPlugin.h"
#include "PluginProject.h"

namespace SGCore
{
    struct SGCORE_EXPORT PluginsManager
    {
        /**
         * Returns a vector of all loaded plugins.
         * \return Vector of plugins.
         */
        SG_NOINLINE static auto& getPlugins() noexcept
        {
            return m_plugins;
        }
        
        /**
         * Creates a standard plugin project.\n\n
         * Creates files in the plugin folder: CMakeLists.txt for building the plugin;\n
         * Sources/PluginMain.h and Sources/PluginMain.cpp for loading the plugin by the engine;\n
         * Sources/{pluginName}.h and Sources/{pluginName}.cpp to implement a structure called {pluginName} that inherits SGCore::IPlugin.
         *
         * \note Please note that to create a plugin project you must have the SUNGEAR_SOURCES_ROOT environment variable set.\n
         * It should indicate the path to the Sungear Engine sources.
         *
         * \param[in] projectPath - Path to the folder in which the folder with the plugin project will be created.
         * \param[in] pluginName - Plugin name.\n
         * When creating a plugin, a folder with the same name will be created, as well as the name of the structure that inherits SGCore::IPlugin will be the same.
         * \param[in] cxxStandard - The C++ standard that the plugin will use.\n
         * Available standards: C++98, C++03, C++11, C++14, C++17, C++20, C++23.
         * \return The string containing the error caused when creating the plugin.\n
         * If the plugin was successfully created, the string is empty.
         */
        static PluginProject createPluginProject(const std::filesystem::path& projectPath,
                                                 const std::string& pluginName,
                                                 const std::string& cxxStandard,
                                                 std::string& error);
        
        /**
         * Loads the plugin by local path {localPluginPath} and adds the plugin to the plugin vector upon successful loading.
         * \param[in] pluginName - Plugin name.
         * \param[in] localPluginPath - Local path on the device to the plugin folder.
         * \param[in] entryArgs - Arguments that will be passed to the SGCore::IPlugin::onConstruct function.
         * \param[in] cmakeBuildDir -
         * \todo Make an error return when loading the plugin.
         * \return SGCore::Ref\<SGCore::PluginWrap\>.
         */
        static Ref<PluginWrap> loadPlugin(const std::string& pluginName,
                                          const std::filesystem::path& localPluginPath,
                                          const std::vector<std::string>& entryArgs,
                                          const std::string& cmakeBuildDir);
        
        /**
         * Reloads the plugin named {pluginName}.\n
         * To reload a plugin, the path that was used to load the plugin is used.
         * \param[in] pluginName - The name of the plugin that needs to be reloaded.
         * \param[in] entryArgs - Arguments that will be passed to the SGCore::IPlugin::onConstruct function.
         * \param[in] cmakeBuildDir -
         * \todo Make an error return when loading the plugin.
         * \return SGCore::Ref\<SGCore::PluginWrap\>.
         */
        static Ref<PluginWrap> reloadPlugin(const std::string& pluginName,
                                            const std::vector<std::string>& entryArgs,
                                            const std::string& cmakeBuildDir);

        static void unloadAllPlugins() noexcept;

        static void unloadPlugin(const std::string& pluginName) noexcept;

        static void loadPluginsFromFile(const std::filesystem::path& pluginsPath) noexcept;

        static bool isPluginExists(const std::string& pluginName) noexcept;
        
    private:
        static inline std::vector<Ref<PluginWrap>> m_plugins;
    };
}

#endif //SUNGEARENGINE_PLUGINSMANAGER_H
