//
// Created by stuka on 21.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

namespace SGCore
{
    struct IKResolver;
    struct MotionPlannersResolver;
    struct AudioProcessor;
    class OctreesSolver;
    struct SphereGizmosUpdater;
    struct LineGizmosRenderer;
    class BoxGizmosRenderer;
    struct DirectionalLightsUpdater;
    struct AtmosphereUpdater;
    struct RenderingBasesUpdater;
    struct PhysicsWorld3D;
    struct Controllables3DUpdater;

    template<typename>
    struct IParallelSystem;
}

namespace SGCore::Serde
{
    // ======================================================== ISystem FWD

    template<FormatType TFormatType>
    struct SerdeSpec<ISystem, TFormatType> :
            BaseTypes<>,
            DerivedTypes<
                Controllables3DUpdater,
                PhysicsWorld3D,
                RenderingBasesUpdater,
                AtmosphereUpdater,
                DirectionalLightsUpdater,
                TransformationsUpdater,
                BoxGizmosRenderer,
                LineGizmosRenderer,
                SphereGizmosUpdater,
                OctreesSolver,
                AudioProcessor,
                MotionPlannersResolver,
                IKResolver
            >
    {
        static inline const std::string type_name = "SGCore::ISystem";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ISystem, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<ISystem, TFormatType>& valueView) noexcept;
    };

    // ======================================================== IParallelSystem FWD

    template<typename ParallelSystemT, FormatType TFormatType>
    struct SerdeSpec<IParallelSystem<ParallelSystemT>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IParallelSystem";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== Controllables3DUpdater FWD

    template<FormatType TFormatType>
    struct SerdeSpec<Controllables3DUpdater, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Controllables3DUpdater";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Controllables3DUpdater, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<Controllables3DUpdater, TFormatType>& valueView) noexcept;
    };

    // ======================================================== PhysicsWorld3D FWD

    template<FormatType TFormatType>
    struct SerdeSpec<PhysicsWorld3D, TFormatType> :
            BaseTypes<IParallelSystem<PhysicsWorld3D>, ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::PhysicsWorld3D";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const PhysicsWorld3D, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<PhysicsWorld3D, TFormatType>& valueView) noexcept;
    };

    // ======================================================== RenderingBasesUpdater FWD

    template<FormatType TFormatType>
    struct SerdeSpec<RenderingBasesUpdater, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::RenderingBasesUpdater";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const RenderingBasesUpdater, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<RenderingBasesUpdater, TFormatType>& valueView) noexcept;
    };

    // ======================================================== AtmosphereUpdater FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AtmosphereUpdater, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AtmosphereUpdater";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AtmosphereUpdater, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AtmosphereUpdater, TFormatType>& valueView) noexcept;
    };

    // ======================================================== DirectionalLightsUpdater FWD

    template<FormatType TFormatType>
    struct SerdeSpec<DirectionalLightsUpdater, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::DirectionalLightsUpdater";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const DirectionalLightsUpdater, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<DirectionalLightsUpdater, TFormatType>& valueView) noexcept;
    };

    // ======================================================== TransformationsUpdater FWD

    template<FormatType TFormatType>
    struct SerdeSpec<TransformationsUpdater, TFormatType> :
            BaseTypes<IParallelSystem<TransformationsUpdater>, ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TransformationsUpdater";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const TransformationsUpdater, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<TransformationsUpdater, TFormatType>& valueView) noexcept;
    };

    // ======================================================== BoxGizmosRenderer FWD

    template<FormatType TFormatType>
    struct SerdeSpec<BoxGizmosRenderer, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::BoxGizmosRenderer";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const BoxGizmosRenderer, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<BoxGizmosRenderer, TFormatType>& valueView) noexcept;
    };

    // ======================================================== LineGizmosRenderer FWD

    template<FormatType TFormatType>
    struct SerdeSpec<LineGizmosRenderer, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::LineGizmosRenderer";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const LineGizmosRenderer, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<LineGizmosRenderer, TFormatType>& valueView) noexcept;
    };

    // ======================================================== SphereGizmosUpdater FWD

    template<FormatType TFormatType>
    struct SerdeSpec<SphereGizmosUpdater, TFormatType> :
        BaseTypes<ISystem>,
        DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SphereGizmosUpdater";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SphereGizmosUpdater, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<SphereGizmosUpdater, TFormatType>& valueView) noexcept;
    };

    // ======================================================== OctreesSolver FWD

    template<FormatType TFormatType>
    struct SerdeSpec<OctreesSolver, TFormatType> :
            BaseTypes<IParallelSystem<OctreesSolver>, ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::OctreesSolver";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const OctreesSolver, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<OctreesSolver, TFormatType>& valueView) noexcept;
    };

    // ======================================================== AudioProcessor FWD

    template<FormatType TFormatType>
    struct SerdeSpec<AudioProcessor, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AudioProcessor";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AudioProcessor, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<AudioProcessor, TFormatType>& valueView) noexcept;
    };

    // ======================================================== MotionPlannersResolver FWD

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlannersResolver, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlannersResolver";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlannersResolver, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<MotionPlannersResolver, TFormatType>& valueView) noexcept;
    };

    // ======================================================== IKResolver FWD

    template<FormatType TFormatType>
    struct SerdeSpec<IKResolver, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IKResolver";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IKResolver, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<IKResolver, TFormatType>& valueView) noexcept;
    };
}
