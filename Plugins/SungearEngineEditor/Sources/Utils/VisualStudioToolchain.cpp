//
// Created by Ilya on 16.07.2024.
//

#include "VisualStudioToolchain.h"

#include <SGCore/Utils/Utils.h>
#include <SGCore/Utils/FileUtils.h>

std::string SGE::VCArchTypeToString(const SGE::VCArchType& archType) noexcept
{
    switch(archType)
    {
        case VCArchType::X86:
            return "x86";
        case VCArchType::AMD64:
            return "amd64";
        case VCArchType::X86_AMD64:
            return "x86_amd64";
        case VCArchType::X86_ARM:
            return "x86_arm";
        case VCArchType::X86_ARM64:
            return "x86_arm64";
        case VCArchType::AMD64_X86:
            return "amd64_x86";
        case VCArchType::AMD64_ARM:
            return "amd64_arm";
        case VCArchType::AMD64_ARM64:
            return "amd64_arm64";
    }

    return "";
}

std::string SGE::VCPlatformTypeToString(const SGE::VCPlatformType& platformType) noexcept
{
    switch(platformType)
    {
        case VCPlatformType::EMPTY:
            return "";
        case VCPlatformType::STORE:
            return "store";
        case VCPlatformType::UWP:
            return "uwp";
    }

    return "";
}

SGE::VisualStudioToolchain::VisualStudioToolchain()
{
    m_type = ToolchainType::VISUAL_STUDIO;
}

void SGE::VisualStudioToolchain::buildProject(const std::filesystem::path& pathToProjectRoot,
                                              const std::string& cmakePresetName)
{
    Toolchain::buildProject(pathToProjectRoot, cmakePresetName);

    // BUILDING PROJECT

    const auto vcvarsallbatPath = SGCore::FileUtils::findFile(m_path, "vcvarsall.bat");

    if(vcvarsallbatPath.filename() != "vcvarsall.bat")
    {
        throw std::runtime_error(fmt::format(
                "Error while building project '{0}'. File vcvarsall.bat was not found in Visual Studio toolchain",
                SGCore::Utils::toUTF8(pathToProjectRoot.u16string())));
    }

    const std::string archTypeAsString = VCArchTypeToString(m_archType);
    const std::string platformTypeAsString = VCPlatformTypeToString(m_platformType);

    // PREPARING ENVIRONMENT
    const std::string vcvarsallCommand = fmt::format("\"{0}\" {1} {2} {3}",
                                                     SGCore::Utils::toUTF8(vcvarsallbatPath.u16string()),
                                                     archTypeAsString,
                                                     platformTypeAsString,
                                                     m_winSDKVersion);

    std::printf("Building project '%s' using Visual Studio toolchain: vcvarsall.bat command: '%s'\n",
                SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
                vcvarsallCommand.c_str());

    const std::string envPreparingOutput = SGCore::Utils::consoleExecute(vcvarsallCommand);

    std::printf("Building project '%s' using Visual Studio toolchain: vcvarsall.bat command output:\n%s\n",
                SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
                envPreparingOutput.c_str());

    const std::string cmakeProjectLoadingCommand = fmt::format("\"{0}\" --preset {1} -S \"{2}\"",
                                                               SGCore::Utils::toUTF8(m_cmakePath.u16string()),
                                                               cmakePresetName,
                                                               SGCore::Utils::toUTF8(pathToProjectRoot.u16string()));

    std::printf("Building project '%s' using Visual Studio toolchain: cmake project loading command: '%s'\n",
                SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
                cmakeProjectLoadingCommand.c_str());

    const std::string cmakeProjectLoadingOutput = SGCore::Utils::consoleExecute(cmakeProjectLoadingCommand);

    std::printf("Building project '%s' using Visual Studio toolchain: cmake project loading output:\n%s\n",
                SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
                cmakeProjectLoadingOutput.c_str());

    const std::string cmakeProjectBuildOutput = SGCore::Utils::consoleExecute(
            fmt::format("\"{0}\" --build \"{1}\"", SGCore::Utils::toUTF8(m_cmakePath.u16string()),
            SGCore::Utils::toUTF8(pathToProjectRoot.u16string()) + "/" + m_currentBuildingPresetBinaryDir));

    std::printf("Building project '%s' using Visual Studio toolchain: project build output:\n%s\n",
                SGCore::Utils::toUTF8(pathToProjectRoot.filename().u16string()).c_str(),
                cmakeProjectBuildOutput.c_str());

    // ===============================================================
}
