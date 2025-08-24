//
// Created by stuka on 24.08.2025.
//

#pragma once

namespace SGCore::Serde
{
    // ======================================================== impl EntityBaseInfo

    template<FormatType TFormatType>
    void SerdeSpec<EntityBaseInfo, TFormatType>::serialize(SerializableValueView<const EntityBaseInfo, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_thisEntity", valueView.m_data->getThisEntity());
    }

    template<FormatType TFormatType>
    void SerdeSpec<EntityBaseInfo, TFormatType>::deserialize(DeserializableValueView<EntityBaseInfo, TFormatType>& valueView) noexcept
    {
        const auto m_deserializedThisEntity = valueView.getValueContainer().template getMember<ECS::entity_t>("m_thisEntity");

        if(m_deserializedThisEntity)
        {
            valueView.m_data->m_deserializedThisEntity = static_cast<ECS::entity_t>(*m_deserializedThisEntity);
        }
    }

    // ======================================================== impl Transform

    template<FormatType TFormatType>
    void SerdeSpec<Transform, TFormatType>::serialize(SerializableValueView<const Transform, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_ownTransform", valueView.m_data->m_ownTransform);
        valueView.getValueContainer().addMember("m_finalTransform", valueView.m_data->m_finalTransform);
        valueView.getValueContainer().addMember("m_followParentTRS", valueView.m_data->m_followParentTRS);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Transform, TFormatType>::deserialize(DeserializableValueView<Transform, TFormatType>& valueView) noexcept
    {
        const auto m_ownTransform = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_ownTransform)>("m_ownTransform");
        if(m_ownTransform)
        {
            valueView.m_data->m_ownTransform = *m_ownTransform;
        }

        const auto m_finalTransform = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_finalTransform)>("m_finalTransform");
        if(m_finalTransform)
        {
            valueView.m_data->m_finalTransform = *m_finalTransform;
        }

        const auto m_followParentTRS = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_followParentTRS)>("m_followParentTRS");
        if(m_followParentTRS)
        {
            valueView.m_data->m_followParentTRS = *m_followParentTRS;
        }
    }

    // ======================================================== impl TransformBase

    template<FormatType TFormatType>
    void SerdeSpec<TransformBase, TFormatType>::serialize(SerializableValueView<const TransformBase, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_blockTranslation", valueView.m_data->m_blockTranslation);
        valueView.getValueContainer().addMember("m_blockScale", valueView.m_data->m_blockScale);
        valueView.getValueContainer().addMember("m_blockRotation", valueView.m_data->m_blockRotation);
        valueView.getValueContainer().addMember("m_rotation", valueView.m_data->m_rotation);
        valueView.getValueContainer().addMember("m_aabb", valueView.m_data->m_aabb);
        valueView.getValueContainer().addMember("m_position", valueView.m_data->m_position);
        valueView.getValueContainer().addMember("m_yawPitchRoll", valueView.m_data->m_yawPitchRoll);
        valueView.getValueContainer().addMember("m_scale", valueView.m_data->m_scale);
        valueView.getValueContainer().addMember("m_right", valueView.m_data->m_right);
        valueView.getValueContainer().addMember("m_forward", valueView.m_data->m_forward);
        valueView.getValueContainer().addMember("m_up", valueView.m_data->m_up);
    }

    template<FormatType TFormatType>
    void SerdeSpec<TransformBase, TFormatType>::deserialize(DeserializableValueView<TransformBase, TFormatType>& valueView) noexcept
    {
        const auto m_blockTranslation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_blockTranslation)>("m_blockTranslation");
        if(m_blockTranslation)
        {
            valueView.m_data->m_blockTranslation = *m_blockTranslation;
        }

        const auto m_blockScale = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_blockScale)>("m_blockScale");
        if(m_blockScale)
        {
            valueView.m_data->m_blockScale = *m_blockScale;
        }

        const auto m_blockRotation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_blockRotation)>("m_blockRotation");
        if(m_blockRotation)
        {
            valueView.m_data->m_blockRotation = *m_blockRotation;
        }

        const auto m_rotation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rotation)>("m_rotation");
        if(m_rotation)
        {
            valueView.m_data->m_rotation = *m_rotation;
        }

        const auto m_aabb = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_aabb)>("m_aabb");
        if(m_aabb)
        {
            valueView.m_data->m_aabb = *m_aabb;
        }

        const auto m_position = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_position)>("m_position");
        if(m_position)
        {
            valueView.m_data->m_position = *m_position;
        }

        const auto m_yawPitchRoll = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_yawPitchRoll)>("m_yawPitchRoll");
        if(m_yawPitchRoll)
        {
            valueView.m_data->m_yawPitchRoll = *m_yawPitchRoll;
        }

        const auto m_scale = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_scale)>("m_scale");
        if(m_scale)
        {
            valueView.m_data->m_scale = *m_scale;
        }

        const auto m_right = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_right)>("m_right");
        if(m_right)
        {
            valueView.m_data->m_right = *m_right;
        }

        const auto m_forward = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_forward)>("m_forward");
        if(m_forward)
        {
            valueView.m_data->m_forward = *m_forward;
        }

        const auto m_up = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_up)>("m_up");
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
        valueView.getValueContainer().addMember("m_pickableForCameras", valueView.m_data->m_pickableForCameras,
                                                deserializableEntity, toRegistry);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Pickable, TFormatType>::deserialize(DeserializableValueView<Pickable, TFormatType>& valueView,
                                                       const ECS::entity_t& deserializableEntity,
                                                       ECS::registry_t& toRegistry) noexcept
    {
        auto m_pickableForCameras = valueView.getValueContainer().template getMember<decltype(Pickable::m_pickableForCameras)>(
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
        valueView.getValueContainer().addMember("m_min", valueView.m_data->m_min);
        valueView.getValueContainer().addMember("m_max", valueView.m_data->m_max);
    }

    template<typename ScalarT,FormatType TFormatType>
    void SerdeSpec<AABB<ScalarT>, TFormatType>::deserialize(DeserializableValueView<AABB<ScalarT>, TFormatType>& valueView) noexcept
    {
        const auto m_min = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_min)>("m_min");
        if(m_min)
        {
            valueView.m_data->m_min = *m_min;
        }

        const auto m_max = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_max)>("m_max");
        if(m_max)
        {
            valueView.m_data->m_max = *m_max;
        }
    }
}