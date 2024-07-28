#include "ToolchainType.h"
#include "Toolchain.h"
#include <SGCore/Exceptions/FileNotFoundException.h>

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
        std::string buildToolTest = SGCore::Utils::consoleExecute(
                fmt::format("\"{0}\"",
                            SGCore::Utils::toUTF8(m_cmakePath.u16string()))
        );

        // BUILD TOOL AS NINJA. TEST PASSED
        if(buildToolTest.contains("ninja") && buildToolTest.contains("error"))
        {
            m_buildToolVersion = SGCore::Utils::consoleExecute(
                    fmt::format("\"{0}\" --version",
                                SGCore::Utils::toUTF8(m_cmakePath.u16string()))
            );
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

std::string SGE::Toolchain::getCMakeVersion() const
{
    return m_cmakeVersion;
}

std::string SGE::Toolchain::getBuildToolVersion() const
{
    return m_buildToolVersion;
}
