#include "ToolchainType.h"
#include "Toolchain.h"
#include "SungearEngineEditor.h"
#include "Views/DialogWindowsManager.h"
#include <SGCore/Exceptions/FileNotFoundException.h>

#include <iostream>
#include <SGCore/Utils/Utils.h>
#include <SGCore/Utils/FileUtils.h>
#include <SGCore/PluginsSystem/DynamicLibrary.h>
#include <SGCore/Logger/Logger.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/Utils/CMakeUtils.h>

//
// Created by Ilya on 16.07.2024.
//

SGE::ToolchainType SGE::Toolchain::getType() const
{
    return m_type;
}

std::filesystem::path SGE::Toolchain::getPath() const
{
    return m_path;
}

void SGE::Toolchain::setPath(const std::filesystem::path& path)
{
    m_path = path;

    configurate();
}

std::filesystem::path SGE::Toolchain::getCMakePath() const
{
    return m_cmakePath;
}

void SGE::Toolchain::setCMakePath(const std::filesystem::path& cmakePath)
{
    m_cmakeVersion = "";

    m_cmakePath = cmakePath;

    if(m_cmakePath.empty()) return;

    // CMAKE TEST
    if(std::filesystem::exists(m_cmakePath))
    {
        std::string cmakeTestResult = SGCore::Utils::consoleExecute(
                fmt::format("\"{0}\" --version",
                            SGCore::Utils::toUTF8(m_cmakePath.u16string()))
        );

        // SKIPPING FIRST TWO LINES OF OUTPUT (THIS LINES PROVIDES COMMAND THAT WAS PASSED TO CONSOLE)
        std::vector<std::string> lines;
        SGCore::Utils::splitString(cmakeTestResult, '\n', lines);
        if(lines.size() < 3)
        {
            throw std::exception("Invalid CMake");
        }
        std::vector<std::string> thirdLineWords;
        SGCore::Utils::splitString(lines[2], ' ', thirdLineWords);

        if(thirdLineWords.size() >= 3 && thirdLineWords[0] == "cmake" && thirdLineWords[1] == "version")
        {
            m_cmakeVersion = thirdLineWords[2];
        }
        else
        {
            throw std::exception("Invalid CMake");
        }
    }
    else
    {
        throw SGCore::FileNotFoundException(m_cmakePath, "Invalid CMake path");
    }

    // configurate();
}

std::filesystem::path SGE::Toolchain::getBuildToolPath() const
{
    return m_buildToolPath;
}

void SGE::Toolchain::setBuildToolPath(const std::filesystem::path& buildToolPath)
{
    m_buildToolVersion = "";

    m_buildToolPath = buildToolPath;

    if(m_buildToolPath.empty()) return;

    // CMAKE TEST
    if(std::filesystem::exists(m_buildToolPath))
    {
        // BUILD TOOL AS NINJA
        if(SGCore::Utils::toUTF8(m_buildToolPath.u16string()).contains("ninja.exe"))
        {
            std::string versionCommandResult = SGCore::Utils::consoleExecute(
                    fmt::format("\"{0}\" --version",
                                SGCore::Utils::toUTF8(m_buildToolPath.u16string()))
            );

            // SKIPPING FIRST TWO LINES OF OUTPUT (THIS LINES PROVIDES COMMAND THAT WAS PASSED TO CONSOLE)
            std::vector<std::string> lines;
            SGCore::Utils::splitString(versionCommandResult, '\n', lines);
            if(lines.size() < 3)
            {
                throw std::exception("Invalid CMake");
            }
            std::vector<std::string> thirdLineWords;
            SGCore::Utils::splitString(lines[2], ' ', thirdLineWords);

            if(!thirdLineWords.empty())
            {
                m_buildToolVersion = "Ninja " + thirdLineWords[0];
            }
        }
        else
        {
            throw std::exception("Invalid build tool");
        }
    }
    else
    {
        throw SGCore::FileNotFoundException(m_cmakePath, "Invalid build tool path");
    }

    // configurate();
}

/*std::filesystem::path SGE::Toolchain::getCCompilerPath() const
{
    return m_CCompilerPath;
}

void SGE::Toolchain::setCCompilerPath(const std::filesystem::path& CCompilerPath)
{
    m_CCompilerPath = CCompilerPath;

    configurate();
}

std::filesystem::path SGE::Toolchain::getCXXCompilerPath() const
{
    return m_CXXCompilerPath;
}

void SGE::Toolchain::setCXXCompilerPath(const std::filesystem::path& CXXCompilerPath)
{
    m_CXXCompilerPath = CXXCompilerPath;

    configurate();
}*/

void SGE::Toolchain::configurate()
{
    switch (m_type)
    {
        case ToolchainType::VISUAL_STUDIO:
        {
            #ifdef PLATFORM_OS_LINUX
            throw std::runtime_error("Toolchain unsupported combination: host platform Linux and toolchain type is Visual Studio");
            #endif

            if (!std::filesystem::exists(m_path))
            {
                throw SGCore::FileNotFoundException(m_path, "Path of toolchain does not exist");
            }

            // auto vcvarsallbatPath = SGCore::FileUtils::findFile(m_path, "vcvarsall.bat");

            // FINDING cmake.exe
            if(m_cmakePath.empty())
            {
                std::filesystem::path cmakePath = SGCore::Utils::consoleExecute("where cmake.exe");
                if (cmakePath.filename() != "cmake.exe")
                {
                    cmakePath = SGCore::FileUtils::findFile(m_path, "cmake.exe");
                }

                m_cmakePath = cmakePath;

                if(!std::filesystem::exists(cmakePath))
                {
                    throw SGCore::FileNotFoundException(m_cmakePath,
                                                        "Can not find path to cmake.exe in environment variables or in toolchain files");
                }
            }

            // ===================================================

            if(m_buildToolPath.empty())
            {
                std::filesystem::path buildToolPath = SGCore::Utils::consoleExecute("where ninja.exe");
                if (buildToolPath.filename() != "ninja.exe")
                {
                    buildToolPath = SGCore::FileUtils::findFile(m_path, "ninja.exe");
                }

                if (!std::filesystem::exists(buildToolPath))
                {
                    throw SGCore::FileNotFoundException(buildToolPath,
                                                        "Can not find path to build tool in environment variables or in toolchain files");
                }

                m_buildToolPath = buildToolPath;
            }

            break;
        }
        case ToolchainType::MINGW:
            break;
        case ToolchainType::UNKNOWN:
            break;
    }
}

void SGE::Toolchain::buildProject(const std::filesystem::path& pathToProjectRoot, const std::string& cmakePresetName)
{
    // Configuring incomplete

    const std::filesystem::path cmakePresetsPath = pathToProjectRoot / "CMakePresets.json";

    const std::string projectName = SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string());

    if(!std::filesystem::exists(cmakePresetsPath))
    {
        LOG_E(SGEDITOR_TAG,
              "Error while building project '{}'. File CMakePresets.json was not found in project directory '{}'",
              projectName, SGCore::Utils::toUTF8(pathToProjectRoot.u16string()));

        return;
    }

    SGCore::CMakePresetsFileInfo projectCMakePresetsFileInfo(cmakePresetsPath);
    const SGCore::CMakePresetsFileInfo::Preset* projectPreset = projectCMakePresetsFileInfo.getPreset(cmakePresetName);
    if(!projectPreset)
    {
        LOG_E(SGEDITOR_TAG,
              "Error while building project '{}'. Preset '{}' was not found in CMakePreset.json file of project.",
              projectName, cmakePresetName);

        return;
    }

    if(projectPreset->m_binaryDir.empty())
    {
        LOG_E(SGEDITOR_TAG, "Error while building project '{}'. Preset '{}' was not found in CMakePresets.json",
              projectName, cmakePresetName);

        return;
    }

    // PRESET AND ITS BINARY DIR WERE SUCCESSFULLY FOUND

    m_currentBuildingPresetBinaryDir = projectPreset->m_binaryDir;

    m_builtDynamicLibraryPath = pathToProjectRoot;
    m_builtDynamicLibraryPath += '/' + projectPreset->m_binaryDir + '/' + projectName + DL_POSTFIX;
    m_projectName = projectName;

    LOG_I(PROJECT_BUILD_TAG, "Building project '{}' using Visual Studio toolchain: found binary dir '{}'.\n",
          SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
          projectPreset->m_binaryDir.c_str());
}

std::string SGE::Toolchain::getCMakeVersion() const
{
    return m_cmakeVersion;
}

std::string SGE::Toolchain::getBuildToolVersion() const
{
    return m_buildToolVersion;
}

void SGE::Toolchain::ProjectSpecific::setCurrentCMakePreset(const std::string& presetName) noexcept
{
    m_currentCMakePreset = presetName;
}

std::string SGE::Toolchain::ProjectSpecific::getCurrentCMakePreset() noexcept
{
    return m_currentCMakePreset;
}

void SGE::Toolchain::ProjectSpecific::buildProject(const SGCore::Ref<SGE::Toolchain>& toolchain) noexcept
{
    auto currentEditorProject = SungearEngineEditor::getInstance()->m_currentProject;

    if(!SungearEngineEditor::checkSungearEngineEnvironmentRootPathValidity("Can not build Sungear Engine: missing environment variable 'SUNGEAR_SOURCES_ROOT'."))
    {
        return;
    }

    const std::filesystem::path sungearPluginsPathStr = SungearEngineEditor::getSungearEngineRootPath() / "Plugins";

    const bool isPresetsEquals = m_currentCMakePreset == SG_BUILD_PRESET;

    const std::string projectName = currentEditorProject->m_pluginProject.m_name;
    toolchain->onProjectBuiltSynchronized = [projectName](const Toolchain::ProjectBuildOutput& buildOutput) {
        auto projectBuiltDialogWindow = DialogWindowsManager::createOneButtonWindow("Project Build", "OK");
        projectBuiltDialogWindow->onCustomBodyRenderListener = [projectName]() {
            ImGui::SameLine();
            ImGui::TextWrapped(fmt::format("The project '{}' has been built. "
                                           "Check the logs for more details. "
                                           "Also, if you or someone else has changed the source code of plugins or Sungear Engine, it is recommended to exit the engine, build Sungear Engine and plugins source code of wich was changed and then start the engine again..", projectName).c_str());
        };
        DialogWindowsManager::addDialogWindow(projectBuiltDialogWindow);
    };

    toolchain->onProjectBuilt = [currentEditorProject,
            toolchainPtr = toolchain.get(),
            sungearPluginsPathStr,
            isPresetsEquals,
            projectName]
            (const Toolchain::ProjectBuildOutput& buildOutput)
    {
        // disable creating new thread because currently we are not in main thread (already parallel build)
        toolchainPtr->m_doInBackground = false;
        // copying toolchain for building plugins and project
        auto toolchainCopy = SGCore::Ref<Toolchain>(toolchainPtr->copy());
        toolchainCopy->onProjectBuilt = nullptr;
        toolchainCopy->onProjectBuiltSynchronized = nullptr;

        // building meta info project
        toolchainCopy->buildProject(currentEditorProject->m_pluginProject.m_pluginPath / "MetaInfo",
                                    m_currentCMakePreset);

        toolchainCopy->onProjectBuilt = [&currentEditorProject, projectName]
                (const Toolchain::ProjectBuildOutput& buildOutput) {
            // after building meta info project we are loading the dynamic library of meta project and setting path to current user project
            auto metaInfoProjectDL = SGCore::MakeRef<SGCore::DynamicLibrary>();

            std::string metaInfoProjectDLLoadOutput;
            metaInfoProjectDL->load(
                    currentEditorProject->m_pluginProject.m_pluginPath / "MetaInfo" / buildOutput.m_binaryDir /
                    (std::string("MetaInfo") + DL_POSTFIX),
                    metaInfoProjectDLLoadOutput
            );

            if(!metaInfoProjectDLLoadOutput.empty())
            {
                // printing error of meta info project loading
                LOG_E(SGEDITOR_TAG, "Important! Can not load meta info project of project '{}'. Error is: {}",
                      projectName, metaInfoProjectDLLoadOutput);
            }
            else
            {
                LOG_D(SGEDITOR_TAG, "Meta info project of project '{}' was successfully loaded.",
                      projectName);

                std::string symbolsLoadingErr;

                // loading function 'addMetaInfo' to add user`s meta info
                std::function<void()> metaInfoEntry = metaInfoProjectDL->loadSymbol<void()>("addMetaInfo", symbolsLoadingErr);
                // loading myProjectPath to set pointer to current user project path
                auto** currentProjectPath = metaInfoProjectDL->loadSymbol<std::filesystem::path*>("myProjectPath", symbolsLoadingErr);

                if(!symbolsLoadingErr.empty())
                {
                    LOG_E(SGEDITOR_TAG, "Important! Can not load meta info project symbols (symbols 'addMetaInfo' and 'myProjectPath') of project '{}'. Error is: {}",
                          projectName, symbolsLoadingErr);
                }
                else
                {
                    *currentProjectPath = &currentEditorProject->m_pluginProject.m_pluginPath;
                    metaInfoEntry();

                    currentEditorProject->m_metaInfoProjectEntryPoint = metaInfoEntry;

                    LOG_D(SGEDITOR_TAG, "Meta info project symbols of project '{}' were successfully set.",
                          projectName);
                }
            }

            currentEditorProject->m_metaInfoProjectDL = metaInfoProjectDL;
        };

        // building all plugins
        for(const auto& dirEntry : std::filesystem::directory_iterator(sungearPluginsPathStr))
        {
            // building plugin after building the Sungear Engine
            toolchainCopy->buildProject(dirEntry.path(), m_currentCMakePreset);
        }

        // IF CURRENT ENGINE INSTANCE IS BUILT BY THE SAME PRESET
        if(isPresetsEquals)
        {
            toolchainCopy->onProjectBuilt = [&currentEditorProject, toolchainPtr, projectName]
                    (const Toolchain::ProjectBuildOutput& buildOutput)
            {
                std::string projectDynamicLibraryLoadError;

                currentEditorProject->m_loadedPlugin = SGCore::PluginsManager::loadPlugin(
                        SGCore::Utils::toUTF8(buildOutput.m_projectName.u16string()),
                        currentEditorProject->m_pluginProject.m_pluginPath,
                        {},
                        buildOutput.m_binaryDir
                );

                // if plugin was successfully loaded
                if(currentEditorProject->m_loadedPlugin)
                {
                    std::string userProjectEntriesLoadingErr;
                    currentEditorProject->m_editorHelperEntryPoint = currentEditorProject->m_loadedPlugin->getPluginLib()->loadSymbol<void()>(
                            "editorGeneratedCodeEntry", userProjectEntriesLoadingErr
                    );
                    currentEditorProject->m_editorHelperExitPoint = currentEditorProject->m_loadedPlugin->getPluginLib()->loadSymbol<void()>(
                            "editorGeneratedCodeExit", userProjectEntriesLoadingErr
                    );

                    if(!userProjectEntriesLoadingErr.empty())
                    {
                        LOG_E(SGEDITOR_TAG,
                              "Important! Can not load project' symbols (symbols 'editorGeneratedCodeEntry' and 'editorGeneratedCodeExit') of project '{}'. This project will not work correct. Error is: {}",
                              projectName,
                              userProjectEntriesLoadingErr);
                    }
                    else
                    {
                        if(currentEditorProject->m_editorHelperEntryPoint)
                        {
                            // calling editor helper entry point
                            currentEditorProject->m_editorHelperEntryPoint();
                        }
                    }
                }
            };
        }
        else
        {
            // TODO: MAKE MIGRATION TO OTHER ENGINE BUILD DIALOG
            buildOutput.m_binaryDir;
        }

        // building new project after building the Sungear Engine
        toolchainCopy->buildProject(currentEditorProject->m_pluginProject.m_pluginPath, m_currentCMakePreset);

        // restore state of m_doInBackground to use parallel building in next project builds
        toolchainPtr->m_doInBackground = true;
    };
    // building the Sungear Engine
    toolchain->buildProject(SungearEngineEditor::getSungearEngineRootPath(), m_currentCMakePreset);
}

void SGE::Toolchain::ProjectSpecific::buildProject() noexcept
{
    if(!EngineSettings::getInstance()->getToolchains().empty())
    {
        buildProject(EngineSettings::getInstance()->getToolchains()[0]);
    }
    else
    {
        // showing warning dialog that no toolchains were added
        auto projectBuiltDialogWindow = DialogWindowsManager::createTwoButtonsWindow("Project Build", "Create Toolchain", "OK");
        // getting "Create Toolchain" button
        Button* createToolchainButton = projectBuiltDialogWindow->tryGetButton(0);
        createToolchainButton->onClicked = [](Button& self, SGCore::ImGuiWrap::IView* parent) {
            // opening the engine settings window in toolchains tab
            auto engineSettingsView = SungearEngineEditor::getInstance()->getMainView()->getTopToolbarView()->m_engineSettingsView;
            engineSettingsView->setActive(true);
            TreeNode engineSettingsToolchainsNode;
            if(engineSettingsView->m_settingsTree.tryCopyGetTreeNodeRecursively("Toolchains", &engineSettingsToolchainsNode))
            {
                engineSettingsToolchainsNode.openBranchAndSelectThis();
            }

            // closing the dialog window
            parent->setActive(false);
        };
        projectBuiltDialogWindow->m_level = SGCore::Logger::Level::LVL_WARN;
        projectBuiltDialogWindow->onCustomBodyRenderListener = []() {
            ImGui::SameLine();
            ImGui::TextWrapped(fmt::format("The project '{}' cannot be built: no toolchain has been added. "
                                           "Go to the <Engine Settings - Development and Support - Toolchains> and add your first toolchain.",
                                           SungearEngineEditor::getInstance()->m_currentProject->m_pluginProject.m_name).c_str());
        };
        DialogWindowsManager::addDialogWindow(projectBuiltDialogWindow);
    }
}
