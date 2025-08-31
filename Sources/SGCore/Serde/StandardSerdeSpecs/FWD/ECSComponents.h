//
// Created by stuka on 24.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

namespace SGCore
{
    struct Mesh;
    struct Atmosphere;
    struct EntityBaseInfo;
    class UniqueNameWrapper;
    struct TransformBase;
    struct Transform;
    struct RenderingBase;
    struct Rigidbody3D;
    struct MotionPlannerConnection;
    struct MotionPlannerNode;
    struct MotionPlanner;
    struct OpaqueEntityTag;
    struct TransparentEntityTag;
    class UICamera;
    struct Camera3D;
    struct Controllable3D;
    struct DirectionalLight;
    struct LightBase;
    struct MeshBase;
    struct LineGizmo;
    struct GizmoBase;
    struct BoxGizmo;
    struct SphereGizmo;
    struct AudioSource;
    struct Pickable;

    template<typename ScalarT>
    requires(std::is_signed_v<ScalarT>)
    struct AABB;
}

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

    // ======================================================== Mesh FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Mesh, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Mesh";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Mesh, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Mesh, TFormatType>& valueView) noexcept;
    };

    // ======================================================== LightBase FWD

    template<FormatType TFormatType>
    struct SerdeSpec<LightBase, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::LightBase";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const LightBase, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<LightBase, TFormatType>& valueView) noexcept;
    };

    // ======================================================== DirectionalLight FWD

    template<FormatType TFormatType>
    struct SerdeSpec<DirectionalLight, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::DirectionalLight";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const DirectionalLight, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<DirectionalLight, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Controllable3D FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Controllable3D, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Controllable3D";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Controllable3D, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Controllable3D, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Camera3D FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Camera3D, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Camera3D";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Camera3D, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Camera3D, TFormatType>& valueView) noexcept;
    };

    // ======================================================== UICamera FWD

    template<FormatType TFormatType>
    struct SerdeSpec<UICamera, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::UICamera";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const UICamera, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<UICamera, TFormatType>& valueView) noexcept;
    };

    // ======================================================== TransparentEntityTag FWD

    template<FormatType TFormatType>
    struct SerdeSpec<TransparentEntityTag, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TransparentEntityTag";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const TransparentEntityTag, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<TransparentEntityTag, TFormatType>& valueView) noexcept;
    };

    // ======================================================== OpaqueEntityTag FWD

    template<FormatType TFormatType>
    struct SerdeSpec<OpaqueEntityTag, TFormatType> :
           BaseTypes<>,
           DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::OpaqueEntityTag";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const OpaqueEntityTag, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<OpaqueEntityTag, TFormatType>& valueView) noexcept;
    };

    // ======================================================== MotionPlanner FWD

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlanner, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlanner";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlanner, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<MotionPlanner, TFormatType>& valueView) noexcept;
    };

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlannerNode, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlannerNode";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlannerNode, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<MotionPlannerNode, TFormatType>& valueView) noexcept;
    };

    // ======================================================== MotionPlannerConnection FWD

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlannerConnection, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlannerConnection";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlannerConnection, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<MotionPlannerConnection, TFormatType>& valueView) noexcept;
    };

    // ======================================================== MotionPlannerConnection FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Rigidbody3D, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Rigidbody3D";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Rigidbody3D, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Rigidbody3D, TFormatType>& valueView) noexcept;
    };
}
