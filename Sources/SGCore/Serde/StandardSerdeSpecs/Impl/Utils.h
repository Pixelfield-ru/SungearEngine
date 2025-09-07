//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Main/Config.h"
#include "SGCore/Serde/Serde.h"

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
    }

    template<FormatType TFormatType>
    void SerdeSpec<Config, TFormatType>::deserialize(DeserializableValueView<Config, TFormatType>& valueView) noexcept
    {
        auto m_loadablePlugins = valueView.container().template getMember<std::vector<LoadablePluginConfig>>("m_loadablePlugins");
        if(m_loadablePlugins)
        {
            valueView.m_data->m_loadablePlugins = std::move(*m_loadablePlugins);
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
}