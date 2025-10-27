//
// Created by stuka on 09.10.2024.
//

#ifndef SUNGEARENGINE_CMAKEUTILS_H
#define SUNGEARENGINE_CMAKEUTILS_H

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "SGCore/Serde/Defines.h"

sg_predeclare_serde()

namespace SGCore::CMake
{
    struct Preset
    {
        std::string m_name;
        std::string m_binaryDir;
        std::unordered_map<std::string, std::string> m_cachedVariables;
    };

    struct PresetsFileInfo
    {
        sg_serde_as_friend()

        PresetsFileInfo() noexcept = default;;
        explicit PresetsFileInfo(const std::filesystem::path& cmakePresetsPath) noexcept;
        PresetsFileInfo(const PresetsFileInfo&) = default;
        PresetsFileInfo(PresetsFileInfo&&) = default;

        PresetsFileInfo& operator=(const PresetsFileInfo&) = default;
        PresetsFileInfo& operator=(PresetsFileInfo&&) = default;

        [[nodiscard]] const Preset* getPreset(const std::string& name) const noexcept;

        [[nodiscard]] const std::vector<Preset>& getPresets() const noexcept;

    private:
        std::filesystem::path m_path;
        std::vector<Preset> m_presets;
        std::vector<PresetsFileInfo> m_includedFiles;
    };
}

#endif //SUNGEARENGINE_CMAKEUTILS_H
