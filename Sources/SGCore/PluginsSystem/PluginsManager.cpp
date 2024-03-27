//
// Created by ilya on 19.03.24.
//

#include "PluginsManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGUtils/CrashHandler/Platform.h"

#include <filesystem>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "PluginWrap.h"

#ifdef PLATFORM_OS_LINUX
#include <dlfcn.h>
#endif

std::string SGCore::PluginsManager::createPluginProject(const std::string& projectPath,
                                                        const std::string& pluginName,
                                                        const std::string& cxxStandard)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;

    if(!std::filesystem::exists(CoreMain::m_pathToSungearEngineSources + sep + "FindSungearEngine.cmake"))
    {
        return "Error: Incorrect Sungear Engine sources directory!";
    }
    
    try
    {
        auto finalSGPath = CoreMain::m_pathToSungearEngineSources;
        if(std::filesystem::path::preferred_separator == L'\\')
        {
            finalSGPath = SGUtils::Utils::replaceAll(finalSGPath, R"(\)", R"(\\)");
        }

        const auto pluginDir = projectPath + sep + pluginName;
        const auto cmakeListsPath = pluginDir + sep + "CMakeLists.txt";
        const auto pluginSourcesDir = pluginDir + sep + "Sources";
        
        if(std::filesystem::exists(pluginDir) && !std::filesystem::is_empty(pluginDir))
        {
            return "Error: Project directory is not empty!";
        }
        
        if(cxxStandard != "98" && cxxStandard != "03" && cxxStandard != "11" && cxxStandard != "14" &&
           cxxStandard != "17" && cxxStandard != "20" && cxxStandard != "23" && cxxStandard != "26")
        {
            return "Error: Unknown CXX Standard!";
        }
        
        std::filesystem::create_directories(pluginDir);
        std::filesystem::create_directories(pluginSourcesDir);
        
        // ====================================== CMakeLists.txt
        
        std::string cmakeListsContent;
        cmakeListsContent += "cmake_minimum_required(VERSION 3.25)\n";
        cmakeListsContent += fmt::format("project({0})\n\n", pluginName);
        cmakeListsContent += fmt::format("set(CMAKE_CXX_STANDARD {0})\n", cxxStandard);
        cmakeListsContent += "set(CMAKE_SHARED_LIBRARY_PREFIX \"\")\n";
        cmakeListsContent += "if(${CMAKE_COMPILER_IS_GNUCXX})\n";
        cmakeListsContent += "\tset(CMAKE_CXX_FLAGS \"-g -rdynamic -fno-pie -no-pie\")\n";
        cmakeListsContent += "endif()\n\n";
        cmakeListsContent += "add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)\n";
        cmakeListsContent += "add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)\n\n";
        cmakeListsContent += "set(SG_INCLUDE_BULLET ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_PUGIXML ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_FREETYPE ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLM ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_ASSIMP ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_ENTT ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLFW ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_IMGUI ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_SPDLOG ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_STB ON)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLAD ON)\n\n";
        cmakeListsContent += fmt::format("set(SUNGEAR_ENGINE_DIR \"{0}\")\n", finalSGPath);
        cmakeListsContent += "list(APPEND CMAKE_MODULE_PATH ${SUNGEAR_ENGINE_DIR})\n\n";
        cmakeListsContent += "find_package(SungearEngine REQUIRED)\n\n";
        cmakeListsContent += "add_library(${PROJECT_NAME} " +
                             fmt::format(
                                     "SHARED Sources/PluginMain.h Sources/PluginMain.cpp Sources/{0}.h Sources/{1}.cpp)\n\n",
                                     pluginName,
                                     pluginName);
        cmakeListsContent += "target_include_directories(${PROJECT_NAME} PRIVATE ${SungearEngine_INCLUDE_DIRS})\n";
        cmakeListsContent += "target_link_libraries(${PROJECT_NAME} PRIVATE ${SungearEngine_LIBS})\n";
        
        std::ofstream cmakeListsStream(cmakeListsPath);
        cmakeListsStream << cmakeListsContent;
        
        // ====================================== PluginMain.h
        
        std::string upperPluginName = pluginName;
        std::transform(pluginName.begin(), pluginName.end(), upperPluginName.begin(), ::toupper);
        
        std::string pluginMainHDefine = fmt::format("{0}_PLUGINMAIN_H", upperPluginName);
        
        std::string pluginMainHContent;
        pluginMainHContent += fmt::format("#ifndef {0}\n", pluginMainHDefine);
        pluginMainHContent += fmt::format("#define {0}\n\n", pluginMainHDefine);
        pluginMainHContent += "#include <iostream>\n";
        pluginMainHContent += "#include <SGCore/Main/CoreGlobals.h>\n";
        pluginMainHContent += "#include <SGCore/PluginsSystem/PluginsManager.h>\n";
        pluginMainHContent += "#include <SGCore/PluginsSystem/IPlugin.h>\n\n";
        pluginMainHContent += fmt::format("#include \"{0}.h\"\n\n", pluginName);
        pluginMainHContent += fmt::format("static SGCore::Ref<{0}> s_{1}Instance;\n\n", pluginName, pluginName);
        pluginMainHContent += "extern \"C\" SGCore::Ref<SGCore::IPlugin> SGPluginMain();\n\n";
        pluginMainHContent += fmt::format("#endif // {0}\n", pluginMainHDefine);
        
        std::ofstream pluginMainHStream(pluginSourcesDir + sep + "PluginMain.h");
        pluginMainHStream << pluginMainHContent;
        
        // ====================================== PluginMain.cpp
        
        std::string pluginMainCPPContent;
        pluginMainCPPContent += "#include \"PluginMain.h\"\n\n";
        pluginMainCPPContent += "extern \"C\" SGCore::Ref<SGCore::IPlugin> SGPluginMain()\n";
        pluginMainCPPContent += "{\n";
        pluginMainCPPContent += fmt::format("\ts_{0}Instance = SGCore::MakeRef<{1}>();\n\n", pluginName, pluginName);
        pluginMainCPPContent += fmt::format("\treturn s_{0}Instance;\n", pluginName);
        pluginMainCPPContent += "}\n";
        
        std::ofstream pluginMainCPPStream(pluginSourcesDir + sep + "PluginMain.cpp");
        pluginMainCPPStream << pluginMainCPPContent;
        
        // ====================================== {pluginName}.h
        
        std::string pluginHContent;
        pluginHContent += fmt::format("#ifndef {0}_H\n", upperPluginName);
        pluginHContent += fmt::format("#define {0}_H\n\n", upperPluginName);
        pluginHContent += "#include <SGCore/PluginsSystem/IPlugin.h>\n\n";
        pluginHContent += fmt::format("struct {0} : public SGCore::IPlugin\n", pluginName);
        pluginHContent += "{\n";
        pluginHContent += fmt::format("\t~{0}() override = default;\n\n", pluginName);
        pluginHContent += "\tstd::string load(const std::vector<std::string>& args) override;\n";
        pluginHContent += "};\n\n";
        pluginHContent += fmt::format("#endif // {0}_H\n", upperPluginName);
        
        std::ofstream pluginHStream(pluginSourcesDir + sep + fmt::format("{0}.h", pluginName));
        pluginHStream << pluginHContent;
        
        // ====================================== {pluginName}.cpp
        
        std::string pluginCPPContent;
        pluginCPPContent += fmt::format("#include \"{0}.h\"\n\n", pluginName);
        pluginCPPContent += fmt::format("std::string {0}::load(const std::vector<std::string>& args)\n", pluginName);
        pluginCPPContent += "{\n";
        pluginCPPContent += fmt::format("\tm_name = \"{0}\";\n", pluginName);
        pluginCPPContent += "\tm_version = \"1.0.0\";\n\n";
        pluginCPPContent += "\t// No error.\n";
        pluginCPPContent += "\treturn \"\";\n";
        pluginCPPContent += "}\n\n";
        
        std::ofstream pluginCPPStream(pluginSourcesDir + sep + fmt::format("{0}.cpp", pluginName));
        pluginCPPStream << pluginCPPContent;
    }
    catch(const std::filesystem::filesystem_error& err)
    {
        return std::string("Error: ") + err.what();
    }
    
    return "";
}

SGCore::Ref<SGCore::PluginWrap>
SGCore::PluginsManager::loadPlugin(const std::string& pluginName,
                                   const std::string& pluginVersion,
                                   const std::string& localPluginPath,
                                   const std::vector<std::string>& entryArgs,
                                   PluginBuildType pluginBuildType)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(s_plugins.begin(), s_plugins.end(), [&pluginName, &pluginVersion](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName && pluginWrap->getPlugin()->m_version == pluginVersion;
    });
    
    if(foundIt != s_plugins.end())
    {
        loadedPlugin = *foundIt;
        
        if(loadedPlugin->getPlugin()->m_localPath != localPluginPath)
        {
            spdlog::warn("Warning: Plugin '{0}' (ver. {1}) has other path. Requested local load path: '{2}', plugin cached path: '{3}'.",
                         pluginName,pluginVersion, localPluginPath, loadedPlugin->getPlugin()->m_localPath);
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
        
        std::string pluginDLPath = localPluginPath + sep + buildDir + sep + pluginName + DL_POSTFIX;
        
        std::string dlErr;
        
        Ref<DynamicLibrary> pluginDL = MakeRef<DynamicLibrary>();
        pluginDL->load(pluginDLPath, dlErr);
        
        if(!pluginDL->getNativeHandler())
        {
            spdlog::error("Cannot load plugin '{0}' (ver. {1}) by path '{2}'. Error is: {3}", pluginName, pluginVersion, pluginDLPath, dlErr);
            return nullptr;
        }
        
        std::string dlEntryErr;
        auto pluginEntry = pluginDL->loadFunction<Ref<IPlugin>()>("SGPluginMain", dlEntryErr);
        
        if(!pluginEntry)
        {
            spdlog::error("Cannot load plugin '{0}' (ver. {1}) main function by path '{2}'. Error is: {3}", pluginName, pluginVersion, pluginDLPath, dlEntryErr);
            return nullptr;
        }
        
        loadedPlugin->m_plugin = pluginEntry();
        loadedPlugin->m_pluginLib = pluginDL;
        loadedPlugin->m_plugin->load(entryArgs);
        loadedPlugin->m_plugin->m_localPath = localPluginPath;
    }
    
    s_plugins.push_back(loadedPlugin);
    
    return loadedPlugin;
}

#include <dlfcn.h>

SGCore::Ref<SGCore::PluginWrap>
SGCore::PluginsManager::reloadPlugin(const std::string& pluginName,
                                     const std::string& pluginVersion,
                                     const std::vector<std::string>& entryArgs,
                                     PluginBuildType pluginBuildType)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(s_plugins.begin(), s_plugins.end(), [&pluginName, &pluginVersion](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName && pluginWrap->getPlugin()->m_version == pluginVersion;
    });
    
    if(foundIt != s_plugins.end())
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
        
        std::string pluginSrcPath = loadedPlugin->m_plugin->m_localPath;
        std::string pluginDLPath = pluginSrcPath + sep + buildDir + sep + pluginName + DL_POSTFIX;
        
        loadedPlugin->m_plugin = nullptr;
        
        loadedPlugin->m_pluginLib->unload();
        loadedPlugin->m_pluginLib->load(pluginDLPath, dlErr);
        
        if(!loadedPlugin->m_pluginLib->getNativeHandler())
        {
            spdlog::error("Cannot reload plugin '{0}' (ver. {1}). Error is: {2}", pluginName, pluginVersion, dlErr);
            return nullptr;
        }
        
        // auto pluginEntry = std::function<Ref<IPlugin>()>((Ref<IPlugin>(*)()) dlsym(loadedPlugin->m_pluginLib->getNativeHandler(), "SGPluginMain"));
        auto pluginEntry = loadedPlugin->m_pluginLib->loadFunction<Ref<IPlugin>()>("SGPluginMain", dlEntryErr);
        
        if(!pluginEntry)
        {
            spdlog::error("Cannot reload plugin '{0}' (ver. {1}) main function. Error is: {2}", pluginName, pluginVersion, dlEntryErr);
            return nullptr;
        }
        
        loadedPlugin->m_plugin = pluginEntry();
        loadedPlugin->m_plugin->m_localPath = pluginSrcPath;
        loadedPlugin->m_plugin->load(entryArgs);
        
        std::cout << "loadedPlugin: " << loadedPlugin->getPlugin() << std::endl;
        
        return loadedPlugin;
    }
    else
    {
        spdlog::error("Cannot reload plugin: The plugin '{0}' (ver. {1}) has not been loaded before.", pluginName, pluginVersion);
    }
    
    return nullptr;
}
