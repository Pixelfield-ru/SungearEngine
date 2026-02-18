//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Main/Config.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/Utils/CMakeUtils.h"

namespace SGCore::Serde
{
    // ======================================================== impl UniqueNameWrapper

    template<FormatType TFormatType>
    void SerdeSpec<UniqueNameWrapper, TFormatType>::serialize(SerializableValueView<const UniqueNameWrapper, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_uniqueName", valueView.m_data->m_uniqueName);
    }

    template<FormatType TFormatType>
    void SerdeSpec<UniqueNameWrapper, TFormatType>::deserialize(DeserializableValueView<UniqueNameWrapper, TFormatType>& valueView) noexcept
    {
        const auto m_uniqueName = valueView.container().template getMember<decltype(valueView.m_data->m_uniqueName)>("m_uniqueName");
        if(m_uniqueName)
        {
            valueView.m_data->m_uniqueName = *m_uniqueName;
        }
    }

    // ======================================================== impl UniqueName

    template<FormatType TFormatType>
    void SerdeSpec<UniqueName, TFormatType>::serialize(SerializableValueView<const UniqueName, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_name", valueView.m_data->m_name);
        valueView.container().addMember("m_rawName", valueView.m_data->m_rawName);
        valueView.container().addMember("m_uniqueID", valueView.m_data->m_uniqueID);
    }

    template<FormatType TFormatType>
    void SerdeSpec<UniqueName, TFormatType>::deserialize(DeserializableValueView<UniqueName, TFormatType>& valueView) noexcept
    {
        const auto m_name = valueView.container().template getMember<decltype(valueView.m_data->m_name)>("m_name");
        if(m_name)
        {
            valueView.m_data->m_name = *m_name;
        }

        const auto m_rawName = valueView.container().template getMember<decltype(valueView.m_data->m_rawName)>("m_rawName");
        if(m_rawName)
        {
            valueView.m_data->m_rawName = *m_rawName;
        }

        const auto m_uniqueID = valueView.container().template getMember<decltype(valueView.m_data->m_uniqueID)>("m_uniqueID");
        if(m_uniqueID)
        {
            valueView.m_data->m_uniqueID = *m_uniqueID;
        }
    }

    // ======================================================== impl UniqueName

    template<FormatType TFormatType>
    void SerdeSpec<LoadablePluginConfig, TFormatType>::serialize(SerializableValueView<const LoadablePluginConfig, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_pluginName", valueView.m_data->m_pluginName);
        valueView.container().addMember("m_pluginPath", valueView.m_data->m_pluginPath);
        valueView.container().addMember("m_pluginEntryArgs", valueView.m_data->m_pluginEntryArgs);
        valueView.container().addMember("m_pluginCMakeBuildDir", valueView.m_data->m_pluginCMakeBuildDir);
        valueView.container().addMember("m_enabled", valueView.m_data->m_enabled);
    }

    template<FormatType TFormatType>
    void SerdeSpec<LoadablePluginConfig, TFormatType>::deserialize(DeserializableValueView<LoadablePluginConfig, TFormatType>& valueView) noexcept
    {
        auto m_pluginName = valueView.container().template getMember<std::string>("m_pluginName");
        if(m_pluginName)
        {
            valueView.m_data->m_pluginName = std::move(*m_pluginName);
        }

        auto m_pluginPath = valueView.container().template getMember<InterpolatedPath>("m_pluginPath");
        if(m_pluginPath)
        {
            valueView.m_data->m_pluginPath = std::move(*m_pluginPath);
        }

        auto m_pluginEntryArgs = valueView.container().template getMember<std::vector<std::string>>("m_pluginEntryArgs");
        if(m_pluginEntryArgs)
        {
            valueView.m_data->m_pluginEntryArgs = std::move(*m_pluginEntryArgs);
        }

        auto m_pluginCMakeBuildDir = valueView.container().template getMember<std::string>("m_pluginCMakeBuildDir");
        if(m_pluginCMakeBuildDir)
        {
            valueView.m_data->m_pluginCMakeBuildDir = std::move(*m_pluginCMakeBuildDir);
        }

        auto m_enabled = valueView.container().template getMember<bool>("m_enabled");
        if(m_enabled)
        {
            valueView.m_data->m_enabled = *m_enabled;
        }
    }

    // ======================================================== impl Config

    template<FormatType TFormatType>
    void SerdeSpec<Config, TFormatType>::serialize(SerializableValueView<const Config, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_loadablePlugins", valueView.m_data->m_loadablePlugins);
        valueView.container().addMember("m_enginePath", valueView.m_data->m_enginePath);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Config, TFormatType>::deserialize(DeserializableValueView<Config, TFormatType>& valueView) noexcept
    {
        auto m_loadablePlugins = valueView.container().template getMember<std::vector<LoadablePluginConfig>>("m_loadablePlugins");
        if(m_loadablePlugins)
        {
            valueView.m_data->m_loadablePlugins = std::move(*m_loadablePlugins);
        }

        auto m_enginePath = valueView.container().template getMember<InterpolatedPath>("m_enginePath");
        if(m_enginePath)
        {
            valueView.m_data->m_enginePath = std::move(*m_enginePath);
        }
        else
        {
            valueView.m_data->m_enginePath = "${enginePath}";
        }
    }

    // ======================================================== impl InterpolatedPath

    template<FormatType TFormatType>
    void SerdeSpec<InterpolatedPath, TFormatType>::serialize(SerializableValueView<const InterpolatedPath, TFormatType>& valueView) noexcept
    {
        const std::string u8Path = SGCore::Utils::toUTF8(valueView.m_data->raw().u16string());
        valueView.container().setAsString(u8Path);
    }

    template<FormatType TFormatType>
    void SerdeSpec<InterpolatedPath, TFormatType>::deserialize(DeserializableValueView<InterpolatedPath, TFormatType>& valueView) noexcept
    {
        const std::u16string tmpPath = valueView.container().template getAsString<char16_t>();
        *valueView.m_data = tmpPath;
    }

    // ======================================================== impl CMake::PresetsFileInfo

    template<FormatType TFormatType>
    void SerdeSpec<CMake::Preset, TFormatType>::serialize(SerializableValueView<const CMake::Preset, TFormatType>& valueView, const CMake::PresetsFileInfo& presetsFile) noexcept
    {
        LOG_NOT_IMPLEMENTED(SGCORE_TAG)
    }

    template<FormatType TFormatType>
    void SerdeSpec<CMake::Preset, TFormatType>::deserialize(DeserializableValueView<CMake::Preset, TFormatType>& valueView, const CMake::PresetsFileInfo& presetsFile) noexcept
    {
        auto binaryDir = valueView.container().template getMember<std::string>("binaryDir");
        if(binaryDir)
        {
            valueView.m_data->m_binaryDir = std::move(*binaryDir);
        }

        auto name = valueView.container().template getMember<std::string>("name");
        if(name)
        {
            valueView.m_data->m_name = std::move(*name);
        }

        std::vector<CMake::Preset> inheritedPresets;
        const auto inheritedPresetsTmp = valueView.container().template getMember<std::vector<std::string>>("inherits");
        if(inheritedPresetsTmp)
        {
            for(const auto& presetName : *inheritedPresetsTmp)
            {
                if(const auto* preset = presetsFile.getPreset(presetName))
                {
                    inheritedPresets.push_back(*preset);
                }
            }
        }
        else
        {
            // trying to inherit single preset
            const auto inheritedPresetTmp = valueView.container().template getMember<std::string>("inherits");
            if(inheritedPresetTmp)
            {
                if(const auto* preset = presetsFile.getPreset(*inheritedPresetTmp))
                {
                    inheritedPresets.push_back(*preset);
                }
            }
        }

        static auto joinCaches = [](const std::unordered_map<std::string, std::string>& selfCache,
                                    const std::vector<CMake::Preset>& inheritedPresets) -> std::unordered_map<std::string, std::string> {
            std::unordered_map<std::string, std::string> result;

            for(const auto& [variable, value] : selfCache)
            {
                if(result.contains(variable)) continue;
                result[variable] = value;
            }

            for(std::int64_t i = inheritedPresets.size() - 1; i >= 0; --i)
            {
                const auto& inheritedPreset = inheritedPresets[i];

                for(const auto& [variable, value] : inheritedPreset.m_cachedVariables)
                {
                    if(result.contains(variable)) continue;
                    result[variable] = value;
                }
            }

            return result;
        };

        const auto cacheVariables = valueView.container().template getMember<std::unordered_map<std::string, std::string>>("cacheVariables");
        if(cacheVariables)
        {
            auto cache = joinCaches(*cacheVariables, inheritedPresets);
            valueView.m_data->m_cachedVariables = std::move(cache);
        }
    }

    // ======================================================== impl CMake::PresetsFileInfo

    template<FormatType TFormatType>
    void SerdeSpec<CMake::PresetsFileInfo, TFormatType>::serialize(SerializableValueView<const CMake::PresetsFileInfo, TFormatType>& valueView) noexcept
    {
        LOG_NOT_IMPLEMENTED(SGCORE_TAG)
    }

    template<FormatType TFormatType>
    void SerdeSpec<CMake::PresetsFileInfo, TFormatType>::deserialize(DeserializableValueView<CMake::PresetsFileInfo, TFormatType>& valueView) noexcept
    {
        static auto matchNextVariable = [](std::string& stringToFormat, const std::string& leftBraceSign, std::string& variable) noexcept {
            const size_t leftBracePos = stringToFormat.find(leftBraceSign);
            if(leftBracePos == std::string::npos)
            {
                variable = "";
                return leftBracePos;
            }

            const size_t rightBracePos = stringToFormat.find("}", leftBracePos);
            const std::string varName = stringToFormat.substr(leftBracePos, rightBracePos - leftBracePos);

            std::cout << "CMakePresets: found variable: " << varName << std::endl;

            stringToFormat.erase(leftBracePos, 5);
            stringToFormat.erase(rightBracePos, 1);

            variable = varName;
            return leftBracePos;
        };

        const auto includedFiles = valueView.container().template getMember<std::vector<std::string>>("include");
        if(includedFiles)
        {
            for(const auto& includedFilePath : *includedFiles)
            {
                auto formattedPath = includedFilePath;
                std::string variable;
                while(size_t varPlace = matchNextVariable(formattedPath, "$env{", variable) != std::string::npos)
                {
                    const char* envVarContent = std::getenv(variable.c_str());
                    if(envVarContent)
                    {
                        LOG_E(SGCORE_TAG, "Error while loading presets from cmake: trying to load JSON file by path '{}' but met unknown environemnt variable '{}'", includedFilePath, variable);
                    }

                    std::cout << fmt::format("CMakePresets: found variable '{}', inserted value '{}'", variable, envVarContent) << std::endl;
                    formattedPath.insert(varPlace, envVarContent);
                }

                // todo: implement macro expansion for built-in cmake variables
                while(size_t varPlace = matchNextVariable(formattedPath, "${", variable) != std::string::npos)
                {

                }

                if(!std::filesystem::exists(formattedPath))
                {
                    // trying to apply local path
                    formattedPath = SGCore::Utils::toUTF8(valueView.m_data->m_path.u16string()) + "/../" + formattedPath;
                }

                std::string testDeserLog;
                CMake::PresetsFileInfo includedFile(formattedPath);
                valueView.m_data->m_includedFiles.push_back(includedFile);
            }
        }

        auto configurePresets = valueView.container().template getMember<std::vector<CMake::Preset>>("configurePresets", *valueView.m_data);
        if(configurePresets)
        {
            valueView.m_data->m_presets = std::move(*configurePresets);
        }
    }
}