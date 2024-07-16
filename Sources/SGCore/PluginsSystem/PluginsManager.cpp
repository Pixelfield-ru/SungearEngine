//
// Created by ilya on 19.03.24.
//

#include "PluginsManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/CrashHandler/Platform.h"

#include <filesystem>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "PluginWrap.h"
#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/Formatter.h"

#ifdef PLATFORM_OS_LINUX
#include <dlfcn.h>
#endif

SGCore::PluginProject SGCore::PluginsManager::createPluginProject(const std::string& projectPath,
                                                                  const std::string& pluginName,
                                                                  const std::string& cxxStandard,
                                                                  std::string& error)
{
    PluginProject pluginProject;
    pluginProject.m_pluginPath = projectPath + '/' + pluginName;
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

        const auto pluginDir = projectPath + "/" + pluginName;
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
        
        std::string cmakeListsContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/CMakeLists.cmake"));
        
        std::ofstream cmakeListsStream(pluginDir + "/CMakeLists.txt");
        cmakeListsStream << cmakeListsContent;

        // ====================================== cmake/include_as_plugin.cmake

        std::string includeAsPluginContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/include_as_plugin.cmake"));

        FileUtils::writeToFile(pluginDir + "/cmake/include_as_plugin.cmake", includeAsPluginContent, false, true);
        
        // ====================================== PluginMain.h
        
        std::string pluginMainHContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/PluginMain.h"));
        
        std::ofstream pluginMainHStream(pluginSourcesDir + "/PluginMain.h");
        pluginMainHStream << pluginMainHContent;
        
        // ====================================== PluginMain.cpp
        
        std::string pluginMainCPPContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/PluginMain.cpp"));
        
        std::ofstream pluginMainCPPStream(pluginSourcesDir + "/PluginMain.cpp");
        pluginMainCPPStream << pluginMainCPPContent;
        
        // ====================================== {pluginName}.h
        
        std::string pluginHContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/PluginOwn.h"));
        
        std::ofstream pluginHStream(pluginSourcesDir + "/" + fmt::format("{0}.h", pluginName));
        pluginHStream << pluginHContent;
        
        // ====================================== {pluginName}.cpp
        
        std::string pluginCPPContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/PluginOwn.cpp"));
        
        std::ofstream pluginCPPStream(pluginSourcesDir + "/" + fmt::format("{0}.cpp", pluginName));
        pluginCPPStream << pluginCPPContent;
        
        // ====================================== CMakePresets.json
        
        std::string cmakePresetsContent = FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/CMakePresets.json");
        
        std::ofstream cmakePresetsStream(pluginDir + "/CMakePresets.json");
        cmakePresetsStream << cmakePresetsContent;
        
        // ====================================== vcpkg.json
        
        std::string vcpkgContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/vcpkg.json"));
        
        std::ofstream vcpkgStream(pluginDir + "/vcpkg.json");
        vcpkgStream << vcpkgContent;

        // ====================================== .gitignore

        std::string gitignoreContent = formatter.format(FileUtils::readFile(sgSourcesRootStr + "/Sources/SGCore/PluginsSystem/.references/.gitignore"));

        std::ofstream gitignoreStream(pluginDir + "/.gitignore");
        gitignoreStream << gitignoreContent;
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
                                   const std::string& localPluginPath,
                                   const std::vector<std::string>& entryArgs,
                                   PluginBuildType pluginBuildType)
{
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
            spdlog::warn("Warning: Plugin '{0}' (ver. {1}) has other path. Requested local load path: '{2}', plugin cached path: '{3}'.",
                         pluginName, loadedPlugin->m_plugin->m_version, localPluginPath, loadedPlugin->getPlugin()->m_path);
        }
    }
    else
    {
        loadedPlugin = MakeRef<PluginWrap>();
        
        std::string buildDir;
        switch(pluginBuildType)
        {
            case PBT_DEBUG:
                buildDir = "cmake-build-debug";
                break;
            case PBT_RELEASE:
                buildDir = "cmake-build-release";
                break;
        }
        
        std::string pluginDLPath = std::filesystem::path(localPluginPath + "/" + buildDir + "/" + pluginName + DL_POSTFIX).make_preferred().string();
        
        std::string dlErr;
        
        Ref<DynamicLibrary> pluginDL = MakeRef<DynamicLibrary>();
        pluginDL->load(pluginDLPath, dlErr);
        
        if(!pluginDL->getNativeHandler())
        {
            spdlog::error("Cannot load plugin '{0}' by path '{1}'. Error is: {2}", pluginName, pluginDLPath, dlErr);
            return nullptr;
        }
        
        std::string dlEntryErr;
        auto pluginEntry = pluginDL->loadFunction<Ref<IPlugin>()>("SGPluginMain", dlEntryErr);
        
        if(!pluginEntry)
        {
            spdlog::error("Cannot load plugin '{0}' main function by path '{1}'. Error is: {2}", pluginName, pluginDLPath, dlEntryErr);
            return nullptr;
        }
        
        loadedPlugin->m_plugin = pluginEntry();
        loadedPlugin->m_plugin->m_path = localPluginPath;
        std::cout << "LOADED PLUGIN: " << loadedPlugin->m_plugin << std::endl;
        loadedPlugin->m_plugin->onConstruct(entryArgs);
        std::cout << "ERROR HERE" << std::endl;
        loadedPlugin->m_pluginLib = pluginDL;
    }
    
    m_plugins.push_back(loadedPlugin);
    
    return loadedPlugin;
}

SGCore::Ref<SGCore::PluginWrap>
SGCore::PluginsManager::reloadPlugin(const std::string& pluginName,
                                     const std::vector<std::string>& entryArgs,
                                     PluginBuildType pluginBuildType)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(m_plugins.begin(), m_plugins.end(), [&pluginName](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName;
    });
    
    if(foundIt != m_plugins.end())
    {
        loadedPlugin = *foundIt;
        
        std::string buildDir;
        switch(pluginBuildType)
        {
            case PBT_DEBUG:
                buildDir = "cmake-build-debug";
                break;
            case PBT_RELEASE:
                buildDir = "cmake-build-release";
                break;
        }
        
        std::string dlErr;
        std::string dlEntryErr;
        
        std::string pluginSrcPath = loadedPlugin->m_plugin->m_path;
        std::string pluginDLPath = pluginSrcPath + sep + buildDir + sep + pluginName + DL_POSTFIX;
        
        loadedPlugin->m_plugin = nullptr;
        
        loadedPlugin->m_pluginLib->unload();
        loadedPlugin->m_pluginLib->load(pluginDLPath, dlErr);
        
        if(!loadedPlugin->m_pluginLib->getNativeHandler())
        {
            spdlog::error("Cannot reload plugin '{0}'. Error is: {1}", pluginName, dlErr);
            return nullptr;
        }
        
        // auto pluginEntry = std::function<Ref<IPlugin>()>((Ref<IPlugin>(*)()) dlsym(loadedPlugin->m_pluginLib->getNativeHandler(), "SGPluginMain"));
        auto pluginEntry = loadedPlugin->m_pluginLib->loadFunction<Ref<IPlugin>()>("SGPluginMain", dlEntryErr);
        
        if(!pluginEntry)
        {
            spdlog::error("Cannot reload plugin '{0}' main function. Error is: {1}", pluginName, dlEntryErr);
            return nullptr;
        }
        
        loadedPlugin->m_plugin = pluginEntry();
        loadedPlugin->m_plugin->m_path = pluginSrcPath;
        loadedPlugin->m_plugin->onConstruct(entryArgs);
        
        std::cout << "loadedPlugin: " << loadedPlugin->getPlugin() << std::endl;
        
        return loadedPlugin;
    }
    else
    {
        spdlog::error("Cannot reload plugin: The plugin '{0}' has not been loaded before.", pluginName);
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
