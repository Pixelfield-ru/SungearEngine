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
    const char* sgSourcesRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    if(!sgSourcesRoot || !std::filesystem::exists(std::string(sgSourcesRoot) + "/cmake/SungearEngineInclude.cmake"))
    {
        return "Error: Incorrect Sungear Engine sources directory!\nCheck if the 'SUNGEAR_SOURCES_ROOT' environment variable\nexists and correct.";
    }
    
    try
    {
        auto finalSGPath = CoreMain::m_pathToSungearEngineSDKSources;
        if(std::filesystem::path::preferred_separator == L'\\')
        {
            finalSGPath = SGUtils::Utils::replaceAll(finalSGPath, R"(\)", R"(\\)");
        }

        const auto pluginDir = projectPath + "/" + pluginName;
        const auto cmakeListsPath = pluginDir + "/CMakeLists.txt";
        const auto pluginSourcesDir = pluginDir + "/Sources";
        
        if(std::filesystem::exists(pluginDir) && !std::filesystem::is_empty(pluginDir))
        {
            return "Error: Project directory is not empty!";
        }
        
        if(cxxStandard != "C++98" && cxxStandard != "C++03" && cxxStandard != "C++11" && cxxStandard != "C++14" &&
           cxxStandard != "C++17" && cxxStandard != "C++20" && cxxStandard != "C++23" && cxxStandard != "C++26")
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
        cmakeListsContent += "\tset(CMAKE_CXX_FLAGS \"-g -rdynamic -fno-pie -no-pie -fno-gnu-unique\")\n";
        cmakeListsContent += "endif()\n\n";
        
        cmakeListsContent += "add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE)\n";
        cmakeListsContent += "add_definitions(-DGLM_ENABLE_EXPERIMENTAL)\n";
        cmakeListsContent += "add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)\n\n";
        
        cmakeListsContent += "set(SG_INCLUDE_BULLET OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_PUGIXML OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_FREETYPE OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLM OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_ASSIMP OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_ENTT OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLFW OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_IMGUI OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_SPDLOG OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_STB OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLAD OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_OPENAL OFF)\n";
        cmakeListsContent += "set(SG_INCLUDE_GLAZE OFF)\n\n";
        
        cmakeListsContent += "include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)\n\n";
        
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
        
        pluginMainHContent += "extern \"C\" SGCore::Ref<SGCore::IPlugin> SGPluginMain();\n\n";
        
        pluginMainHContent += fmt::format("#endif // {0}\n", pluginMainHDefine);
        
        std::ofstream pluginMainHStream(pluginSourcesDir + "/PluginMain.h");
        pluginMainHStream << pluginMainHContent;
        
        // ====================================== PluginMain.cpp
        
        std::string pluginMainCPPContent;
        pluginMainCPPContent += "#include \"PluginMain.h\"\n\n";
        
        pluginMainCPPContent += "extern \"C\" SGCore::Ref<SGCore::IPlugin> SGPluginMain()\n";
        pluginMainCPPContent += "{\n";
        pluginMainCPPContent += fmt::format("\treturn {0}::getInstance();\n", pluginName);
        pluginMainCPPContent += "}\n";
        
        std::ofstream pluginMainCPPStream(pluginSourcesDir + "/PluginMain.cpp");
        pluginMainCPPStream << pluginMainCPPContent;
        
        // ====================================== {pluginName}.h
        
        std::string pluginHContent;
        pluginHContent += fmt::format("#ifndef {0}_H\n", upperPluginName);
        pluginHContent += fmt::format("#define {0}_H\n\n", upperPluginName);
        
        pluginHContent += "#include <SGCore/PluginsSystem/IPlugin.h>\n";
        pluginHContent += "#include <SGUtils/Utils.h>\n";
        pluginHContent += "#include <SGCore/Main/CoreMain.h>\n\n";
        
        pluginHContent += fmt::format("struct {0} : public SGCore::IPlugin\n", pluginName);
        pluginHContent += "{\n";
        pluginHContent += fmt::format("\t~{0}() override = default;\n\n", pluginName);
        
        pluginHContent += "\tstd::string onConstruct(const std::vector<std::string>& args) override;\n\n";
        
        pluginHContent += fmt::format("\tSG_NOINLINE static SGCore::Ref<{0}> getInstance() noexcept;\n", pluginName);
        pluginHContent += "private:\n";
        pluginHContent += fmt::format("\tstatic inline SGCore::Ref<{0}> s_{1}Instance = SGCore::MakeRef<{2}>();\n", pluginName, pluginName, pluginName);
        pluginHContent += "};\n\n";
        
        pluginHContent += fmt::format("#endif // {0}_H\n", upperPluginName);
        
        std::ofstream pluginHStream(pluginSourcesDir + "/" + fmt::format("{0}.h", pluginName));
        pluginHStream << pluginHContent;
        
        // ====================================== {pluginName}.cpp
        
        std::string pluginCPPContent;
        pluginCPPContent += fmt::format("#include \"{0}.h\"\n\n", pluginName);
        
        pluginCPPContent += fmt::format("std::string {0}::onConstruct(const std::vector<std::string>& args)\n", pluginName);
        pluginCPPContent += "{\n";
        pluginCPPContent += fmt::format("\tm_name = \"{0}\";\n", pluginName);
        pluginCPPContent += "\tm_version = \"1.0.0\";\n\n";
        
        pluginCPPContent += "\t// No error.\n";
        pluginCPPContent += "\treturn \"\";\n";
        pluginCPPContent += "}\n\n";
        
        pluginCPPContent += fmt::format("SGCore::Ref<{0}> {0}::getInstance() noexcept\n", pluginName, pluginName);
        pluginCPPContent += "{\n";
        pluginCPPContent += fmt::format("\treturn s_{0}Instance\n", pluginName);
        pluginCPPContent += "}\n\n";
        
        std::ofstream pluginCPPStream(pluginSourcesDir + "/" + fmt::format("{0}.cpp", pluginName));
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
                                   const std::string& localPluginPath,
                                   const std::vector<std::string>& entryArgs,
                                   PluginBuildType pluginBuildType)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(s_plugins.begin(), s_plugins.end(), [&pluginName](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName;
    });
    
    if(foundIt != s_plugins.end())
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
    
    s_plugins.push_back(loadedPlugin);
    
    return loadedPlugin;
}

SGCore::Ref<SGCore::PluginWrap>
SGCore::PluginsManager::reloadPlugin(const std::string& pluginName,
                                     const std::vector<std::string>& entryArgs,
                                     PluginBuildType pluginBuildType)
{
    const auto& sep = (char) std::filesystem::path::preferred_separator;
    
    Ref<PluginWrap> loadedPlugin;
    
    auto foundIt = std::find_if(s_plugins.begin(), s_plugins.end(), [&pluginName](const Ref<PluginWrap>& pluginWrap) {
        return pluginWrap->getPlugin()->m_name == pluginName;
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
