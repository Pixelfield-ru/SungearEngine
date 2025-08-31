//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

namespace SGCore
{
    struct InterpolatedPath;
    struct Config;
    struct LoadablePluginConfig;
    struct UniqueName;
    class UniqueNameWrapper;
    struct EntityBaseInfo;
}

namespace SGCore::Serde
{
    // ======================================================== UniqueNameWrapper FWD

    template<FormatType TFormatType>
    struct SerdeSpec<UniqueNameWrapper, TFormatType> :
            BaseTypes<>,
            DerivedTypes<EntityBaseInfo>
    {
        static inline const std::string type_name = "SGCore::UniqueNameWrapper";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const UniqueNameWrapper, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<UniqueNameWrapper, TFormatType>& valueView) noexcept;
    };

    // ======================================================== UniqueName FWD

    template<FormatType TFormatType>
    struct SerdeSpec<UniqueName, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::UniqueName";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const UniqueName, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<UniqueName, TFormatType>& valueView) noexcept;
    };

    // ======================================================== LoadablePluginConfig FWD

    template<FormatType TFormatType>
    struct SerdeSpec<LoadablePluginConfig, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::LoadablePluginConfig";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const LoadablePluginConfig, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<LoadablePluginConfig, TFormatType>& valueView) noexcept;
    };

    // ======================================================== LoadablePluginConfig FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Config, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Config";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Config, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Config, TFormatType>& valueView) noexcept;
    };

    // ======================================================== InterpolatedPath FWD

    template<FormatType TFormatType>
    struct SerdeSpec<InterpolatedPath, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::InterpolatedPath";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const InterpolatedPath, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<InterpolatedPath, TFormatType>& valueView) noexcept;
    };
}
