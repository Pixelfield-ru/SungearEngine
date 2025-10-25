//
// Created by stuka on 09.10.2024.
//

#ifndef SUNGEARENGINE_CMAKEUTILS_H
#define SUNGEARENGINE_CMAKEUTILS_H

#include <filesystem>
#include <string>
#include <vector>

namespace SGCore
{
    struct CMakePresetsFileInfo
    {
        struct Preset
        {
            std::string m_name;
            std::string m_binaryDir;
            std::string m_buildType;
        };

        explicit CMakePresetsFileInfo(const std::filesystem::path& cmakePresetsPath) noexcept;

        CMakePresetsFileInfo(const CMakePresetsFileInfo&) = default;
        CMakePresetsFileInfo(CMakePresetsFileInfo&&) = default;

        CMakePresetsFileInfo& operator=(const CMakePresetsFileInfo&) = default;
        CMakePresetsFileInfo& operator=(CMakePresetsFileInfo&&) = default;

        [[nodiscard]] const Preset* getPreset(const std::string& name) const noexcept;

        [[nodiscard]] const std::vector<Preset>& getPresets() const noexcept;

    private:
        std::vector<Preset> m_presets;
    };

    struct CMakeUtils
    {

    };
}

#endif //SUNGEARENGINE_CMAKEUTILS_H
