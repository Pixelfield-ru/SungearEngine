//
// Created by stuka on 24.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Scene/EntityBaseInfo.h"

namespace SGCore::Serde
{
    // ======================================================== EntityBaseInfo FWD

    template<FormatType TFormatType>
    struct SerdeSpec<EntityBaseInfo, TFormatType> :
            BaseTypes<UniqueNameWrapper>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::EntityBaseInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const EntityBaseInfo, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<EntityBaseInfo, TFormatType>& valueView) noexcept;
    };

    // ======================================================== TransformBase FWD

    template<FormatType TFormatType>
    struct SerdeSpec<TransformBase, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TransformBase";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const TransformBase, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<TransformBase, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Transform FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Transform, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Transform";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Transform, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Transform, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Pickable FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Pickable, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Pickable";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Pickable, TFormatType>& valueView,
                              const ECS::entity_t& deserializableEntity,
                              ECS::registry_t& toRegistry) noexcept;

        static void deserialize(DeserializableValueView<Pickable, TFormatType>& valueView,
                                const ECS::entity_t& deserializableEntity,
                                ECS::registry_t& toRegistry) noexcept;
    };

    // ======================================================== AABB FWD

    template<typename ScalarT, FormatType TFormatType>
    struct SerdeSpec<AABB<ScalarT>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AABB";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AABB<ScalarT>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AABB<ScalarT>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== RenderingBase FWD

    template<FormatType TFormatType>
    struct SerdeSpec<RenderingBase, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::RenderingBase";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const RenderingBase, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<RenderingBase, TFormatType>& valueView) noexcept;
    };

    // ======================================================== AudioSource FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AudioSource, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AudioSource";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AudioSource, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AudioSource, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Atmosphere FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Atmosphere, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Atmosphere";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Atmosphere, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Atmosphere, TFormatType>& valueView) noexcept;
    };

    // ======================================================== SphereGizmo FWD

    template<FormatType TFormatType>
    struct SerdeSpec<SphereGizmo, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SphereGizmo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SphereGizmo, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<SphereGizmo, TFormatType>& valueView) noexcept;
    };

    // ======================================================== BoxGizmo FWD

    template<FormatType TFormatType>
    struct SerdeSpec<BoxGizmo, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::BoxGizmo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const BoxGizmo, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<BoxGizmo, TFormatType>& valueView) noexcept;
    };

    // ======================================================== GizmoBase FWD

    template<FormatType TFormatType>
    struct SerdeSpec<GizmoBase, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::GizmoBase";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const GizmoBase, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<GizmoBase, TFormatType>& valueView) noexcept;
    };

    // ======================================================== LineGizmo FWD

    template<FormatType TFormatType>
    struct SerdeSpec<LineGizmo, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::LineGizmo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const LineGizmo, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<LineGizmo, TFormatType>& valueView) noexcept;
    };

    // ======================================================== MeshBase FWD

    template<FormatType TFormatType>
    struct SerdeSpec<MeshBase, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MeshBase";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MeshBase, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<MeshBase, TFormatType>& valueView) noexcept;
    };
}
