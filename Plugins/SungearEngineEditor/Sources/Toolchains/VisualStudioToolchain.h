//
// Created by Ilya on 16.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_VISUALSTUDIOTOOLCHAIN_H
#define SUNGEARENGINEEDITOR_VISUALSTUDIOTOOLCHAIN_H

#include "Toolchain.h"

namespace SGE
{
    enum class VCArchType
    {
        X86,
        AMD64,
        X86_AMD64,
        X86_ARM,
        X86_ARM64,
        AMD64_X86,
        AMD64_ARM,
        AMD64_ARM64
    };

    enum class VCPlatformType
    {
        EMPTY,
        STORE,
        UWP
    };

    std::string VCArchTypeToString(const VCArchType& archType) noexcept;
    std::string VCPlatformTypeToString(const VCPlatformType& platformType) noexcept;

    struct VisualStudioToolchain : public Toolchain
    {
        VisualStudioToolchain();
        VisualStudioToolchain(const VisualStudioToolchain&) = default;
        VisualStudioToolchain(VisualStudioToolchain&&) = default;

        VCArchType m_archType = VCArchType::X86;
        VCPlatformType m_platformType = VCPlatformType::EMPTY;

        std::string m_winSDKVersion;

        std::filesystem::path m_vcvarsallPath;

        void configurate() final;
        void buildProject(const std::filesystem::path& pathToProjectRoot, const std::string& cmakePresetName) final;
    };
}

#endif //SUNGEARENGINEEDITOR_VISUALSTUDIOTOOLCHAIN_H
