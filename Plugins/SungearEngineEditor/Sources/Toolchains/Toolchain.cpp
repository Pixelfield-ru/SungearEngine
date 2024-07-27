#include "ToolchainType.h"
#include "Toolchain.h"

#include <iostream>
#include <SGCore/Utils/Utils.h>
#include <SGCore/Utils/FileUtils.h>

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
    m_cmakePath = cmakePath;

    configurate();
}

std::filesystem::path SGE::Toolchain::getBuildToolPath() const
{
    return m_buildToolPath;
}

void SGE::Toolchain::setBuildToolPath(const std::filesystem::path& buildToolPath)
{
    m_buildToolPath = buildToolPath;

    configurate();
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
                throw std::runtime_error(fmt::format("Path of toolchain {0} does not exist",
                                                     SGCore::Utils::toUTF8(m_path.u16string())));
            }

            // auto vcvarsallbatPath = SGCore::FileUtils::findFile(m_path, "vcvarsall.bat");

            // FINDING cmake.exe

            std::filesystem::path cmakePath = SGCore::Utils::consoleExecute("where cmake.exe");
            if(cmakePath.filename() != "cmake.exe")
            {
                cmakePath = SGCore::FileUtils::findFile(m_path, "cmake.exe");
            }

            m_cmakePath = cmakePath;

            if(!std::filesystem::exists(cmakePath))
            {
                throw std::runtime_error("Can not find path to cmake.exe in environment variables or in toolchain files");
            }

            // ===================================================

            std::filesystem::path buildToolPath = SGCore::Utils::consoleExecute("where ninja.exe");
            if(buildToolPath.filename() != "ninja.exe")
            {
                buildToolPath = SGCore::FileUtils::findFile(m_path, "ninja.exe");
            }

            if(!std::filesystem::exists(buildToolPath))
            {
                throw std::runtime_error("Can not find path to build tool in environment variables or in toolchain files");
            }

            m_buildToolPath = buildToolPath;

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

    auto cmakePresetsPath = pathToProjectRoot;
    cmakePresetsPath += "/CMakePresets.json";

    if(!std::filesystem::exists(cmakePresetsPath))
    {
        throw std::runtime_error(fmt::format("Error while building project '{0}'. File CMakePresets.json was not found in project directory '{1}'",
                                             SGCore::Utils::toUTF8(
                                                     pathToProjectRoot.filename().u16string()),
                                             SGCore::Utils::toUTF8(
                                                     pathToProjectRoot.u16string())));
    }

    rapidjson::Document cmakePresetsDocument;
    cmakePresetsDocument.Parse(SGCore::FileUtils::readFile(cmakePresetsPath).c_str());

    if(!cmakePresetsDocument.HasMember("configurePresets"))
    {
        throw std::runtime_error(fmt::format("Error while building project '{0}'. Section 'configurePresets' was not found in CMakePresets.json",
                                             SGCore::Utils::toUTF8(
                                                     pathToProjectRoot.filename().u16string()),
                                             SGCore::Utils::toUTF8(
                                                     pathToProjectRoot.u16string())));
    }

    std::string cmakePresetBinaryDir;

    auto& presets = cmakePresetsDocument["configurePresets"];

    for(std::size_t i = 0; i < presets.Size(); ++i)
    {
        auto& currentPreset = presets[i];

        if(!currentPreset.HasMember("name"))
        {
            throw std::runtime_error(fmt::format(
                    "Error while building project '{0}'. Section 'name' was not found in one of presets provided in CMakePresets.json file",
                    SGCore::Utils::toUTF8(
                            pathToProjectRoot.filename().u16string())));
        }

        std::string currentPresetName = currentPreset["name"].GetString();
        if(currentPresetName != cmakePresetName)
        {
            continue;
        }

        if(!currentPreset.HasMember("binaryDir"))
        {
            throw std::runtime_error(fmt::format(
                    "Error while building project '{0}'. Section 'binaryDir' was not found in preset '{1}' provided in CMakePresets.json file",
                    SGCore::Utils::toUTF8(
                            pathToProjectRoot.filename().u16string()),
                    currentPresetName));
        }

        cmakePresetBinaryDir = currentPreset["binaryDir"].GetString();
    }

    if(cmakePresetBinaryDir.empty())
    {
        throw std::runtime_error(fmt::format(
                "Error while building project '{0}'. Preset '{1}' was not found in CMakePresets.json",
                SGCore::Utils::toUTF8(
                        pathToProjectRoot.filename().u16string()),
                cmakePresetName));
    }

    // PRESET AND ITS BINARY DIR WERE SUCCESSFULLY FOUND

    m_currentBuildingPresetBinaryDir = cmakePresetBinaryDir;

    std::printf("Building project '%s' using Visual Studio toolchain: found binary dir '%s'\n",
                SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
                cmakePresetBinaryDir.c_str());
}
