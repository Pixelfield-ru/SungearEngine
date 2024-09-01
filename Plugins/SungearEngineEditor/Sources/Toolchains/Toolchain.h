//
// Created by Ilya on 16.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAIN_H
#define SUNGEARENGINEEDITOR_TOOLCHAIN_H

#include "ToolchainType.h"

#include <filesystem>
#include <SGCore/Utils/UniqueName.h>
#include <SGCore/Utils/Utils.h>
#include <SGCore/Main/CoreGlobals.h>

sg_predeclare_serdespec()

namespace SGE
{
    struct Toolchain
    {
        struct ProjectSpecific
        {
            static void buildProject(const SGCore::Ref<Toolchain>& toolchain) noexcept;

            static void setCurrentCMakePreset(const std::string& presetName) noexcept;
            SG_NOINLINE static std::string getCurrentCMakePreset() noexcept;

        private:
            static inline std::string m_currentCMakePreset = SG_BUILD_PRESET;
        };

        struct ProjectBuildOutput
        {
            std::filesystem::path m_projectDynamicLibraryPath;
            std::filesystem::path m_projectName;
            std::string m_binaryDir;
        };

        sg_serdespec_as_friend()

        bool m_doInBackground = true;
        std::function<void(const ProjectBuildOutput& buildOutput)> onProjectBuilt;
        // callback in main thread
        std::function<void(const ProjectBuildOutput& buildOutput)> onProjectBuiltSynchronized;

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
        std::filesystem::path m_builtDynamicLibraryPath;
        std::filesystem::path m_projectName;
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAIN_H
