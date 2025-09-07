//
// Created by stuka on 21.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Scene/ISystem.h"

#include "SGCore/Transformations/Controllables3DUpdater.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/Render/RenderingBasesUpdater.h"
#include "SGCore/Render/Atmosphere/AtmosphereUpdater.h"
#include "SGCore/Render/Gizmos/BoxGizmosRenderer.h"
#include "SGCore/Render/Gizmos/LineGizmosRenderer.h"
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLightsUpdater.h"
#include "SGCore/Render/SpacePartitioning/OctreesSolver.h"
#include "SGCore/Audio/AudioProcessor.h"
#include "SGCore/Motion/MotionPlannersResolver.h"
#include "SGCore/Motion/IK/IKResolver.h"

namespace SGCore::Serde
{
    // ============================================== impl ISystem
    template<FormatType TFormatType>
    void SerdeSpec<ISystem, TFormatType>::serialize(SerializableValueView<const ISystem, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<ISystem, TFormatType>::deserialize(DeserializableValueView<ISystem, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl IParallelSystem

    template<typename ParallelSystemT, FormatType TFormatType>
    void SerdeSpec<IParallelSystem<ParallelSystemT>, TFormatType>::serialize(SerializableValueView<const IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept
    {
    }

    template<typename ParallelSystemT, FormatType TFormatType>
    void SerdeSpec<IParallelSystem<ParallelSystemT>, TFormatType>::deserialize(DeserializableValueView<IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl Controllables3DUpdater

    template<FormatType TFormatType>
    void SerdeSpec<Controllables3DUpdater, TFormatType>::serialize(SerializableValueView<const Controllables3DUpdater, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<Controllables3DUpdater, TFormatType>::deserialize(DeserializableValueView<Controllables3DUpdater, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl PhysicsWorld3D

    template<FormatType TFormatType>
    void SerdeSpec<PhysicsWorld3D, TFormatType>::serialize(SerializableValueView<const PhysicsWorld3D, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<PhysicsWorld3D, TFormatType>::deserialize(DeserializableValueView<PhysicsWorld3D, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl RenderingBasesUpdater

    template<FormatType TFormatType>
    void SerdeSpec<RenderingBasesUpdater, TFormatType>::serialize(SerializableValueView<const RenderingBasesUpdater, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<RenderingBasesUpdater, TFormatType>::deserialize(DeserializableValueView<RenderingBasesUpdater, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl AtmosphereUpdater

    template<FormatType TFormatType>
    void SerdeSpec<AtmosphereUpdater, TFormatType>::serialize(SerializableValueView<const AtmosphereUpdater, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<AtmosphereUpdater, TFormatType>::deserialize(DeserializableValueView<AtmosphereUpdater, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl DirectionalLightsUpdater

    template<FormatType TFormatType>
    void SerdeSpec<DirectionalLightsUpdater, TFormatType>::serialize(SerializableValueView<const DirectionalLightsUpdater, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_maxLightsCount", valueView.m_data->m_maxLightsCount);
    }

    template<FormatType TFormatType>
    void SerdeSpec<DirectionalLightsUpdater, TFormatType>::deserialize(DeserializableValueView<DirectionalLightsUpdater, TFormatType>& valueView) noexcept
    {
        const auto m_maxLightsCount = valueView.container().template getMember<decltype(valueView.m_data->m_maxLightsCount)>("m_maxLightsCount");

        if(m_maxLightsCount)
        {
            valueView.m_data->m_maxLightsCount = *m_maxLightsCount;
        }
    }

    // =============================================== impl TransformationsUpdater

    template<FormatType TFormatType>
    void SerdeSpec<TransformationsUpdater, TFormatType>::serialize(SerializableValueView<const TransformationsUpdater, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<TransformationsUpdater, TFormatType>::deserialize(DeserializableValueView<TransformationsUpdater, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl BoxGizmosRenderer

    template<FormatType TFormatType>
    void SerdeSpec<BoxGizmosRenderer, TFormatType>::serialize(SerializableValueView<const BoxGizmosRenderer, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<BoxGizmosRenderer, TFormatType>::deserialize(DeserializableValueView<BoxGizmosRenderer, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl LineGizmosRenderer

    template<FormatType TFormatType>
    void SerdeSpec<LineGizmosRenderer, TFormatType>::serialize(SerializableValueView<const LineGizmosRenderer, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<LineGizmosRenderer, TFormatType>::deserialize(DeserializableValueView<LineGizmosRenderer, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl SphereGizmosUpdater

    template<FormatType TFormatType>
    void SerdeSpec<SphereGizmosUpdater, TFormatType>::serialize(SerializableValueView<const SphereGizmosUpdater, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<SphereGizmosUpdater, TFormatType>::deserialize(DeserializableValueView<SphereGizmosUpdater, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl OctreesSolver

    template<FormatType TFormatType>
    void SerdeSpec<OctreesSolver, TFormatType>::serialize(SerializableValueView<const OctreesSolver, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<OctreesSolver, TFormatType>::deserialize(DeserializableValueView<OctreesSolver, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl AudioProcessor

    template<FormatType TFormatType>
    void SerdeSpec<AudioProcessor, TFormatType>::serialize(SerializableValueView<const AudioProcessor, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<AudioProcessor, TFormatType>::deserialize(DeserializableValueView<AudioProcessor, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl MotionPlannersResolver

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlannersResolver, TFormatType>::serialize(SerializableValueView<const MotionPlannersResolver, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlannersResolver, TFormatType>::deserialize(DeserializableValueView<MotionPlannersResolver, TFormatType>& valueView) noexcept
    {
    }

    // =============================================== impl IKResolver

    template<FormatType TFormatType>
    void SerdeSpec<IKResolver, TFormatType>::serialize(SerializableValueView<const IKResolver, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<IKResolver, TFormatType>::deserialize(DeserializableValueView<IKResolver, TFormatType>& valueView) noexcept
    {
    }
}
