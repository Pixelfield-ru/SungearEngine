//
// Created by Ilya on 31.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_SERIALIZERS_H
#define SUNGEARENGINEEDITOR_SERIALIZERS_H

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs/SerdeSpecs.h>
#include "Toolchains/Toolchain.h"
#include "Toolchains/VisualStudioToolchain.h"
#include "Settings/EngineSettings.h"

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::Toolchain, TFormatType> : BaseTypes<>, DerivedTypes<SGE::VisualStudioToolchain>
{
    static inline const std::string type_name = "SGE::Toolchain";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<const SGE::Toolchain, TFormatType>& valueView)
    {
        valueView.container().addMember("m_name", valueView.m_data->m_name);
        valueView.container().addMember("m_type", valueView.m_data->m_type);
        valueView.container().addMember("m_path", valueView.m_data->m_path);
        valueView.container().addMember("m_cmakePath", valueView.m_data->m_cmakePath);
        valueView.container().addMember("m_cmakeVersion", valueView.m_data->m_cmakeVersion);
        valueView.container().addMember("m_buildToolPath", valueView.m_data->m_buildToolPath);
        valueView.container().addMember("m_buildToolVersion", valueView.m_data->m_buildToolVersion);
    }

    static void deserialize(DeserializableValueView<SGE::Toolchain, TFormatType>& valueView)
    {
        std::cout << "deser toolchain" << std::endl;

        const auto name = valueView.container().template getMember<SGCore::UniqueName>("m_name");
        if(name)
        {
            valueView.m_data->m_name = *name;
        }

        const auto type = valueView.container().template getMember<SGE::ToolchainType>("m_type");
        if(type)
        {
            valueView.m_data->m_type = *type;
        }

        const auto path = valueView.container().template getMember<std::filesystem::path>("m_path");
        if(path)
        {
            valueView.m_data->m_path = *path;
        }

        const auto cmakePath = valueView.container().template getMember<std::filesystem::path>("m_cmakePath");
        if(cmakePath)
        {
            valueView.m_data->m_cmakePath = *cmakePath;
        }

        const auto cmakeVersion = valueView.container().template getMember<std::string>("m_cmakeVersion");
        if(cmakeVersion)
        {
            valueView.m_data->m_cmakeVersion = *cmakeVersion;
        }

        const auto buildToolPath = valueView.container().template getMember<std::filesystem::path>("m_buildToolPath");
        if(buildToolPath)
        {
            valueView.m_data->m_buildToolPath = *buildToolPath;
        }

        const auto buildToolVersion = valueView.container().template getMember<std::string>("m_buildToolVersion");
        if(buildToolVersion)
        {
            valueView.m_data->m_buildToolVersion = *buildToolVersion;
        }
    }
};

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::VisualStudioToolchain, TFormatType> : BaseTypes<SGE::Toolchain>, DerivedTypes<>
{
    static inline const std::string type_name = "SGE::VisualStudioToolchain";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<const SGE::VisualStudioToolchain, TFormatType>& valueView)
    {
        valueView.container().addMember("m_archType", valueView.m_data->m_archType);
        valueView.container().addMember("m_platformType", valueView.m_data->m_platformType);
        valueView.container().addMember("m_winSDKVersion", valueView.m_data->m_winSDKVersion);
        valueView.container().addMember("m_vcvarsallPath", valueView.m_data->m_vcvarsallPath);
    }

    static void deserialize(DeserializableValueView<SGE::VisualStudioToolchain, TFormatType>& valueView)
    {
        std::cout << "deser VisualStudioToolchain" << std::endl;

        const auto archType = valueView.container().template getMember<SGE::VCArchType>("m_archType");
        if(archType)
        {
            valueView.m_data->m_archType = *archType;
        }

        const auto platformType = valueView.container().template getMember<SGE::VCPlatformType>("m_platformType");
        if(platformType)
        {
            valueView.m_data->m_platformType = *platformType;
        }

        const auto winSDKVersion = valueView.container().template getMember<std::string>("m_winSDKVersion");
        if(winSDKVersion)
        {
            valueView.m_data->m_winSDKVersion = *winSDKVersion;
        }

        const auto vcvarsallPath = valueView.container().template getMember<std::filesystem::path>("m_vcvarsallPath");
        if(vcvarsallPath)
        {
            valueView.m_data->m_vcvarsallPath = *vcvarsallPath;
        }
    }
};

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::EngineSettings, TFormatType> : BaseTypes<>, DerivedTypes<>
{
    static inline const std::string type_name = "SGE::EngineSettings";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<const SGE::EngineSettings, TFormatType>& valueView)
    {
        valueView.container().addMember("m_toolchains", valueView.m_data->m_toolchains);
    }

    static void deserialize(DeserializableValueView<SGE::EngineSettings, TFormatType>& valueView)
    {
        const auto toolchains = valueView.container()
                .template getMember<std::vector<SGCore::Ref<SGE::Toolchain>>>("m_toolchains");
        if(toolchains)
        {
            valueView.m_data->m_toolchains = *toolchains;
        }
    }
};

#endif //SUNGEARENGINEEDITOR_SERIALIZERS_H
