//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

namespace SGCore
{
    struct BlendingState;
    struct MeshRenderState;
    struct RenderState;
    struct ShaderTextureBinding;
    struct ShaderDefine;
}

namespace SGCore::Serde
{
    // ======================================================== RenderState FWD

    template<FormatType TFormatType>
    struct SerdeSpec<RenderState, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::RenderState")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const RenderState, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<RenderState, TFormatType>& valueView) noexcept;
    };

    // ======================================================== MeshRenderState FWD

    template<FormatType TFormatType>
    struct SerdeSpec<MeshRenderState, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::MeshRenderState")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MeshRenderState, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<MeshRenderState, TFormatType>& valueView) noexcept;
    };

    // ======================================================== BlendingState FWD

    template<FormatType TFormatType>
    struct SerdeSpec<BlendingState, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::BlendingState")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const BlendingState, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<BlendingState, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ShaderTextureBinding FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ShaderTextureBinding, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::ShaderTextureBinding")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ShaderTextureBinding, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<ShaderTextureBinding, TFormatType>& valueView) noexcept;
    };

    // ======================================================== ShaderDefine FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ShaderDefine, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("SGCore::ShaderDefine")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ShaderDefine, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<ShaderDefine, TFormatType>& valueView) noexcept;
    };
}
