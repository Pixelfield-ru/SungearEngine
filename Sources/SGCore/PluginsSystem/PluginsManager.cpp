//
// Created by ilya on 19.03.24.
//

#include "PluginsManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/CrashHandler/Platform.h"

#include <filesystem>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "PluginWrap.h"
#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/Formatter.h"

#ifdef PLATFORM_OS_LINUX
#include <dlfcn.h>
#endif

SGCore::PluginProject SGCore::PluginsManager::createPluginProject(const std::filesystem::path& projectPath,
                                                                  const std::string& pluginName,
                                                                  const std::string& cxxStandard,
                                                                  std::string& error)
{
    const std::string u8Path = Utils::toUTF8(projectPath.u16string());

    PluginProject pluginProject;
    pluginProject.m_pluginPath = u8Path + '/' + pluginName;
    pluginProject.m_parentPath = projectPath;
    pluginProject.m_name = pluginName;
    pluginProject.m_cxxStandard = cxxStandard;

    const char* sgSourcesRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    if(!sgSourcesRoot || !std::filesystem::exists(std::string(sgSourcesRoot) + "/cmake/SungearEngineInclude.cmake"))
    {
        error = "Error: Incorrect Sungear Engine sources directory!\nCheck if the 'SUNGEAR_SOURCES_ROOT' environment variable\nexists and correct.";
        return pluginProject;
    }
    
    try
    {
        const std::string sgSourcesRootStr = sgSourcesRoot;
        
        auto finalSGPath = CoreMain::m_pathToSungearEngineSDKSources;
        if(std::filesystem::path::preferred_separator == L'\\')
        {
            finalSGPath = SGCore::Utils::replaceAll<char>(finalSGPath, R"(\)", R"(\\)");
        }

        const auto pluginDir = u8Path + "/" + pluginName;
        const auto pluginSourcesDir = pluginDir + "/Sources";
        
        if(std::filesystem::exists(pluginDir) && !std::filesystem::is_empty(pluginDir))
        {
            error = "Error: Project directory is not empty!";
            return pluginProject;
        }
        
        if(cxxStandard != "C++98" && cxxStandard != "C++03" && cxxStandard != "C++11" && cxxStandard != "C++14" &&
           cxxStandard != "C++17" && cxxStandard != "C++20" && cxxStandard != "C++23" && cxxStandard != "C++26")
        {
            error = "Error: Unknown CXX Standard!";
            return pluginProject;
        }
        
        std::filesystem::create_directories(pluginDir);
        std::filesystem::create_directories(pluginSourcesDir);
        
        std::string upperPluginName = pluginName;
        std::transform(pluginName.begin(), pluginName.end(), upperPluginName.begin(), ::toupper);
        
        // SETTING UP FORMATTER
        
        Formatter formatter;
        
        formatter["pluginName"] = pluginName;
        formatter["cxxStandard"] = cxxStandard.substr(3);
        formatter["upperPluginName"] = upperPluginName;
        
        // ====================================== CMakeLists.txt
        
        std::string cmakeListsContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/CMakeLists.cmake"));
        
        std::ofstream cmakeListsStream(pluginDir + "/CMakeLists.txt");
        cmakeListsStream << cmakeListsContent;

        // ====================================== cmake/include_as_plugin.cmake

        std::string includeAsPluginContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/include_as_plugin.cmake"));

        FileUtils::writeToFile(pluginDir + "/cmake/include_as_plugin.cmake", includeAsPluginContent, false, true);
        
        // ====================================== PluginMain.h
        
        std::string pluginMainHContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/PluginMain.h"));
        
        std::ofstream pluginMainHStream(pluginSourcesDir + "/PluginMain.h");
        pluginMainHStream << pluginMainHContent;
        
        // ====================================== PluginMain.cpp
        
        std::string pluginMainCPPContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/PluginMain.cpp"));
        
        std::ofstream pluginMainCPPStream(pluginSourcesDir + "/PluginMain.cpp");
        pluginMainCPPStream << pluginMainCPPContent;
        
        // ====================================== {pluginName}.h
        
        std::string pluginHContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/PluginOwn.h"));
        
        std::ofstream pluginHStream(pluginSourcesDir + "/" + fmt::format("{0}.h", pluginName));
        pluginHStream << pluginHContent;
        
        // ====================================== {pluginName}.cpp
        
        std::string pluginCPPContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/PluginOwn.cpp"));
        
        std::ofstream pluginCPPStream(pluginSourcesDir + "/" + fmt::format("{0}.cpp", pluginName));
        pluginCPPStream << pluginCPPContent;
        
        // ====================================== CMakePresets.json
        
        std::string cmakePresetsContent = FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/CMakePresets.json");
        
        std::ofstream cmakePresetsStream(pluginDir + "/CMakePresets.json");
        cmakePresetsStream << cmakePresetsContent;
        
        // ====================================== vcpkg.json
        
        std::string vcpkgContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/vcpkg.json"));
        
        std::ofstream vcpkgStream(pluginDir + "/vcpkg.json");
        vcpkgStream << vcpkgContent;

        // ====================================== .gitignore

        std::string gitignoreContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.templates/.gitignore"));

        std::ofstream gitignoreStream(pluginDir + "/.gitignore");
        gitignoreStream << gitignoreContent;

        // ====================================== Resources directory
        std::filesystem::create_directory(pluginDir + "/Resources");
    }
    catch(const std::filesystem::filesystem_error& err)
    {
        error = std::string("Error: ") + err.what();
        return pluginProject;
    }

    error = "";
    return pluginProject;
}

SGCore::Ref<SGCore::PluginWrap>
SGCore::PluginsManager::loadPlugin(const std::string& pluginName,
                                   const std::filesystem::path& localPluginPath,
                                   const std::vector<std::string>& entryArgs,
                                   const std::string& cmakeBuildDir)
{
    const std::string u8Path = Utils::toUTF8(localPluginPath.u16string());

    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(m_plugins.begin(), m_plugins.end(), [&pluginName](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName;
    });
    
    if(foundIt != m_plugins.end())
    {
        loadedPlugin = *foundIt;
        
        if(loadedPlugin->getPlugin()->m_path != localPluginPath)
        {
            LOG_W(SGCORE_TAG, "Warning: Plugin '{}' (ver. {}) has other path. Requested local load path: '{}', plugin cached path: '{}'.",
                  pluginName, loadedPlugin->m_plugin->m_version, u8Path, loadedPlugin->getPlugin()->m_path);
        }
    }
    else
    {
        loadedPlugin = MakeRef<PluginWrap>();
        
        std::string pluginDLPath = std::filesystem::path(u8Path + "/" + cmakeBuildDir + "/" + pluginName + DL_POSTFIX).string();
        
        std::string dlErr;
        
        Ref<DynamicLibrary> pluginDL = MakeRef<DynamicLibrary>();
        pluginDL->load(pluginDLPath, dlErr);
        
        if(!pluginDL->getNativeHandler())
        {
            LOG_E(SGCORE_TAG, "Cannot load plugin '{}' by path '{}'. Error is: {}", pluginName, pluginDLPath, dlErr);
            return nullptr;
        }
        
        std::string dlEntryErr;
        auto pluginEntry = pluginDL->loadFunction<Ref<IPlugin>()>("SGPluginMain", dlEntryErr);
        
        if(!pluginEntry)
        {
            LOG_E(SGCORE_TAG, "Cannot load plugin '{}' main function by path '{}'. Error is: {}", pluginName, pluginDLPath, dlEntryErr);
            return nullptr;
        }
        
        loadedPlugin->m_plugin = pluginEntry();
        loadedPlugin->m_plugin->m_path = u8Path;
        std::cout << "LOADED PLUGIN: " << loadedPlugin->m_plugin << std::endl;
        loadedPlugin->m_plugin->onConstruct(entryArgs);
        std::cout << "ERROR HERE" << std::endl;
        loadedPlugin->m_pluginLib = pluginDL;

        LOG_I(SGCORE_TAG, "Loaded plugin '{}'.", pluginName);
    }
    
    m_plugins.push_back(loadedPlugin);
    
    return loadedPlugin;
}

SGCore::Ref<SGCore::PluginWrap>
SGCore::PluginsManager::reloadPlugin(const std::string& pluginName,
                                     const std::vector<std::string>& entryArgs,
                                     const std::string& cmakeBuildDir)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(m_plugins.begin(), m_plugins.end(), [&pluginName](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName;
    });
    
    if(foundIt != m_plugins.end())
    {
        loadedPlugin = *foundIt;
        
        std::string dlErr;
        std::string dlEntryErr;
        
        std::string pluginSrcPath = loadedPlugin->m_plugin->m_path;
        std::string pluginDLPath = pluginSrcPath + sep + cmakeBuildDir + sep + pluginName + DL_POSTFIX;
        
        loadedPlugin->m_plugin = nullptr;
        
        loadedPlugin->m_pluginLib->unload();
        loadedPlugin->m_pluginLib->load(pluginDLPath, dlErr);
        
        if(!loadedPlugin->m_pluginLib->getNativeHandler())
        {
            LOG_E(SGCORE_TAG, "Cannot reload plugin '{}'. Error is: {}", pluginName, dlErr);
            return nullptr;
        }
        
        // auto pluginEntry = std::function<Ref<IPlugin>()>((Ref<IPlugin>(*)()) dlsym(loadedPlugin->m_pluginLib->getNativeHandler(), "SGPluginMain"));
        auto pluginEntry = loadedPlugin->m_pluginLib->loadFunction<Ref<IPlugin>()>("SGPluginMain", dlEntryErr);
        
        if(!pluginEntry)
        {
            LOG_E(SGCORE_TAG, "Cannot reload plugin '{}' main function. Error is: {}", pluginName, dlEntryErr);
            return nullptr;
        }
        
        loadedPlugin->m_plugin = pluginEntry();
        loadedPlugin->m_plugin->m_path = pluginSrcPath;
        loadedPlugin->m_plugin->onConstruct(entryArgs);
        
        return loadedPlugin;
    }
    else
    {
        LOG_E(SGCORE_TAG, "Cannot reload plugin: The plugin '{}' has not been loaded before.", pluginName);
    }
    
    return nullptr;
}

void SGCore::PluginsManager::unloadPlugin(const std::string& pluginName) noexcept
{
    auto foundIt = std::find_if(m_plugins.begin(), m_plugins.end(), [&pluginName](const Ref<PluginWrap>& plugin) {
        return plugin->m_plugin->m_name == pluginName;
    });

    if(foundIt != m_plugins.end())
    {
        auto plugin = *foundIt;

        plugin->m_plugin = nullptr;
        plugin->m_pluginLib->unload();
        m_plugins.erase(foundIt);
    }
}

bool SGCore::PluginsManager::isPluginExists(const std::string& pluginName) noexcept
{
    return std::find_if(m_plugins.begin(), m_plugins.end(), [&pluginName](const Ref<PluginWrap>& plugin) {
        return plugin->m_plugin->m_name == pluginName;
    }) != m_plugins.end();
}

void SGCore::PluginsManager::unloadAllPlugins() noexcept
{
    for(auto& plugin : m_plugins)
    {
        plugin->m_plugin = nullptr;
        plugin->m_pluginLib->unload();
    }

    m_plugins.clear();
}
