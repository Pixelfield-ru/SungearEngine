//
// Created by stuka on 09.10.2024.
//

#include "CMakeUtils.h"
#include "FileUtils.h"

#include <rapidjson/rapidjson.h>
#include <SGCore/Logger/Logger.h>

SGCore::CMakePresetsFileInfo::CMakePresetsFileInfo(const std::filesystem::path& cmakePresetsPath) noexcept
{
    rapidjson::Document cmakePresetsDocument;
    cmakePresetsDocument.Parse(SGCore::FileUtils::readFile(cmakePresetsPath).c_str());

    if(!cmakePresetsDocument.HasMember("configurePresets"))
    {
        LOG_E(SGCORE_TAG, "Can not load CMakePresets.json by path '{}'. Can not find 'configurePresets' section in CMakePresets.json file.",
              SGCore::Utils::toUTF8(cmakePresetsPath.u16string()));
        return;
    }

    auto& presets = cmakePresetsDocument["configurePresets"];

    for(std::size_t i = 0; i < presets.Size(); ++i)
    {
        auto& jsonPreset = presets[i];
        Preset preset;

        if(!jsonPreset.HasMember("name"))
        {
            LOG_W(SGCORE_TAG, "In CMakePresets.json by path '{}': can not load 'name' section of preset.",
                  SGCore::Utils::toUTF8(cmakePresetsPath.u16string()));
        }
        else
        {
            preset.m_name = jsonPreset["name"].GetString();
        }

        if(!jsonPreset.HasMember("name"))
        {
            LOG_W(SGCORE_TAG, "In CMakePresets.json by path '{}': can not load 'binaryDir' section of preset.",
                  SGCore::Utils::toUTF8(cmakePresetsPath.u16string()));
        }
        else
        {
            preset.m_binaryDir = jsonPreset["binaryDir"].GetString();
        }

        if(!jsonPreset.HasMember("cacheVariables"))
        {
            LOG_W(SGCORE_TAG, "In CMakePresets.json by path '{}': can not load 'cacheVariables' section of preset.",
                  SGCore::Utils::toUTF8(cmakePresetsPath.u16string()));
        }
        else
        {
            auto& cacheVariablesOfPresetSection = jsonPreset["cacheVariables"];

            if(!cacheVariablesOfPresetSection.HasMember("CMAKE_BUILD_TYPE"))
            {
                LOG_W(SGCORE_TAG, "In CMakePresets.json by path '{}': can not load 'CMAKE_BUILD_TYPE' section in 'cacheVariables' section of preset.",
                      SGCore::Utils::toUTF8(cmakePresetsPath.u16string()));
            }
            else
            {
                preset.m_buildType = cacheVariablesOfPresetSection["CMAKE_BUILD_TYPE"].GetString();
            }
        }

        m_presets.push_back(preset);
    }
}

const SGCore::CMakePresetsFileInfo::Preset*
SGCore::CMakePresetsFileInfo::getPreset(const std::string& name) const noexcept
{
    for(const auto& preset : m_presets)
    {
        if(preset.m_name == name) return &preset;
    }

    return nullptr;
}

const std::vector<SGCore::CMakePresetsFileInfo::Preset>& SGCore::CMakePresetsFileInfo::getPresets() const noexcept
{
    return m_presets;
}
