#ifndef __SGCORE_SERDE_GENERATEDSERDESPECS_H__
#define __SGCORE_SERDE_GENERATEDSERDESPECS_H__

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>


#include "../Utils/UniqueName.h"

// SERDE FORWARD DECL FOR struct 'SGCore::UniqueNameWrapper'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                          SGCore::Serde::DerivedTypes<SGCore::EntityBaseInfo>
{
    static inline const std::string type_name = "SGCore::UniqueNameWrapper";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Audio/AudioSource.h"

// SERDE FORWARD DECL FOR struct 'SGCore::AudioSource'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::AudioSource, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                    SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::AudioSource";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Transformations/Transform.h"

// SERDE FORWARD DECL FOR struct 'SGCore::Transform'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Transform, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                  SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::Transform";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Transform, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Transform, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Utils/UniqueName.h"

// SERDE FORWARD DECL FOR struct 'SGCore::UniqueName'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                   SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::UniqueName";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Math/AABB.h"

// SERDE FORWARD DECL FOR struct 'SGCore::AABB'
// =================================================================================
template<
        typename ScalarT,
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::AABB<ScalarT>, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                      SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::AABB";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Scene/Layer.h"

// SERDE FORWARD DECL FOR struct 'SGCore::Layer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                              SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::Layer";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Transformations/TransformBase.h"

// SERDE FORWARD DECL FOR struct 'SGCore::TransformBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::TransformBase, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                      SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::TransformBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Scene/EntityBaseInfo.h"

// SERDE FORWARD DECL FOR struct 'SGCore::EntityBaseInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType> : SGCore::Serde::BaseTypes<SGCore::UniqueNameWrapper>,
                                                                       SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::EntityBaseInfo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../ImportedScenesArch/MeshDataRenderInfo.h"

// SERDE FORWARD DECL FOR struct 'SGCore::MeshDataRenderInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::MeshDataRenderInfo, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                           SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::MeshDataRenderInfo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::MeshDataRenderInfo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshDataRenderInfo, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/RenderingBase.h"

// SERDE FORWARD DECL FOR struct 'SGCore::RenderingBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::RenderingBase, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                      SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::RenderingBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::RenderingBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderingBase, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Camera3D.h"

// SERDE FORWARD DECL FOR struct 'SGCore::Camera3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Camera3D, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                 SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::Camera3D";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Camera3D, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Camera3D, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Gizmos/GizmoBase.h"

// SERDE FORWARD DECL FOR struct 'SGCore::GizmoBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::GizmoBase, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                  SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::GizmoBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::GizmoBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::GizmoBase, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Gizmos/SphereGizmo.h"

// SERDE FORWARD DECL FOR struct 'SGCore::SphereGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::SphereGizmo, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                    SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::SphereGizmo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::SphereGizmo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmo, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Gizmos/BoxGizmo.h"

// SERDE FORWARD DECL FOR struct 'SGCore::BoxGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::BoxGizmo, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                 SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::BoxGizmo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Gizmos/LineGizmo.h"

// SERDE FORWARD DECL FOR struct 'SGCore::LineGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::LineGizmo, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                  SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::LineGizmo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::LineGizmo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmo, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Atmosphere/Atmosphere.h"

// SERDE FORWARD DECL FOR struct 'SGCore::Atmosphere'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Atmosphere, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                   SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::Atmosphere";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Atmosphere, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Atmosphere, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/MeshBase.h"

// SERDE FORWARD DECL FOR struct 'SGCore::MeshBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                 SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::MeshBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Lighting/LightBase.h"

// SERDE FORWARD DECL FOR struct 'SGCore::LightBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::LightBase, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                  SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::LightBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::LightBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::LightBase, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/Lighting/DirectionalLight.h"

// SERDE FORWARD DECL FOR struct 'SGCore::DirectionalLight'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::DirectionalLight, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                         SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::DirectionalLight";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::DirectionalLight, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::DirectionalLight, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Transformations/Controllable3D.h"

// SERDE FORWARD DECL FOR struct 'SGCore::Controllable3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Controllable3D, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                       SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::Controllable3D";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Controllable3D, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Controllable3D, TFormatType>& valueView) noexcept;
};
// =================================================================================


#include "../Render/UICamera.h"

// SERDE FORWARD DECL FOR struct 'SGCore::UICamera'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UICamera, TFormatType> : SGCore::Serde::BaseTypes<>,
                                                                 SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGCore::UICamera";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::UICamera, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UICamera, TFormatType>& valueView) noexcept;
};
// =================================================================================


// implementations


// SERDE IMPL FOR struct 'SGCore::UniqueNameWrapper'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_uniqueName", valueView.m_data->m_uniqueName);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept
{

    const auto m_uniqueName = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_uniqueName)>>>("m_uniqueName");

    if(m_uniqueName)
    {
        valueView.m_data->m_uniqueName = *m_uniqueName;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::AudioSource'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AudioSource, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_rolloffFactor", valueView.m_data->getRolloffFactor);


    valueView.getValueContainer().addMember("m_pitch", valueView.m_data->getPitch);


    valueView.getValueContainer().addMember("m_gain", valueView.m_data->getGain);


    valueView.getValueContainer().addMember("m_position", valueView.m_data->getPosition);


    valueView.getValueContainer().addMember("m_velocity", valueView.m_data->getVelocity);


    valueView.getValueContainer().addMember("m_direction", valueView.m_data->getDirection);


    valueView.getValueContainer().addMember("m_isLooping", valueView.m_data->isLooping);


    valueView.getValueContainer().addMember("m_attachedAudioTrack", valueView.m_data->m_attachedAudioTrack);


    valueView.getValueContainer().addMember("m_lastState", valueView.m_data->getState);


    valueView.getValueContainer().addMember("m_type", valueView.m_data->getType);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AudioSource, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept
{

    const auto m_rolloffFactor = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getRolloffFactor)>>>("m_rolloffFactor");

    if(m_rolloffFactor)
    {
        valueView.m_data->setRolloffFactor(*m_rolloffFactor);
    }


    const auto m_pitch = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getPitch)>>>("m_pitch");

    if(m_pitch)
    {
        valueView.m_data->setPitch(*m_pitch);
    }


    const auto m_gain = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getGain)>>>("m_gain");

    if(m_gain)
    {
        valueView.m_data->setGain(*m_gain);
    }


    const auto m_position = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getPosition)>>>("m_position");

    if(m_position)
    {
        valueView.m_data->setPosition(*m_position);
    }


    const auto m_velocity = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getVelocity)>>>("m_velocity");

    if(m_velocity)
    {
        valueView.m_data->setVelocity(*m_velocity);
    }


    const auto m_direction = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getDirection)>>>("m_direction");

    if(m_direction)
    {
        valueView.m_data->setDirection(*m_direction);
    }


    const auto m_isLooping = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->isLooping)>>>("m_isLooping");

    if(m_isLooping)
    {
        valueView.m_data->setIsLooping(*m_isLooping);
    }


    const auto m_attachedAudioTrack = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_attachedAudioTrack)>>>("m_attachedAudioTrack");

    if(m_attachedAudioTrack)
    {
        valueView.m_data->m_attachedAudioTrack = *m_attachedAudioTrack;
    }


    const auto m_lastState = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getState)>>>("m_lastState");

    if(m_lastState)
    {
        valueView.m_data->setState(*m_lastState);
    }


    const auto m_type = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getType)>>>("m_type");

    if(m_type)
    {
        valueView.m_data->setType(*m_type);
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::Transform'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Transform, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::Transform, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_ownTransform", valueView.m_data->m_ownTransform);


    valueView.getValueContainer().addMember("m_finalTransform", valueView.m_data->m_finalTransform);


    valueView.getValueContainer().addMember("m_followParentTRS", valueView.m_data->m_followParentTRS);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Transform, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Transform, TFormatType>& valueView) noexcept
{

    const auto m_ownTransform = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_ownTransform)>>>("m_ownTransform");

    if(m_ownTransform)
    {
        valueView.m_data->m_ownTransform = *m_ownTransform;
    }


    const auto m_finalTransform = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_finalTransform)>>>("m_finalTransform");

    if(m_finalTransform)
    {
        valueView.m_data->m_finalTransform = *m_finalTransform;
    }


    const auto m_followParentTRS = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_followParentTRS)>>>("m_followParentTRS");

    if(m_followParentTRS)
    {
        valueView.m_data->m_followParentTRS = *m_followParentTRS;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::UniqueName'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_rawName", valueView.m_data->m_rawName);


    valueView.getValueContainer().addMember("m_uniqueID", valueView.m_data->m_uniqueID);


    valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept
{

    const auto m_rawName = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_rawName)>>>("m_rawName");

    if(m_rawName)
    {
        valueView.m_data->m_rawName = *m_rawName;
    }


    const auto m_uniqueID = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_uniqueID)>>>("m_uniqueID");

    if(m_uniqueID)
    {
        valueView.m_data->m_uniqueID = *m_uniqueID;
    }


    const auto m_name = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_name)>>>("m_name");

    if(m_name)
    {
        valueView.m_data->m_name = *m_name;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::AABB'
// =================================================================================
template<
        typename ScalarT,
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AABB<ScalarT>, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_min", valueView.m_data->m_min);


    valueView.getValueContainer().addMember("m_max", valueView.m_data->m_max);

}

template<
        typename ScalarT,
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AABB<ScalarT>, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept
{

    const auto m_min = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_min)>>>("m_min");

    if(m_min)
    {
        valueView.m_data->m_min = *m_min;
    }


    const auto m_max = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_max)>>>("m_max");

    if(m_max)
    {
        valueView.m_data->m_max = *m_max;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::Layer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_isOpaque", valueView.m_data->m_isOpaque);


    valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);


    valueView.getValueContainer().addMember("m_index", valueView.m_data->m_index);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept
{

    const auto m_isOpaque = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_isOpaque)>>>("m_isOpaque");

    if(m_isOpaque)
    {
        valueView.m_data->m_isOpaque = *m_isOpaque;
    }


    const auto m_name = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_name)>>>("m_name");

    if(m_name)
    {
        valueView.m_data->m_name = *m_name;
    }


    const auto m_index = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_index)>>>("m_index");

    if(m_index)
    {
        valueView.m_data->m_index = *m_index;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::TransformBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::TransformBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_blockTranslation", valueView.m_data->m_blockTranslation);


    valueView.getValueContainer().addMember("m_left", valueView.m_data->m_left);


    valueView.getValueContainer().addMember("m_blockScale", valueView.m_data->m_blockScale);


    valueView.getValueContainer().addMember("m_blockRotation", valueView.m_data->m_blockRotation);


    valueView.getValueContainer().addMember("m_rotation", valueView.m_data->m_rotation);


    valueView.getValueContainer().addMember("m_aabb", valueView.m_data->m_aabb);


    valueView.getValueContainer().addMember("m_position", valueView.m_data->m_position);


    valueView.getValueContainer().addMember("m_scale", valueView.m_data->m_scale);


    valueView.getValueContainer().addMember("m_forward", valueView.m_data->m_forward);


    valueView.getValueContainer().addMember("m_up", valueView.m_data->m_up);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::TransformBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept
{

    const auto m_blockTranslation = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_blockTranslation)>>>("m_blockTranslation");

    if(m_blockTranslation)
    {
        valueView.m_data->m_blockTranslation = *m_blockTranslation;
    }


    const auto m_left = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_left)>>>("m_left");

    if(m_left)
    {
        valueView.m_data->m_left = *m_left;
    }


    const auto m_blockScale = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_blockScale)>>>("m_blockScale");

    if(m_blockScale)
    {
        valueView.m_data->m_blockScale = *m_blockScale;
    }


    const auto m_blockRotation = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_blockRotation)>>>("m_blockRotation");

    if(m_blockRotation)
    {
        valueView.m_data->m_blockRotation = *m_blockRotation;
    }


    const auto m_rotation = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_rotation)>>>("m_rotation");

    if(m_rotation)
    {
        valueView.m_data->m_rotation = *m_rotation;
    }


    const auto m_aabb = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_aabb)>>>("m_aabb");

    if(m_aabb)
    {
        valueView.m_data->m_aabb = *m_aabb;
    }


    const auto m_position = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_position)>>>("m_position");

    if(m_position)
    {
        valueView.m_data->m_position = *m_position;
    }


    const auto m_scale = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_scale)>>>("m_scale");

    if(m_scale)
    {
        valueView.m_data->m_scale = *m_scale;
    }


    const auto m_forward = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_forward)>>>("m_forward");

    if(m_forward)
    {
        valueView.m_data->m_forward = *m_forward;
    }


    const auto m_up = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_up)>>>("m_up");

    if(m_up)
    {
        valueView.m_data->m_up = *m_up;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::EntityBaseInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_parent", valueView.m_data->m_parent);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept
{

    const auto m_parent = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_parent)>>>("m_parent");

    if(m_parent)
    {
        valueView.m_data->m_parent = *m_parent;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::MeshDataRenderInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshDataRenderInfo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::MeshDataRenderInfo, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_useIndices", valueView.m_data->m_useIndices);


    valueView.getValueContainer().addMember("m_linesWidth", valueView.m_data->m_linesWidth);


    valueView.getValueContainer().addMember("m_facesCullingPolygonsOrder", valueView.m_data->m_facesCullingPolygonsOrder);


    valueView.getValueContainer().addMember("m_enableFacesCulling", valueView.m_data->m_enableFacesCulling);


    valueView.getValueContainer().addMember("m_facesCullingFaceType", valueView.m_data->m_facesCullingFaceType);


    valueView.getValueContainer().addMember("m_drawMode", valueView.m_data->m_drawMode);


    valueView.getValueContainer().addMember("m_pointsSize", valueView.m_data->m_pointsSize);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshDataRenderInfo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshDataRenderInfo, TFormatType>& valueView) noexcept
{

    const auto m_useIndices = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_useIndices)>>>("m_useIndices");

    if(m_useIndices)
    {
        valueView.m_data->m_useIndices = *m_useIndices;
    }


    const auto m_linesWidth = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_linesWidth)>>>("m_linesWidth");

    if(m_linesWidth)
    {
        valueView.m_data->m_linesWidth = *m_linesWidth;
    }


    const auto m_facesCullingPolygonsOrder = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_facesCullingPolygonsOrder)>>>("m_facesCullingPolygonsOrder");

    if(m_facesCullingPolygonsOrder)
    {
        valueView.m_data->m_facesCullingPolygonsOrder = *m_facesCullingPolygonsOrder;
    }


    const auto m_enableFacesCulling = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_enableFacesCulling)>>>("m_enableFacesCulling");

    if(m_enableFacesCulling)
    {
        valueView.m_data->m_enableFacesCulling = *m_enableFacesCulling;
    }


    const auto m_facesCullingFaceType = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_facesCullingFaceType)>>>("m_facesCullingFaceType");

    if(m_facesCullingFaceType)
    {
        valueView.m_data->m_facesCullingFaceType = *m_facesCullingFaceType;
    }


    const auto m_drawMode = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_drawMode)>>>("m_drawMode");

    if(m_drawMode)
    {
        valueView.m_data->m_drawMode = *m_drawMode;
    }


    const auto m_pointsSize = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_pointsSize)>>>("m_pointsSize");

    if(m_pointsSize)
    {
        valueView.m_data->m_pointsSize = *m_pointsSize;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::RenderingBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderingBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::RenderingBase, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_top", valueView.m_data->m_top);


    valueView.getValueContainer().addMember("m_fov", valueView.m_data->m_fov);


    valueView.getValueContainer().addMember("m_aspect", valueView.m_data->m_aspect);


    valueView.getValueContainer().addMember("m_zNear", valueView.m_data->m_zNear);


    valueView.getValueContainer().addMember("m_zFar", valueView.m_data->m_zFar);


    valueView.getValueContainer().addMember("m_left", valueView.m_data->m_left);


    valueView.getValueContainer().addMember("m_right", valueView.m_data->m_right);


    valueView.getValueContainer().addMember("m_bottom", valueView.m_data->m_bottom);


    valueView.getValueContainer().addMember("m_projectionMatrix", valueView.m_data->m_projectionMatrix);


    valueView.getValueContainer().addMember("m_orthographicMatrix", valueView.m_data->m_orthographicMatrix);


    valueView.getValueContainer().addMember("m_viewMatrix", valueView.m_data->m_viewMatrix);


    valueView.getValueContainer().addMember("m_projectionSpaceMatrix", valueView.m_data->m_projectionSpaceMatrix);


    valueView.getValueContainer().addMember("m_orthographicSpaceMatrix", valueView.m_data->m_orthographicSpaceMatrix);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderingBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderingBase, TFormatType>& valueView) noexcept
{

    const auto m_top = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_top)>>>("m_top");

    if(m_top)
    {
        valueView.m_data->m_top = *m_top;
    }


    const auto m_fov = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_fov)>>>("m_fov");

    if(m_fov)
    {
        valueView.m_data->m_fov = *m_fov;
    }


    const auto m_aspect = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_aspect)>>>("m_aspect");

    if(m_aspect)
    {
        valueView.m_data->m_aspect = *m_aspect;
    }


    const auto m_zNear = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_zNear)>>>("m_zNear");

    if(m_zNear)
    {
        valueView.m_data->m_zNear = *m_zNear;
    }


    const auto m_zFar = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_zFar)>>>("m_zFar");

    if(m_zFar)
    {
        valueView.m_data->m_zFar = *m_zFar;
    }


    const auto m_left = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_left)>>>("m_left");

    if(m_left)
    {
        valueView.m_data->m_left = *m_left;
    }


    const auto m_right = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_right)>>>("m_right");

    if(m_right)
    {
        valueView.m_data->m_right = *m_right;
    }


    const auto m_bottom = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_bottom)>>>("m_bottom");

    if(m_bottom)
    {
        valueView.m_data->m_bottom = *m_bottom;
    }


    const auto m_projectionMatrix = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_projectionMatrix)>>>("m_projectionMatrix");

    if(m_projectionMatrix)
    {
        valueView.m_data->m_projectionMatrix = *m_projectionMatrix;
    }


    const auto m_orthographicMatrix = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_orthographicMatrix)>>>("m_orthographicMatrix");

    if(m_orthographicMatrix)
    {
        valueView.m_data->m_orthographicMatrix = *m_orthographicMatrix;
    }


    const auto m_viewMatrix = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_viewMatrix)>>>("m_viewMatrix");

    if(m_viewMatrix)
    {
        valueView.m_data->m_viewMatrix = *m_viewMatrix;
    }


    const auto m_projectionSpaceMatrix = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_projectionSpaceMatrix)>>>("m_projectionSpaceMatrix");

    if(m_projectionSpaceMatrix)
    {
        valueView.m_data->m_projectionSpaceMatrix = *m_projectionSpaceMatrix;
    }


    const auto m_orthographicSpaceMatrix = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_orthographicSpaceMatrix)>>>("m_orthographicSpaceMatrix");

    if(m_orthographicSpaceMatrix)
    {
        valueView.m_data->m_orthographicSpaceMatrix = *m_orthographicSpaceMatrix;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::Camera3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Camera3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::Camera3D, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Camera3D, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Camera3D, TFormatType>& valueView) noexcept
{
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::GizmoBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::GizmoBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::GizmoBase, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_color", valueView.m_data->m_color);


    valueView.getValueContainer().addMember("m_meshBase", valueView.m_data->m_meshBase);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::GizmoBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::GizmoBase, TFormatType>& valueView) noexcept
{

    const auto m_color = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_color)>>>("m_color");

    if(m_color)
    {
        valueView.m_data->m_color = *m_color;
    }


    const auto m_meshBase = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_meshBase)>>>("m_meshBase");

    if(m_meshBase)
    {
        valueView.m_data->m_meshBase = *m_meshBase;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::SphereGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::SphereGizmo, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_base", valueView.m_data->m_base);


    valueView.getValueContainer().addMember("m_radius", valueView.m_data->m_radius);


    valueView.getValueContainer().addMember("m_angleIncrement", valueView.m_data->m_angleIncrement);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmo, TFormatType>& valueView) noexcept
{

    const auto m_base = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_base)>>>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }


    const auto m_radius = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_radius)>>>("m_radius");

    if(m_radius)
    {
        valueView.m_data->m_radius = *m_radius;
    }


    const auto m_angleIncrement = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_angleIncrement)>>>("m_angleIncrement");

    if(m_angleIncrement)
    {
        valueView.m_data->m_angleIncrement = *m_angleIncrement;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::BoxGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_lastSize", valueView.m_data->m_lastSize);


    valueView.getValueContainer().addMember("m_base", valueView.m_data->m_base);


    valueView.getValueContainer().addMember("m_size", valueView.m_data->m_size);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept
{

    const auto m_lastSize = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastSize)>>>("m_lastSize");

    if(m_lastSize)
    {
        valueView.m_data->m_lastSize = *m_lastSize;
    }


    const auto m_base = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_base)>>>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }


    const auto m_size = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_size)>>>("m_size");

    if(m_size)
    {
        valueView.m_data->m_size = *m_size;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::LineGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LineGizmo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::LineGizmo, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_base", valueView.m_data->m_base);


    valueView.getValueContainer().addMember("m_start", valueView.m_data->m_start);


    valueView.getValueContainer().addMember("m_end", valueView.m_data->m_end);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LineGizmo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmo, TFormatType>& valueView) noexcept
{

    const auto m_base = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_base)>>>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }


    const auto m_start = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_start)>>>("m_start");

    if(m_start)
    {
        valueView.m_data->m_start = *m_start;
    }


    const auto m_end = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_end)>>>("m_end");

    if(m_end)
    {
        valueView.m_data->m_end = *m_end;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::Atmosphere'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Atmosphere, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::Atmosphere, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_sunPosition", valueView.m_data->m_sunPosition);


    valueView.getValueContainer().addMember("m_lastMieScaleHeight", valueView.m_data->m_lastMieScaleHeight);


    valueView.getValueContainer().addMember("m_lastRayleighScatteringCoeff", valueView.m_data->m_lastRayleighScatteringCoeff);


    valueView.getValueContainer().addMember("m_lastSunRotation", valueView.m_data->m_lastSunRotation);


    valueView.getValueContainer().addMember("m_sunAmbient", valueView.m_data->m_sunAmbient);


    valueView.getValueContainer().addMember("m_rayleighScatteringCoeff", valueView.m_data->m_rayleighScatteringCoeff);


    valueView.getValueContainer().addMember("m_atmosphereRadius", valueView.m_data->m_atmosphereRadius);


    valueView.getValueContainer().addMember("m_sunRotation", valueView.m_data->m_sunRotation);


    valueView.getValueContainer().addMember("m_lastSunAmbient", valueView.m_data->m_lastSunAmbient);


    valueView.getValueContainer().addMember("m_mieScatteringCoeff", valueView.m_data->m_mieScatteringCoeff);


    valueView.getValueContainer().addMember("m_lastMieScatteringCoeff", valueView.m_data->m_lastMieScatteringCoeff);


    valueView.getValueContainer().addMember("m_rayleighScaleHeight", valueView.m_data->m_rayleighScaleHeight);


    valueView.getValueContainer().addMember("m_lastRayOrigin", valueView.m_data->m_lastRayOrigin);


    valueView.getValueContainer().addMember("m_lastRayleighScaleHeight", valueView.m_data->m_lastRayleighScaleHeight);


    valueView.getValueContainer().addMember("m_mieScaleHeight", valueView.m_data->m_mieScaleHeight);


    valueView.getValueContainer().addMember("m_sunIntensity", valueView.m_data->m_sunIntensity);


    valueView.getValueContainer().addMember("m_lastSunIntensity", valueView.m_data->m_lastSunIntensity);


    valueView.getValueContainer().addMember("m_planetRadius", valueView.m_data->m_planetRadius);


    valueView.getValueContainer().addMember("m_lastPlanetRadius", valueView.m_data->m_lastPlanetRadius);


    valueView.getValueContainer().addMember("m_lastAtmosphereRadius", valueView.m_data->m_lastAtmosphereRadius);


    valueView.getValueContainer().addMember("m_miePreferredScatteringDirection", valueView.m_data->m_miePreferredScatteringDirection);


    valueView.getValueContainer().addMember("m_lastMiePreferredScatteringDirection", valueView.m_data->m_lastMiePreferredScatteringDirection);


    valueView.getValueContainer().addMember("m_rayOrigin", valueView.m_data->m_rayOrigin);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Atmosphere, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Atmosphere, TFormatType>& valueView) noexcept
{

    const auto m_sunPosition = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_sunPosition)>>>("m_sunPosition");

    if(m_sunPosition)
    {
        valueView.m_data->m_sunPosition = *m_sunPosition;
    }


    const auto m_lastMieScaleHeight = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastMieScaleHeight)>>>("m_lastMieScaleHeight");

    if(m_lastMieScaleHeight)
    {
        valueView.m_data->m_lastMieScaleHeight = *m_lastMieScaleHeight;
    }


    const auto m_lastRayleighScatteringCoeff = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastRayleighScatteringCoeff)>>>("m_lastRayleighScatteringCoeff");

    if(m_lastRayleighScatteringCoeff)
    {
        valueView.m_data->m_lastRayleighScatteringCoeff = *m_lastRayleighScatteringCoeff;
    }


    const auto m_lastSunRotation = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastSunRotation)>>>("m_lastSunRotation");

    if(m_lastSunRotation)
    {
        valueView.m_data->m_lastSunRotation = *m_lastSunRotation;
    }


    const auto m_sunAmbient = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_sunAmbient)>>>("m_sunAmbient");

    if(m_sunAmbient)
    {
        valueView.m_data->m_sunAmbient = *m_sunAmbient;
    }


    const auto m_rayleighScatteringCoeff = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_rayleighScatteringCoeff)>>>("m_rayleighScatteringCoeff");

    if(m_rayleighScatteringCoeff)
    {
        valueView.m_data->m_rayleighScatteringCoeff = *m_rayleighScatteringCoeff;
    }


    const auto m_atmosphereRadius = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_atmosphereRadius)>>>("m_atmosphereRadius");

    if(m_atmosphereRadius)
    {
        valueView.m_data->m_atmosphereRadius = *m_atmosphereRadius;
    }


    const auto m_sunRotation = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_sunRotation)>>>("m_sunRotation");

    if(m_sunRotation)
    {
        valueView.m_data->m_sunRotation = *m_sunRotation;
    }


    const auto m_lastSunAmbient = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastSunAmbient)>>>("m_lastSunAmbient");

    if(m_lastSunAmbient)
    {
        valueView.m_data->m_lastSunAmbient = *m_lastSunAmbient;
    }


    const auto m_mieScatteringCoeff = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_mieScatteringCoeff)>>>("m_mieScatteringCoeff");

    if(m_mieScatteringCoeff)
    {
        valueView.m_data->m_mieScatteringCoeff = *m_mieScatteringCoeff;
    }


    const auto m_lastMieScatteringCoeff = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastMieScatteringCoeff)>>>("m_lastMieScatteringCoeff");

    if(m_lastMieScatteringCoeff)
    {
        valueView.m_data->m_lastMieScatteringCoeff = *m_lastMieScatteringCoeff;
    }


    const auto m_rayleighScaleHeight = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_rayleighScaleHeight)>>>("m_rayleighScaleHeight");

    if(m_rayleighScaleHeight)
    {
        valueView.m_data->m_rayleighScaleHeight = *m_rayleighScaleHeight;
    }


    const auto m_lastRayOrigin = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastRayOrigin)>>>("m_lastRayOrigin");

    if(m_lastRayOrigin)
    {
        valueView.m_data->m_lastRayOrigin = *m_lastRayOrigin;
    }


    const auto m_lastRayleighScaleHeight = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastRayleighScaleHeight)>>>("m_lastRayleighScaleHeight");

    if(m_lastRayleighScaleHeight)
    {
        valueView.m_data->m_lastRayleighScaleHeight = *m_lastRayleighScaleHeight;
    }


    const auto m_mieScaleHeight = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_mieScaleHeight)>>>("m_mieScaleHeight");

    if(m_mieScaleHeight)
    {
        valueView.m_data->m_mieScaleHeight = *m_mieScaleHeight;
    }


    const auto m_sunIntensity = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_sunIntensity)>>>("m_sunIntensity");

    if(m_sunIntensity)
    {
        valueView.m_data->m_sunIntensity = *m_sunIntensity;
    }


    const auto m_lastSunIntensity = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastSunIntensity)>>>("m_lastSunIntensity");

    if(m_lastSunIntensity)
    {
        valueView.m_data->m_lastSunIntensity = *m_lastSunIntensity;
    }


    const auto m_planetRadius = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_planetRadius)>>>("m_planetRadius");

    if(m_planetRadius)
    {
        valueView.m_data->m_planetRadius = *m_planetRadius;
    }


    const auto m_lastPlanetRadius = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastPlanetRadius)>>>("m_lastPlanetRadius");

    if(m_lastPlanetRadius)
    {
        valueView.m_data->m_lastPlanetRadius = *m_lastPlanetRadius;
    }


    const auto m_lastAtmosphereRadius = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastAtmosphereRadius)>>>("m_lastAtmosphereRadius");

    if(m_lastAtmosphereRadius)
    {
        valueView.m_data->m_lastAtmosphereRadius = *m_lastAtmosphereRadius;
    }


    const auto m_miePreferredScatteringDirection = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_miePreferredScatteringDirection)>>>("m_miePreferredScatteringDirection");

    if(m_miePreferredScatteringDirection)
    {
        valueView.m_data->m_miePreferredScatteringDirection = *m_miePreferredScatteringDirection;
    }


    const auto m_lastMiePreferredScatteringDirection = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_lastMiePreferredScatteringDirection)>>>("m_lastMiePreferredScatteringDirection");

    if(m_lastMiePreferredScatteringDirection)
    {
        valueView.m_data->m_lastMiePreferredScatteringDirection = *m_lastMiePreferredScatteringDirection;
    }


    const auto m_rayOrigin = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_rayOrigin)>>>("m_rayOrigin");

    if(m_rayOrigin)
    {
        valueView.m_data->m_rayOrigin = *m_rayOrigin;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::MeshBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_material", valueView.m_data->getMaterial);


    valueView.getValueContainer().addMember("m_meshDataRenderInfo", valueView.m_data->m_meshDataRenderInfo);


    valueView.getValueContainer().addMember("m_meshData", valueView.m_data->getMeshData);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept
{

    const auto m_material = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMaterial)>>>("m_material");

    if(m_material)
    {
        valueView.m_data->setMaterial(*m_material);
    }


    const auto m_meshDataRenderInfo = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_meshDataRenderInfo)>>>("m_meshDataRenderInfo");

    if(m_meshDataRenderInfo)
    {
        valueView.m_data->m_meshDataRenderInfo = *m_meshDataRenderInfo;
    }


    const auto m_meshData = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMeshData)>>>("m_meshData");

    if(m_meshData)
    {
        valueView.m_data->setMeshData(*m_meshData);
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::LightBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LightBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::LightBase, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_color", valueView.m_data->m_color);


    valueView.getValueContainer().addMember("m_intensity", valueView.m_data->m_intensity);


    valueView.getValueContainer().addMember("m_samplesCount", valueView.m_data->m_samplesCount);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LightBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::LightBase, TFormatType>& valueView) noexcept
{

    const auto m_color = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_color)>>>("m_color");

    if(m_color)
    {
        valueView.m_data->m_color = *m_color;
    }


    const auto m_intensity = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_intensity)>>>("m_intensity");

    if(m_intensity)
    {
        valueView.m_data->m_intensity = *m_intensity;
    }


    const auto m_samplesCount = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_samplesCount)>>>("m_samplesCount");

    if(m_samplesCount)
    {
        valueView.m_data->m_samplesCount = *m_samplesCount;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::DirectionalLight'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::DirectionalLight, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::DirectionalLight, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_base", valueView.m_data->m_base);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::DirectionalLight, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::DirectionalLight, TFormatType>& valueView) noexcept
{

    const auto m_base = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_base)>>>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::Controllable3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Controllable3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::Controllable3D, TFormatType>& valueView) noexcept
{

    valueView.getValueContainer().addMember("m_movementSpeed", valueView.m_data->m_movementSpeed);


    valueView.getValueContainer().addMember("m_rotationSensitive", valueView.m_data->m_rotationSensitive);

}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Controllable3D, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Controllable3D, TFormatType>& valueView) noexcept
{

    const auto m_movementSpeed = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_movementSpeed)>>>("m_movementSpeed");

    if(m_movementSpeed)
    {
        valueView.m_data->m_movementSpeed = *m_movementSpeed;
    }


    const auto m_rotationSensitive = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->m_rotationSensitive)>>>("m_rotationSensitive");

    if(m_rotationSensitive)
    {
        valueView.m_data->m_rotationSensitive = *m_rotationSensitive;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::UICamera'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UICamera, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::UICamera, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UICamera, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::UICamera, TFormatType>& valueView) noexcept
{
}
// =================================================================================

#endif
