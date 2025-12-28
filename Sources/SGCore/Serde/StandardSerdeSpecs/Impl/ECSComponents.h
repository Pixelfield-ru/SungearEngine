//
// Created by stuka on 24.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/UICamera.h"
#include "SGCore/Render/Gizmos/BoxGizmo.h"
#include "SGCore/Render/Gizmos/LineGizmo.h"
#include "SGCore/Render/Gizmos/SphereGizmo.h"
#include "SGCore/Render/Lighting/SpotLight.h"
#include "SGCore/Render/Lighting/LightBase.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Physics/Rigidbody3D.h"
#include "SGCore/Motion/MotionPlanner.h"
#include "SGCore/Motion/MotionPlannerConnection.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"

namespace SGCore::Serde
{
    // ======================================================== impl EntityBaseInfo

    template<FormatType TFormatType>
    void SerdeSpec<EntityBaseInfo, TFormatType>::serialize(SerializableValueView<const EntityBaseInfo, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_thisEntity", valueView.m_data->getThisEntity());
    }

    template<FormatType TFormatType>
    void SerdeSpec<EntityBaseInfo, TFormatType>::deserialize(DeserializableValueView<EntityBaseInfo, TFormatType>& valueView) noexcept
    {
        const auto m_deserializedThisEntity = valueView.container().template getMember<ECS::entity_t>("m_thisEntity");

        if(m_deserializedThisEntity)
        {
            valueView.m_data->m_deserializedThisEntity = static_cast<ECS::entity_t>(*m_deserializedThisEntity);
        }
    }

    // ======================================================== impl Transform

    template<FormatType TFormatType>
    void SerdeSpec<Transform, TFormatType>::serialize(SerializableValueView<const Transform, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_ownTransform", valueView.m_data->m_ownTransform);
        valueView.container().addMember("m_finalTransform", valueView.m_data->m_finalTransform);
        valueView.container().addMember("m_followParentTRS", valueView.m_data->m_followParentTRS);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Transform, TFormatType>::deserialize(DeserializableValueView<Transform, TFormatType>& valueView) noexcept
    {
        const auto m_ownTransform = valueView.container().template getMember<decltype(valueView.m_data->m_ownTransform)>("m_ownTransform");
        if(m_ownTransform)
        {
            valueView.m_data->m_ownTransform = *m_ownTransform;
        }

        const auto m_finalTransform = valueView.container().template getMember<decltype(valueView.m_data->m_finalTransform)>("m_finalTransform");
        if(m_finalTransform)
        {
            valueView.m_data->m_finalTransform = *m_finalTransform;
        }

        const auto m_followParentTRS = valueView.container().template getMember<decltype(valueView.m_data->m_followParentTRS)>("m_followParentTRS");
        if(m_followParentTRS)
        {
            valueView.m_data->m_followParentTRS = *m_followParentTRS;
        }
    }

    // ======================================================== impl TransformBase

    template<FormatType TFormatType>
    void SerdeSpec<TransformBase, TFormatType>::serialize(SerializableValueView<const TransformBase, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_blockTranslation", valueView.m_data->m_blockTranslation);
        valueView.container().addMember("m_blockScale", valueView.m_data->m_blockScale);
        valueView.container().addMember("m_blockRotation", valueView.m_data->m_blockRotation);
        valueView.container().addMember("m_rotation", valueView.m_data->m_rotation);
        valueView.container().addMember("m_aabb", valueView.m_data->m_aabb);
        valueView.container().addMember("m_position", valueView.m_data->m_position);
        valueView.container().addMember("m_yawPitchRoll", valueView.m_data->m_yawPitchRoll);
        valueView.container().addMember("m_scale", valueView.m_data->m_scale);
        valueView.container().addMember("m_right", valueView.m_data->m_right);
        valueView.container().addMember("m_forward", valueView.m_data->m_forward);
        valueView.container().addMember("m_up", valueView.m_data->m_up);
    }

    template<FormatType TFormatType>
    void SerdeSpec<TransformBase, TFormatType>::deserialize(DeserializableValueView<TransformBase, TFormatType>& valueView) noexcept
    {
        const auto m_blockTranslation = valueView.container().template getMember<decltype(valueView.m_data->m_blockTranslation)>("m_blockTranslation");
        if(m_blockTranslation)
        {
            valueView.m_data->m_blockTranslation = *m_blockTranslation;
        }

        const auto m_blockScale = valueView.container().template getMember<decltype(valueView.m_data->m_blockScale)>("m_blockScale");
        if(m_blockScale)
        {
            valueView.m_data->m_blockScale = *m_blockScale;
        }

        const auto m_blockRotation = valueView.container().template getMember<decltype(valueView.m_data->m_blockRotation)>("m_blockRotation");
        if(m_blockRotation)
        {
            valueView.m_data->m_blockRotation = *m_blockRotation;
        }

        const auto m_rotation = valueView.container().template getMember<decltype(valueView.m_data->m_rotation)>("m_rotation");
        if(m_rotation)
        {
            valueView.m_data->m_rotation = *m_rotation;
        }

        const auto m_aabb = valueView.container().template getMember<decltype(valueView.m_data->m_aabb)>("m_aabb");
        if(m_aabb)
        {
            valueView.m_data->m_aabb = *m_aabb;
        }

        const auto m_position = valueView.container().template getMember<decltype(valueView.m_data->m_position)>("m_position");
        if(m_position)
        {
            valueView.m_data->m_position = *m_position;
        }

        const auto m_yawPitchRoll = valueView.container().template getMember<decltype(valueView.m_data->m_yawPitchRoll)>("m_yawPitchRoll");
        if(m_yawPitchRoll)
        {
            valueView.m_data->m_yawPitchRoll = *m_yawPitchRoll;
        }

        const auto m_scale = valueView.container().template getMember<decltype(valueView.m_data->m_scale)>("m_scale");
        if(m_scale)
        {
            valueView.m_data->m_scale = *m_scale;
        }

        const auto m_right = valueView.container().template getMember<decltype(valueView.m_data->m_right)>("m_right");
        if(m_right)
        {
            valueView.m_data->m_right = *m_right;
        }

        const auto m_forward = valueView.container().template getMember<decltype(valueView.m_data->m_forward)>("m_forward");
        if(m_forward)
        {
            valueView.m_data->m_forward = *m_forward;
        }

        const auto m_up = valueView.container().template getMember<decltype(valueView.m_data->m_up)>("m_up");
        if(m_up)
        {
            valueView.m_data->m_up = *m_up;
        }
    }

    // ======================================================== impl Pickable

    template<FormatType TFormatType>
    void SerdeSpec<Pickable, TFormatType>::serialize(SerializableValueView<const Pickable, TFormatType>& valueView,
                                                     const ECS::entity_t& deserializableEntity,
                                                     ECS::registry_t& toRegistry) noexcept
    {
        valueView.container().addMember("m_pickableForCameras", valueView.m_data->m_pickableForCameras,
                                                deserializableEntity, toRegistry);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Pickable, TFormatType>::deserialize(DeserializableValueView<Pickable, TFormatType>& valueView,
                                                       const ECS::entity_t& deserializableEntity,
                                                       ECS::registry_t& toRegistry) noexcept
    {
        auto m_pickableForCameras = valueView.container().template getMember<decltype(Pickable::m_pickableForCameras)>(
                "m_pickableForCameras", deserializableEntity, toRegistry
        );

        if(m_pickableForCameras)
        {
            valueView.m_data->m_pickableForCameras = std::move(*m_pickableForCameras);
        }
    }

    // ======================================================== impl AABB

    template<typename ScalarT, FormatType TFormatType>
    void SerdeSpec<AABB<ScalarT>, TFormatType>::serialize(SerializableValueView<const AABB<ScalarT>, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_min", valueView.m_data->m_min);
        valueView.container().addMember("m_max", valueView.m_data->m_max);
    }

    template<typename ScalarT,FormatType TFormatType>
    void SerdeSpec<AABB<ScalarT>, TFormatType>::deserialize(DeserializableValueView<AABB<ScalarT>, TFormatType>& valueView) noexcept
    {
        const auto m_min = valueView.container().template getMember<decltype(valueView.m_data->m_min)>("m_min");
        if(m_min)
        {
            valueView.m_data->m_min = *m_min;
        }

        const auto m_max = valueView.container().template getMember<decltype(valueView.m_data->m_max)>("m_max");
        if(m_max)
        {
            valueView.m_data->m_max = *m_max;
        }
    }

    // ======================================================== impl RenderingBase

    template<FormatType TFormatType>
    void SerdeSpec<RenderingBase, TFormatType>::serialize(SerializableValueView<const RenderingBase, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_zFar", valueView.m_data->m_zFar);
        valueView.container().addMember("m_fov", valueView.m_data->m_fov);
        valueView.container().addMember("m_aspect", valueView.m_data->m_aspect);
        valueView.container().addMember("m_zNear", valueView.m_data->m_zNear);
        valueView.container().addMember("m_left", valueView.m_data->m_left);
        valueView.container().addMember("m_right", valueView.m_data->m_right);
        valueView.container().addMember("m_bottom", valueView.m_data->m_bottom);
        valueView.container().addMember("m_top", valueView.m_data->m_top);
        valueView.container().addMember("m_projectionMatrix", valueView.m_data->m_projectionMatrix);
        valueView.container().addMember("m_orthographicMatrix", valueView.m_data->m_orthographicMatrix);
        valueView.container().addMember("m_viewMatrix", valueView.m_data->m_viewMatrix);
        valueView.container().addMember("m_projectionSpaceMatrix", valueView.m_data->m_projectionSpaceMatrix);
        valueView.container().addMember("m_orthographicSpaceMatrix", valueView.m_data->m_orthographicSpaceMatrix);
    }

    template<FormatType TFormatType>
    void SerdeSpec<RenderingBase, TFormatType>::deserialize(DeserializableValueView<RenderingBase, TFormatType>& valueView) noexcept
    {
        const auto m_zFar = valueView.container().template getMember<decltype(valueView.m_data->m_zFar)>("m_zFar");
        if(m_zFar)
        {
            valueView.m_data->m_zFar = *m_zFar;
        }

        const auto m_fov = valueView.container().template getMember<decltype(valueView.m_data->m_fov)>("m_fov");
        if(m_fov)
        {
            valueView.m_data->m_fov = *m_fov;
        }

        const auto m_aspect = valueView.container().template getMember<decltype(valueView.m_data->m_aspect)>("m_aspect");
        if(m_aspect)
        {
            valueView.m_data->m_aspect = *m_aspect;
        }

        const auto m_zNear = valueView.container().template getMember<decltype(valueView.m_data->m_zNear)>("m_zNear");
        if(m_zNear)
        {
            valueView.m_data->m_zNear = *m_zNear;
        }

        const auto m_left = valueView.container().template getMember<decltype(valueView.m_data->m_left)>("m_left");
        if(m_left)
        {
            valueView.m_data->m_left = *m_left;
        }

        const auto m_right = valueView.container().template getMember<decltype(valueView.m_data->m_right)>("m_right");
        if(m_right)
        {
            valueView.m_data->m_right = *m_right;
        }

        const auto m_bottom = valueView.container().template getMember<decltype(valueView.m_data->m_bottom)>("m_bottom");
        if(m_bottom)
        {
            valueView.m_data->m_bottom = *m_bottom;
        }

        const auto m_top = valueView.container().template getMember<decltype(valueView.m_data->m_top)>("m_top");
        if(m_top)
        {
            valueView.m_data->m_top = *m_top;
        }

        const auto m_projectionMatrix = valueView.container().template getMember<decltype(valueView.m_data->m_projectionMatrix)>("m_projectionMatrix");
        if(m_projectionMatrix)
        {
            valueView.m_data->m_projectionMatrix = *m_projectionMatrix;
        }

        const auto m_orthographicMatrix = valueView.container().template getMember<decltype(valueView.m_data->m_orthographicMatrix)>("m_orthographicMatrix");
        if(m_orthographicMatrix)
        {
            valueView.m_data->m_orthographicMatrix = *m_orthographicMatrix;
        }

        const auto m_viewMatrix = valueView.container().template getMember<decltype(valueView.m_data->m_viewMatrix)>("m_viewMatrix");
        if(m_viewMatrix)
        {
            valueView.m_data->m_viewMatrix = *m_viewMatrix;
        }

        const auto m_projectionSpaceMatrix = valueView.container().template getMember<decltype(valueView.m_data->m_projectionSpaceMatrix)>("m_projectionSpaceMatrix");
        if(m_projectionSpaceMatrix)
        {
            valueView.m_data->m_projectionSpaceMatrix = *m_projectionSpaceMatrix;
        }

        const auto m_orthographicSpaceMatrix = valueView.container().template getMember<decltype(valueView.m_data->m_orthographicSpaceMatrix)>("m_orthographicSpaceMatrix");
        if(m_orthographicSpaceMatrix)
        {
            valueView.m_data->m_orthographicSpaceMatrix = *m_orthographicSpaceMatrix;
        }
    }

    // ======================================================== impl AudioSource

    template<FormatType TFormatType>
    void SerdeSpec<AudioSource, TFormatType>::serialize(SerializableValueView<const AudioSource, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_gain", valueView.m_data->getGain());
        valueView.container().addMember("m_rolloffFactor", valueView.m_data->getRolloffFactor());
        valueView.container().addMember("m_pitch", valueView.m_data->getPitch());
        valueView.container().addMember("m_position", valueView.m_data->getPosition());
        valueView.container().addMember("m_velocity", valueView.m_data->getVelocity());
        valueView.container().addMember("m_direction", valueView.m_data->getDirection());
        valueView.container().addMember("m_isLooping", valueView.m_data->isLooping());
        valueView.container().addMember("m_lastState", valueView.m_data->getState());
        valueView.container().addMember("m_type", valueView.m_data->getType());
    }

    template<FormatType TFormatType>
    void SerdeSpec<AudioSource, TFormatType>::deserialize(DeserializableValueView<AudioSource, TFormatType>& valueView) noexcept
    {
        const auto m_gain = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getGain())>>>("m_gain");
        if(m_gain)
        {
            valueView.m_data->setGain(*m_gain);
        }

        const auto m_rolloffFactor = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getRolloffFactor())>>>("m_rolloffFactor");
        if(m_rolloffFactor)
        {
            valueView.m_data->setRolloffFactor(*m_rolloffFactor);
        }

        const auto m_pitch = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getPitch())>>>("m_pitch");
        if(m_pitch)
        {
            valueView.m_data->setPitch(*m_pitch);
        }

        const auto m_position = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getPosition())>>>("m_position");
        if(m_position)
        {
            valueView.m_data->setPosition(*m_position);
        }

        const auto m_velocity = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getVelocity())>>>("m_velocity");
        if(m_velocity)
        {
            valueView.m_data->setVelocity(*m_velocity);
        }

        const auto m_direction = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getDirection())>>>("m_direction");
        if(m_direction)
        {
            valueView.m_data->setDirection(*m_direction);
        }

        const auto m_isLooping = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->isLooping())>>>("m_isLooping");
        if(m_isLooping)
        {
            valueView.m_data->setIsLooping(*m_isLooping);
        }

        const auto m_lastState = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getState())>>>("m_lastState");
        if(m_lastState)
        {
            valueView.m_data->setState(*m_lastState);
        }

        const auto m_type = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getType())>>>("m_type");
        if(m_type)
        {
            valueView.m_data->setType(*m_type);
        }
    }

    // ======================================================== impl Atmosphere

    template<FormatType TFormatType>
    void SerdeSpec<Atmosphere, TFormatType>::serialize(SerializableValueView<const Atmosphere, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_sunPosition", valueView.m_data->m_sunPosition);
        valueView.container().addMember("m_sunAmbient", valueView.m_data->m_sunAmbient);
        valueView.container().addMember("m_rayleighScatteringCoeff", valueView.m_data->m_rayleighScatteringCoeff);
        valueView.container().addMember("m_atmosphereRadius", valueView.m_data->m_atmosphereRadius);
        valueView.container().addMember("m_sunRotation", valueView.m_data->m_sunRotation);
        valueView.container().addMember("m_mieScatteringCoeff", valueView.m_data->m_mieScatteringCoeff);
        valueView.container().addMember("m_rayleighScaleHeight", valueView.m_data->m_rayleighScaleHeight);
        valueView.container().addMember("m_mieScaleHeight", valueView.m_data->m_mieScaleHeight);
        valueView.container().addMember("m_sunIntensity", valueView.m_data->m_sunIntensity);
        valueView.container().addMember("m_planetRadius", valueView.m_data->m_planetRadius);
        valueView.container().addMember("m_miePreferredScatteringDirection", valueView.m_data->m_miePreferredScatteringDirection);
        valueView.container().addMember("m_rayOrigin", valueView.m_data->m_rayOrigin);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Atmosphere, TFormatType>::deserialize(DeserializableValueView<Atmosphere, TFormatType>& valueView) noexcept
    {
        const auto m_sunPosition = valueView.container().template getMember<decltype(valueView.m_data->m_sunPosition)>("m_sunPosition");
        if(m_sunPosition)
        {
            valueView.m_data->m_sunPosition = *m_sunPosition;
        }

        const auto m_sunAmbient = valueView.container().template getMember<decltype(valueView.m_data->m_sunAmbient)>("m_sunAmbient");
        if(m_sunAmbient)
        {
            valueView.m_data->m_sunAmbient = *m_sunAmbient;
        }

        const auto m_rayleighScatteringCoeff = valueView.container().template getMember<decltype(valueView.m_data->m_rayleighScatteringCoeff)>("m_rayleighScatteringCoeff");
        if(m_rayleighScatteringCoeff)
        {
            valueView.m_data->m_rayleighScatteringCoeff = *m_rayleighScatteringCoeff;
        }

        const auto m_atmosphereRadius = valueView.container().template getMember<decltype(valueView.m_data->m_atmosphereRadius)>("m_atmosphereRadius");
        if(m_atmosphereRadius)
        {
            valueView.m_data->m_atmosphereRadius = *m_atmosphereRadius;
        }

        const auto m_sunRotation = valueView.container().template getMember<decltype(valueView.m_data->m_sunRotation)>("m_sunRotation");
        if(m_sunRotation)
        {
            valueView.m_data->m_sunRotation = *m_sunRotation;
        }

        const auto m_mieScatteringCoeff = valueView.container().template getMember<decltype(valueView.m_data->m_mieScatteringCoeff)>("m_mieScatteringCoeff");
        if(m_mieScatteringCoeff)
        {
            valueView.m_data->m_mieScatteringCoeff = *m_mieScatteringCoeff;
        }

        const auto m_rayleighScaleHeight = valueView.container().template getMember<decltype(valueView.m_data->m_rayleighScaleHeight)>("m_rayleighScaleHeight");
        if(m_rayleighScaleHeight)
        {
            valueView.m_data->m_rayleighScaleHeight = *m_rayleighScaleHeight;
        }

        const auto m_mieScaleHeight = valueView.container().template getMember<decltype(valueView.m_data->m_mieScaleHeight)>("m_mieScaleHeight");
        if(m_mieScaleHeight)
        {
            valueView.m_data->m_mieScaleHeight = *m_mieScaleHeight;
        }

        const auto m_sunIntensity = valueView.container().template getMember<decltype(valueView.m_data->m_sunIntensity)>("m_sunIntensity");
        if(m_sunIntensity)
        {
            valueView.m_data->m_sunIntensity = *m_sunIntensity;
        }

        const auto m_planetRadius = valueView.container().template getMember<decltype(valueView.m_data->m_planetRadius)>("m_planetRadius");
        if(m_planetRadius)
        {
            valueView.m_data->m_planetRadius = *m_planetRadius;
        }

        const auto m_miePreferredScatteringDirection = valueView.container().template getMember<decltype(valueView.m_data->m_miePreferredScatteringDirection)>("m_miePreferredScatteringDirection");
        if(m_miePreferredScatteringDirection)
        {
            valueView.m_data->m_miePreferredScatteringDirection = *m_miePreferredScatteringDirection;
        }

        const auto m_rayOrigin = valueView.container().template getMember<decltype(valueView.m_data->m_rayOrigin)>("m_rayOrigin");
        if(m_rayOrigin)
        {
            valueView.m_data->m_rayOrigin = *m_rayOrigin;
        }
    }

    // ======================================================== impl SphereGizmo

    template<FormatType TFormatType>
    void SerdeSpec<SphereGizmo, TFormatType>::serialize(SerializableValueView<const SphereGizmo, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_base", valueView.m_data->m_base);
        valueView.container().addMember("m_radius", valueView.m_data->m_radius);
        valueView.container().addMember("m_angleIncrement", valueView.m_data->m_angleIncrement);
    }

    template<FormatType TFormatType>
    void SerdeSpec<SphereGizmo, TFormatType>::deserialize(DeserializableValueView<SphereGizmo, TFormatType>& valueView) noexcept
    {
        const auto m_base = valueView.container().template getMember<decltype(valueView.m_data->m_base)>("m_base");
        if(m_base)
        {
            valueView.m_data->m_base = *m_base;
        }

        const auto m_radius = valueView.container().template getMember<decltype(valueView.m_data->m_radius)>("m_radius");
        if(m_radius)
        {
            valueView.m_data->m_radius = *m_radius;
        }

        const auto m_angleIncrement = valueView.container().template getMember<decltype(valueView.m_data->m_angleIncrement)>("m_angleIncrement");
        if(m_angleIncrement)
        {
            valueView.m_data->m_angleIncrement = *m_angleIncrement;
        }
    }

    // ======================================================== impl BoxGizmo

    template<FormatType TFormatType>
    void SerdeSpec<BoxGizmo, TFormatType>::serialize(SerializableValueView<const BoxGizmo, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_base", valueView.m_data->m_base);
        valueView.container().addMember("m_size", valueView.m_data->m_size);
    }

    template<FormatType TFormatType>
    void SerdeSpec<BoxGizmo, TFormatType>::deserialize(DeserializableValueView<BoxGizmo, TFormatType>& valueView) noexcept
    {
        const auto m_base = valueView.container().template getMember<decltype(valueView.m_data->m_base)>("m_base");
        if(m_base)
        {
            valueView.m_data->m_base = *m_base;
        }

        const auto m_size = valueView.container().template getMember<decltype(valueView.m_data->m_size)>("m_size");
        if(m_size)
        {
            valueView.m_data->m_size = *m_size;
        }
    }

    // ======================================================== impl GizmoBase

    template<FormatType TFormatType>
    void SerdeSpec<GizmoBase, TFormatType>::serialize(SerializableValueView<const GizmoBase, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_color", valueView.m_data->m_color);
        valueView.container().addMember("m_meshBase", valueView.m_data->m_meshBase);
    }

    template<FormatType TFormatType>
    void SerdeSpec<GizmoBase, TFormatType>::deserialize(DeserializableValueView<GizmoBase, TFormatType>& valueView) noexcept
    {
        const auto m_color = valueView.container().template getMember<decltype(valueView.m_data->m_color)>("m_color");
        if(m_color)
        {
            valueView.m_data->m_color = *m_color;
        }

        const auto m_meshBase = valueView.container().template getMember<decltype(valueView.m_data->m_meshBase)>("m_meshBase");
        if(m_meshBase)
        {
            valueView.m_data->m_meshBase = *m_meshBase;
        }
    }

    // ======================================================== impl LineGizmo

    template<FormatType TFormatType>
    void SerdeSpec<LineGizmo, TFormatType>::serialize(SerializableValueView<const LineGizmo, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_base", valueView.m_data->m_base);
        valueView.container().addMember("m_start", valueView.m_data->m_start);
        valueView.container().addMember("m_end", valueView.m_data->m_end);
    }

    template<FormatType TFormatType>
    void SerdeSpec<LineGizmo, TFormatType>::deserialize(DeserializableValueView<LineGizmo, TFormatType>& valueView) noexcept
    {
        const auto m_base = valueView.container().template getMember<decltype(valueView.m_data->m_base)>("m_base");
        if(m_base)
        {
            valueView.m_data->m_base = *m_base;
        }

        const auto m_start = valueView.container().template getMember<decltype(valueView.m_data->m_start)>("m_start");
        if(m_start)
        {
            valueView.m_data->m_start = *m_start;
        }

        const auto m_end = valueView.container().template getMember<decltype(valueView.m_data->m_end)>("m_end");
        if(m_end)
        {
            valueView.m_data->m_end = *m_end;
        }
    }

    // ======================================================== impl MeshBase

    template<FormatType TFormatType>
    void SerdeSpec<MeshBase, TFormatType>::serialize(SerializableValueView<const MeshBase, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_material", valueView.m_data->getMaterial());
        valueView.container().addMember("m_meshData", valueView.m_data->getMeshData());
    }

    template<FormatType TFormatType>
    void SerdeSpec<MeshBase, TFormatType>::deserialize(DeserializableValueView<MeshBase, TFormatType>& valueView) noexcept
    {
        const auto m_meshData = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMeshData())>>>("m_meshData");
        if(m_meshData)
        {
            LOG_D(SGCORE_TAG, "Deserializing meshdata from mesh base...");
            valueView.m_data->setMeshData(*m_meshData);
        }

        // WE DESERIALIZING MATERIAL STRICTLY AFTER m_meshData BECAUSE MATERIAL CAN BE CUSTOM
        const auto m_material = valueView.container().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMaterial())>>>("m_material");
        if(m_material)
        {
            valueView.m_data->setMaterial(*m_material);
        }
    }

    // ======================================================== impl Mesh

    template<FormatType TFormatType>
    void SerdeSpec<Mesh, TFormatType>::serialize(SerializableValueView<const Mesh, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_base", valueView.m_data->m_base);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Mesh, TFormatType>::deserialize(DeserializableValueView<Mesh, TFormatType>& valueView) noexcept
    {
        const auto m_base = valueView.container().template getMember<decltype(valueView.m_data->m_base)>("m_base");

        if(m_base)
        {
            valueView.m_data->m_base = *m_base;
        }
    }

    // ======================================================== impl LightBase

    template<FormatType TFormatType>
    void SerdeSpec<LightBase, TFormatType>::serialize(SerializableValueView<const LightBase, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_color", valueView.m_data->m_color);
        valueView.container().addMember("m_intensity", valueView.m_data->m_intensity);
        valueView.container().addMember("m_samplesCount", valueView.m_data->m_samplesCount);
    }

    template<FormatType TFormatType>
    void SerdeSpec<LightBase, TFormatType>::deserialize(DeserializableValueView<LightBase, TFormatType>& valueView) noexcept
    {
        const auto m_color = valueView.container().template getMember<decltype(valueView.m_data->m_color)>("m_color");
        if(m_color)
        {
            valueView.m_data->m_color = *m_color;
        }

        const auto m_intensity = valueView.container().template getMember<decltype(valueView.m_data->m_intensity)>("m_intensity");
        if(m_intensity)
        {
            valueView.m_data->m_intensity = *m_intensity;
        }

        const auto m_samplesCount = valueView.container().template getMember<decltype(valueView.m_data->m_samplesCount)>("m_samplesCount");
        if(m_samplesCount)
        {
            valueView.m_data->m_samplesCount = *m_samplesCount;
        }
    }

    // ======================================================== impl SpotLight

    template<FormatType TFormatType>
    void SerdeSpec<SpotLight, TFormatType>::serialize(SerializableValueView<const SpotLight, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_base", valueView.m_data->m_base);
    }

    template<FormatType TFormatType>
    void SerdeSpec<SpotLight, TFormatType>::deserialize(DeserializableValueView<SpotLight, TFormatType>& valueView) noexcept
    {
        const auto m_base = valueView.container().template getMember<decltype(valueView.m_data->m_base)>("m_base");
        if(m_base)
        {
            valueView.m_data->m_base = *m_base;
        }
    }

    // ======================================================== impl Controllable3D

    template<FormatType TFormatType>
    void SerdeSpec<Controllable3D, TFormatType>::serialize(SerializableValueView<const Controllable3D, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_movementSpeed", valueView.m_data->m_movementSpeed);
        valueView.container().addMember("m_rotationSensitive", valueView.m_data->m_rotationSensitive);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Controllable3D, TFormatType>::deserialize(DeserializableValueView<Controllable3D, TFormatType>& valueView) noexcept
    {
        const auto m_movementSpeed = valueView.container().template getMember<decltype(valueView.m_data->m_movementSpeed)>("m_movementSpeed");
        if(m_movementSpeed)
        {
            valueView.m_data->m_movementSpeed = *m_movementSpeed;
        }

        const auto m_rotationSensitive = valueView.container().template getMember<decltype(valueView.m_data->m_rotationSensitive)>("m_rotationSensitive");
        if(m_rotationSensitive)
        {
            valueView.m_data->m_rotationSensitive = *m_rotationSensitive;
        }
    }

    // ======================================================== impl Camera3D

    template<FormatType TFormatType>
    void SerdeSpec<Camera3D, TFormatType>::serialize(SerializableValueView<const Camera3D, TFormatType>& valueView) noexcept
    {

    }

    template<FormatType TFormatType>
    void SerdeSpec<Camera3D, TFormatType>::deserialize(DeserializableValueView<Camera3D, TFormatType>& valueView) noexcept
    {

    }

    // ======================================================== impl UICamera

    template<FormatType TFormatType>
    void SerdeSpec<UICamera, TFormatType>::serialize(SerializableValueView<const UICamera, TFormatType>& valueView) noexcept
    {

    }

    template<FormatType TFormatType>
    void SerdeSpec<UICamera, TFormatType>::deserialize(DeserializableValueView<UICamera, TFormatType>& valueView) noexcept
    {

    }

    // ======================================================== impl TransparentEntityTag

    template<FormatType TFormatType>
    void SerdeSpec<TransparentEntityTag, TFormatType>::serialize(SerializableValueView<const TransparentEntityTag, TFormatType>& valueView) noexcept
    {

    }

    template<FormatType TFormatType>
    void SerdeSpec<TransparentEntityTag, TFormatType>::deserialize(DeserializableValueView<TransparentEntityTag, TFormatType>& valueView) noexcept
    {

    }

    // ======================================================== impl OpaqueEntityTag

    template<FormatType TFormatType>
    void SerdeSpec<OpaqueEntityTag, TFormatType>::serialize(SerializableValueView<const OpaqueEntityTag, TFormatType>& valueView) noexcept
    {

    }

    template<FormatType TFormatType>
    void SerdeSpec<OpaqueEntityTag, TFormatType>::deserialize(DeserializableValueView<OpaqueEntityTag, TFormatType>& valueView) noexcept
    {

    }

    // ======================================================== impl MotionPlanner

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlanner, TFormatType>::serialize(SerializableValueView<const MotionPlanner, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_rootNodes", valueView.m_data->m_rootNodes);
        valueView.container().addMember("m_skeleton", valueView.m_data->m_skeleton);
        valueView.container().addMember("m_maxBonesPerMesh", valueView.m_data->m_maxBonesPerMesh);
    }

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlanner, TFormatType>::deserialize(DeserializableValueView<MotionPlanner, TFormatType>& valueView) noexcept
    {
        auto rootNodes = valueView.container().template getMember<decltype(MotionPlanner::m_rootNodes)>("m_rootNodes");
        if(rootNodes)
        {
            valueView.m_data->m_rootNodes = std::move(*rootNodes);
        }

        auto skeleton = valueView.container().template getMember<decltype(MotionPlanner::m_skeleton)>("m_skeleton");
        if(skeleton)
        {
            valueView.m_data->m_skeleton = std::move(*skeleton);
        }

        auto maxBonesPerMesh = valueView.container().template getMember<std::int32_t>("m_maxBonesPerMesh");
        if(maxBonesPerMesh)
        {
            valueView.m_data->m_maxBonesPerMesh = std::move(*maxBonesPerMesh);
        }
    }

    // ======================================================== impl MotionPlannerNode

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlannerNode, TFormatType>::serialize(SerializableValueView<const MotionPlannerNode, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_connections", valueView.m_data->m_connections);
        valueView.container().addMember("m_skeletalAnimation", valueView.m_data->m_skeletalAnimation);
        valueView.container().addMember("m_isActive", valueView.m_data->m_isActive);
        // valueView.container().addMember("m_isPaused", valueView.m_data->m_isPaused);
        // valueView.container().addMember("m_isPlaying", valueView.m_data->m_isPlaying);
        valueView.container().addMember("m_isRepeated", valueView.m_data->m_isRepeated);
        valueView.container().addMember("m_animationSpeed", valueView.m_data->m_animationSpeed);
    }

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlannerNode, TFormatType>::deserialize(DeserializableValueView<MotionPlannerNode, TFormatType>& valueView) noexcept
    {
        auto connections = valueView.container().template getMember<decltype(MotionPlannerNode::m_connections)>("m_connections");
        if(connections)
        {
            valueView.m_data->m_connections = std::move(*connections);
        }

        // setup connections
        for(const auto& connection : valueView.m_data->m_connections)
        {
            // USE ONLY weak_from_this(). DO NOT USE shared_from_this()
            connection->m_previousNode = valueView.m_data->weak_from_this();
        }

        auto skeletalAnimation = valueView.container().template getMember<decltype(MotionPlannerNode::m_skeletalAnimation)>("m_skeletalAnimation");
        if(skeletalAnimation)
        {
            valueView.m_data->m_skeletalAnimation = std::move(*skeletalAnimation);
        }

        auto isActive = valueView.container().template getMember<bool>("m_isActive");
        if(isActive)
        {
            valueView.m_data->m_isActive = std::move(*isActive);
        }

        auto isRepeated = valueView.container().template getMember<bool>("m_isRepeated");
        if(isRepeated)
        {
            valueView.m_data->m_isRepeated = std::move(*isRepeated);
        }

        auto animationSpeed = valueView.container().template getMember<float>("m_animationSpeed");
        if(animationSpeed)
        {
            valueView.m_data->m_animationSpeed = std::move(*animationSpeed);
        }
    }

    // ======================================================== impl MotionPlannerConnection

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlannerConnection, TFormatType>::serialize(SerializableValueView<const MotionPlannerConnection, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_blendTime", valueView.m_data->m_blendTime);
        valueView.container().addMember("m_blendSpeed", valueView.m_data->m_blendSpeed);
        valueView.container().addMember("m_activationAction", valueView.m_data->m_activationAction);
        // serializing only next node because m_previousNode owns this connection
        valueView.container().addMember("m_nextNode", valueView.m_data->m_nextNode);
    }

    template<FormatType TFormatType>
    void SerdeSpec<MotionPlannerConnection, TFormatType>::deserialize(DeserializableValueView<MotionPlannerConnection, TFormatType>& valueView) noexcept
    {
        auto blendTime = valueView.container().template getMember<float>("m_blendTime");
        if(blendTime)
        {
            valueView.m_data->m_blendTime = std::move(*blendTime);
        }

        auto blendSpeed = valueView.container().template getMember<float>("m_blendSpeed");
        if(blendSpeed)
        {
            valueView.m_data->m_blendSpeed = std::move(*blendSpeed);
        }

        auto activationAction = valueView.container().template getMember<decltype(MotionPlannerConnection::m_activationAction)>("m_activationAction");
        if(activationAction)
        {
            valueView.m_data->m_activationAction = std::move(*activationAction);
        }

        auto nextNode = valueView.container().template getMember<decltype(MotionPlannerConnection::m_nextNode)>("m_nextNode");
        if(nextNode)
        {
            valueView.m_data->m_nextNode = std::move(*nextNode);
        }
    }

    // ======================================================== impl Rigidbody3D

    template<FormatType TFormatType>
    void SerdeSpec<Rigidbody3D, TFormatType>::serialize(SerializableValueView<const Rigidbody3D, TFormatType>& valueView) noexcept
    {
        auto& valueContainer = valueView.container();
        auto& value = valueView.m_data;

        valueContainer.addMember("m_mass", value->m_body->getMass());

        const auto inertia = value->m_body->getLocalInertia();
        valueContainer.addMember("m_inertia", inertia);

        valueContainer.addMember("m_finalShape", static_cast<btCollisionShape*>(value->m_finalShape.get()), btTransform {}, *valueView.m_data);

        valueContainer.addMember("m_type", value->getType());
    }

    template<FormatType TFormatType>
    void SerdeSpec<Rigidbody3D, TFormatType>::deserialize(DeserializableValueView<Rigidbody3D, TFormatType>& valueView) noexcept
    {
        const auto mass = valueView.container().template getMember<btScalar>("m_mass");
        const auto localInertia = valueView.container().template getMember<btVector3>("m_inertia");

        if(mass && localInertia)
        {
            valueView.m_data->m_body->setMassProps(*mass, *localInertia);
        }

        // deserializing final shape (compound shape)
        btTransform finalShapeTransform; // unused
        auto finalShape = valueView.container().template getMember<Ref<btCollisionShape>>("m_finalShape", finalShapeTransform, *valueView.m_data);
        if(finalShape && (*finalShape)->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
        {
            valueView.m_data->m_finalShape = std::move(std::static_pointer_cast<btCompoundShape>(*finalShape));
            valueView.m_data->m_body->setCollisionShape(finalShape->get());
        }

        const auto type = valueView.container().template getMember<PhysicalObjectType>("m_type");
        if(type)
        {
            valueView.m_data->setType(*type);
        }
    }

    // ======================================================== impl Rigidbody3D

    template<FormatType TFormatType>
    void SerdeSpec<IKRootJoint, TFormatType>::serialize(SerializableValueView<const IKRootJoint, TFormatType>& valueView) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<IKRootJoint, TFormatType>::deserialize(DeserializableValueView<IKRootJoint, TFormatType>& valueView) noexcept
    {
    }

    // ======================================================== impl IKJoint

    template<FormatType TFormatType>
    void SerdeSpec<IKJoint, TFormatType>::serialize(SerializableValueView<const IKJoint, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_isEndJoint", valueView.m_data->m_isEndJoint);
    }

    template<FormatType TFormatType>
    void SerdeSpec<IKJoint, TFormatType>::deserialize(DeserializableValueView<IKJoint, TFormatType>& valueView) noexcept
    {
        const auto isEndJoint = valueView.container().template getMember<bool>("m_isEndJoint");
        if(isEndJoint)
        {
            valueView.m_data->m_isEndJoint = *isEndJoint;
        }
    }
}