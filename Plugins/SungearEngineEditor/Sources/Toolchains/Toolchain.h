//
// Created by Ilya on 16.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAIN_H
#define SUNGEARENGINEEDITOR_TOOLCHAIN_H

#include "ToolchainType.h"

#include <filesystem>
#include <SGCore/Utils/UniqueName.h>

namespace SGE
{
    struct Toolchain
    {
        virtual ~Toolchain() = default;

        SGCore::UniqueName m_name;

        [[nodiscard]] ToolchainType getType() const;

        [[nodiscard]] std::filesystem::path getPath() const;
        void setPath(const std::filesystem::path& path);

        [[nodiscard]] std::filesystem::path getCMakePath() const;
        void setCMakePath(const std::filesystem::path& cmakePath);

        [[nodiscard]] std::filesystem::path getBuildToolPath() const;
        void setBuildToolPath(const std::filesystem::path& buildToolPath);

        [[nodiscard]] std::string getCMakeVersion() const;
        [[nodiscard]] std::string getBuildToolVersion() const;

        /*[[nodiscard]] std::filesystem::path getCCompilerPath() const;
        void setCCompilerPath(const std::filesystem::path& CCompilerPath);

        [[nodiscard]] std::filesystem::path getCXXCompilerPath() const;
        void setCXXCompilerPath(const std::filesystem::path& CXXCompilerPath);*/

        virtual void configurate();

        virtual void buildProject(const std::filesystem::path& pathToProjectRoot, const std::string& cmakePresetName);

        virtual Toolchain* copy() const = 0;

    protected:
        ToolchainType m_type = ToolchainType::UNKNOWN;

        std::filesystem::path m_path;

        std::filesystem::path m_cmakePath;
        std::string m_cmakeVersion;

        std::filesystem::path m_buildToolPath;
        std::string m_buildToolVersion;

        // std::filesystem::path m_CCompilerPath;
        // std::filesystem::path m_CXXCompilerPath;

        // TEMP DATA
        std::string m_currentBuildingPresetBinaryDir;
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAIN_H
