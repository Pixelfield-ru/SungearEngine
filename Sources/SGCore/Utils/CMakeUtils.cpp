//
// Created by stuka on 09.10.2024.
//

#include "CMakeUtils.h"

#include "FileUtils.h"

#include <SGCore/Logger/Logger.h>

#include "SGCore/Serde/Common.h"
#include "SGCore/Serde/Serde.h"

#include "SGCore/Serde/StandardSerdeSpecs/STD.h"
#include "SGCore/Serde/StandardSerdeSpecs/Utils.h"

SGCore::CMake::PresetsFileInfo::PresetsFileInfo(const std::filesystem::path& cmakePresetsPath) noexcept
{
    if(!std::filesystem::exists(cmakePresetsPath))
    {
        LOG_W(SGCORE_TAG, "Error while parsing CMakePresets.json. Path '{}' does not exist.", Utils::toUTF8(cmakePresetsPath.u16string()))
        return;
    }

    std::string log;
    PresetsFileInfo fileInfo;
    fileInfo.m_path = cmakePresetsPath;
    Serde::Serializer::fromFormat(
        FileUtils::readFile(cmakePresetsPath),
        fileInfo, Serde::FormatType::JSON, log);

    *this = fileInfo;

    if(!log.empty())
    {
        LOG_W(SGCORE_TAG, "Warnings while parsing CMakePresets.json by path '{}': {}", Utils::toUTF8(cmakePresetsPath.u16string()), log)
    }
}

const SGCore::CMake::Preset*
SGCore::CMake::PresetsFileInfo::getPreset(const std::string& name) const noexcept
{
    for(const auto& preset : m_presets)
    {
        if(preset.m_name == name) return &preset;
    }

    for(const auto& presetsFile : m_includedFiles)
    {
        if(const auto* preset = presetsFile.getPreset(name)) return preset;
    }

    return nullptr;
}

const std::vector<SGCore::CMake::Preset>& SGCore::CMake::PresetsFileInfo::getPresets() const noexcept
{
    return m_presets;
}
