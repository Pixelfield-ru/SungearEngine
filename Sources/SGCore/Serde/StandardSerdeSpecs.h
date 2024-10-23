//
// Created by Ilya on 18.08.2024.
//

#ifndef SUNGEARENGINE_STANDARDSERDESPECS_H
#define SUNGEARENGINE_STANDARDSERDESPECS_H

#include "Serde.h"
#include <glm/glm.hpp>
#include "SGCore/Logger/Logger.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"

#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Math/AABB.h"
#include "SGCore/Scene/Layer.h"
#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/Gizmos/GizmoBase.h"
#include "SGCore/Render/Gizmos/SphereGizmo.h"
#include "SGCore/Render/Gizmos/BoxGizmo.h"
#include "SGCore/Render/Gizmos/LineGizmo.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Render/MeshBase.h"
#include "SGCore/Render/Atmosphere/AtmosphereUpdater.h"
#include "SGCore/Render/Lighting/LightBase.h"
#include "SGCore/Transformations/Controllables3DUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/UICamera.h"
#include "SGCore/Scene/ISystem.h"
#include "SGCore/Scene/IParallelSystem.h"
#include "SGCore/Render/RenderingBasesUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLightsUpdater.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Render/Gizmos/BoxGizmosRenderer.h"
#include "SGCore/Render/Gizmos/LineGizmosRenderer.h"
#include "SGCore/Render/Batching/BatchesRenderer.h"
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/SpacePartitioning/OctreesSolver.h"
#include "SGCore/Audio/AudioProcessor.h"
#include "SGCore/Graphics/API/ITexture2D.h"

#include "SGCore/Serde/Components/NonSavable.h"

// =========================================================================================
// STANDARD SerdeSpec IMPLEMENTATIONS
// =========================================================================================

#pragma region Generated
// SERDE FORWARD DECL FOR struct 'SGCore::UniqueNameWrapper'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                SGCore::EntityBaseInfo
                                   >
{
    static inline const std::string type_name = "SGCore::UniqueNameWrapper";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::AudioSource'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::AudioSource, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::AudioSource";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::Transform'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Transform, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Transform";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Transform, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Transform, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::UniqueName'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::UniqueName";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::AABB'
// =================================================================================
template<
        typename ScalarT,
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::AABB<ScalarT>, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::AABB";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::Layer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Layer";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::TransformBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::TransformBase, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::TransformBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::EntityBaseInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::UniqueNameWrapper
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::EntityBaseInfo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::MeshDataRenderInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::MeshDataRenderInfo, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::MeshDataRenderInfo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::MeshDataRenderInfo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshDataRenderInfo, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::RenderingBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::RenderingBase, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::RenderingBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::RenderingBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderingBase, TFormatType>& valueView) noexcept;
};
// =================================================================================






// SERDE FORWARD DECL FOR struct 'SGCore::Camera3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Camera3D, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Camera3D";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Camera3D, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Camera3D, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::GizmoBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::GizmoBase, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::GizmoBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::GizmoBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::GizmoBase, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::SphereGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::SphereGizmo, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::SphereGizmo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::SphereGizmo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmo, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::BoxGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::BoxGizmo, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::BoxGizmo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::LineGizmo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::LineGizmo, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::LineGizmo";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::LineGizmo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmo, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::Atmosphere'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Atmosphere, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Atmosphere";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Atmosphere, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Atmosphere, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::MeshBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::MeshBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::AtmosphereUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::AtmosphereUpdater, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::AtmosphereUpdater";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::AtmosphereUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AtmosphereUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::LightBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::LightBase, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::LightBase";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::LightBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::LightBase, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::Controllables3DUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Controllables3DUpdater, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Controllables3DUpdater";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::DirectionalLight'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::DirectionalLight, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::DirectionalLight";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::DirectionalLight, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::DirectionalLight, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::PhysicsWorld3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::PhysicsWorld3D, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::IParallelSystem<SGCore::PhysicsWorld3D>, SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::PhysicsWorld3D";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::PhysicsWorld3D, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::PhysicsWorld3D, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::Controllable3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Controllable3D, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Controllable3D";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::Controllable3D, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Controllable3D, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::UICamera'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UICamera, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::UICamera";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::UICamera, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UICamera, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::ISystem'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::ISystem, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                SGCore::Controllables3DUpdater, SGCore::PhysicsWorld3D, SGCore::RenderingBasesUpdater, SGCore::AtmosphereUpdater, SGCore::DirectionalLightsUpdater, SGCore::TransformationsUpdater, SGCore::BoxGizmosRenderer, SGCore::LineGizmosRenderer, SGCore::BatchesRenderer, SGCore::SphereGizmosUpdater, SGCore::DebugDraw, SGCore::OctreesSolver, SGCore::AudioProcessor
                                   >
{
    static inline const std::string type_name = "SGCore::ISystem";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::ISystem, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::ISystem, TFormatType>& valueView) noexcept;
};
// =================================================================================






// SERDE FORWARD DECL FOR struct 'SGCore::IParallelSystem'
// =================================================================================
template<
        typename ParallelSystemT,
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::IParallelSystem<ParallelSystemT>, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::IParallelSystem";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::RenderingBasesUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::RenderingBasesUpdater, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::RenderingBasesUpdater";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::RenderingBasesUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderingBasesUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::DirectionalLightsUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::DirectionalLightsUpdater, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::DirectionalLightsUpdater";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::DirectionalLightsUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::DirectionalLightsUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::TransformationsUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::TransformationsUpdater, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::IParallelSystem<SGCore::TransformationsUpdater>, SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::TransformationsUpdater";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::TransformationsUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransformationsUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::BoxGizmosRenderer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::BoxGizmosRenderer, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::BoxGizmosRenderer";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::BoxGizmosRenderer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::BoxGizmosRenderer, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::LineGizmosRenderer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::LineGizmosRenderer, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::LineGizmosRenderer";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::BatchesRenderer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::BatchesRenderer, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::BatchesRenderer";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::BatchesRenderer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::BatchesRenderer, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::SphereGizmosUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::SphereGizmosUpdater, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::SphereGizmosUpdater";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::DebugDraw'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::DebugDraw, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::DebugDraw";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::DebugDraw, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::DebugDraw, TFormatType>& valueView) noexcept;
};
// =================================================================================




// SERDE FORWARD DECL FOR struct 'SGCore::OctreesSolver'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::OctreesSolver, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::IParallelSystem<SGCore::OctreesSolver>, SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::OctreesSolver";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::OctreesSolver, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::OctreesSolver, TFormatType>& valueView) noexcept;
};
// =================================================================================






// SERDE FORWARD DECL FOR struct 'SGCore::AudioProcessor'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::AudioProcessor, TFormatType> :
        SGCore::Serde::BaseTypes<
                SGCore::ISystem
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::AudioProcessor";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<SGCore::AudioProcessor, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioProcessor, TFormatType>& valueView) noexcept;
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

    const auto m_uniqueName = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_uniqueName)>("m_uniqueName");

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


    valueView.getValueContainer().addMember("m_gain", valueView.m_data->getGain());




    valueView.getValueContainer().addMember("m_rolloffFactor", valueView.m_data->getRolloffFactor());




    valueView.getValueContainer().addMember("m_pitch", valueView.m_data->getPitch());




    valueView.getValueContainer().addMember("m_position", valueView.m_data->getPosition());




    valueView.getValueContainer().addMember("m_velocity", valueView.m_data->getVelocity());




    valueView.getValueContainer().addMember("m_direction", valueView.m_data->getDirection());




    valueView.getValueContainer().addMember("m_isLooping", valueView.m_data->isLooping());




    valueView.getValueContainer().addMember("m_lastState", valueView.m_data->getState());




    valueView.getValueContainer().addMember("m_type", valueView.m_data->getType());


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AudioSource, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept
{

    const auto m_gain = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getGain())>>>("m_gain");

    if(m_gain)
    {
        valueView.m_data->setGain(*m_gain);
    }


    const auto m_rolloffFactor = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getRolloffFactor())>>>("m_rolloffFactor");

    if(m_rolloffFactor)
    {
        valueView.m_data->setRolloffFactor(*m_rolloffFactor);
    }


    const auto m_pitch = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getPitch())>>>("m_pitch");

    if(m_pitch)
    {
        valueView.m_data->setPitch(*m_pitch);
    }


    const auto m_position = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getPosition())>>>("m_position");

    if(m_position)
    {
        valueView.m_data->setPosition(*m_position);
    }


    const auto m_velocity = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getVelocity())>>>("m_velocity");

    if(m_velocity)
    {
        valueView.m_data->setVelocity(*m_velocity);
    }


    const auto m_direction = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getDirection())>>>("m_direction");

    if(m_direction)
    {
        valueView.m_data->setDirection(*m_direction);
    }


    const auto m_isLooping = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->isLooping())>>>("m_isLooping");

    if(m_isLooping)
    {
        valueView.m_data->setIsLooping(*m_isLooping);
    }


    const auto m_lastState = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getState())>>>("m_lastState");

    if(m_lastState)
    {
        valueView.m_data->setState(*m_lastState);
    }


    const auto m_type = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getType())>>>("m_type");

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
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::UniqueName'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);




    valueView.getValueContainer().addMember("m_rawName", valueView.m_data->m_rawName);




    valueView.getValueContainer().addMember("m_uniqueID", valueView.m_data->m_uniqueID);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept
{

    const auto m_name = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_name)>("m_name");

    if(m_name)
    {
        valueView.m_data->m_name = *m_name;
    }


    const auto m_rawName = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rawName)>("m_rawName");

    if(m_rawName)
    {
        valueView.m_data->m_rawName = *m_rawName;
    }


    const auto m_uniqueID = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_uniqueID)>("m_uniqueID");

    if(m_uniqueID)
    {
        valueView.m_data->m_uniqueID = *m_uniqueID;
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

    const auto m_isOpaque = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_isOpaque)>("m_isOpaque");

    if(m_isOpaque)
    {
        valueView.m_data->m_isOpaque = *m_isOpaque;
    }


    const auto m_name = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_name)>("m_name");

    if(m_name)
    {
        valueView.m_data->m_name = *m_name;
    }


    const auto m_index = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_index)>("m_index");

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

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::TransformBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransformBase, TFormatType>& valueView) noexcept
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
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::EntityBaseInfo'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept
{


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept
{
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

    const auto m_useIndices = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useIndices)>("m_useIndices");

    if(m_useIndices)
    {
        valueView.m_data->m_useIndices = *m_useIndices;
    }


    const auto m_linesWidth = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_linesWidth)>("m_linesWidth");

    if(m_linesWidth)
    {
        valueView.m_data->m_linesWidth = *m_linesWidth;
    }


    const auto m_facesCullingPolygonsOrder = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_facesCullingPolygonsOrder)>("m_facesCullingPolygonsOrder");

    if(m_facesCullingPolygonsOrder)
    {
        valueView.m_data->m_facesCullingPolygonsOrder = *m_facesCullingPolygonsOrder;
    }


    const auto m_enableFacesCulling = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_enableFacesCulling)>("m_enableFacesCulling");

    if(m_enableFacesCulling)
    {
        valueView.m_data->m_enableFacesCulling = *m_enableFacesCulling;
    }


    const auto m_facesCullingFaceType = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_facesCullingFaceType)>("m_facesCullingFaceType");

    if(m_facesCullingFaceType)
    {
        valueView.m_data->m_facesCullingFaceType = *m_facesCullingFaceType;
    }


    const auto m_drawMode = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_drawMode)>("m_drawMode");

    if(m_drawMode)
    {
        valueView.m_data->m_drawMode = *m_drawMode;
    }


    const auto m_pointsSize = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_pointsSize)>("m_pointsSize");

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


    valueView.getValueContainer().addMember("m_zFar", valueView.m_data->m_zFar);




    valueView.getValueContainer().addMember("m_fov", valueView.m_data->m_fov);




    valueView.getValueContainer().addMember("m_aspect", valueView.m_data->m_aspect);




    valueView.getValueContainer().addMember("m_zNear", valueView.m_data->m_zNear);




    valueView.getValueContainer().addMember("m_left", valueView.m_data->m_left);




    valueView.getValueContainer().addMember("m_right", valueView.m_data->m_right);




    valueView.getValueContainer().addMember("m_bottom", valueView.m_data->m_bottom);




    valueView.getValueContainer().addMember("m_top", valueView.m_data->m_top);




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

    const auto m_zFar = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_zFar)>("m_zFar");

    if(m_zFar)
    {
        valueView.m_data->m_zFar = *m_zFar;
    }


    const auto m_fov = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_fov)>("m_fov");

    if(m_fov)
    {
        valueView.m_data->m_fov = *m_fov;
    }


    const auto m_aspect = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_aspect)>("m_aspect");

    if(m_aspect)
    {
        valueView.m_data->m_aspect = *m_aspect;
    }


    const auto m_zNear = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_zNear)>("m_zNear");

    if(m_zNear)
    {
        valueView.m_data->m_zNear = *m_zNear;
    }


    const auto m_left = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_left)>("m_left");

    if(m_left)
    {
        valueView.m_data->m_left = *m_left;
    }


    const auto m_right = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_right)>("m_right");

    if(m_right)
    {
        valueView.m_data->m_right = *m_right;
    }


    const auto m_bottom = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_bottom)>("m_bottom");

    if(m_bottom)
    {
        valueView.m_data->m_bottom = *m_bottom;
    }


    const auto m_top = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_top)>("m_top");

    if(m_top)
    {
        valueView.m_data->m_top = *m_top;
    }


    const auto m_projectionMatrix = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_projectionMatrix)>("m_projectionMatrix");

    if(m_projectionMatrix)
    {
        valueView.m_data->m_projectionMatrix = *m_projectionMatrix;
    }


    const auto m_orthographicMatrix = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_orthographicMatrix)>("m_orthographicMatrix");

    if(m_orthographicMatrix)
    {
        valueView.m_data->m_orthographicMatrix = *m_orthographicMatrix;
    }


    const auto m_viewMatrix = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_viewMatrix)>("m_viewMatrix");

    if(m_viewMatrix)
    {
        valueView.m_data->m_viewMatrix = *m_viewMatrix;
    }


    const auto m_projectionSpaceMatrix = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_projectionSpaceMatrix)>("m_projectionSpaceMatrix");

    if(m_projectionSpaceMatrix)
    {
        valueView.m_data->m_projectionSpaceMatrix = *m_projectionSpaceMatrix;
    }


    const auto m_orthographicSpaceMatrix = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_orthographicSpaceMatrix)>("m_orthographicSpaceMatrix");

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

    const auto m_color = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_color)>("m_color");

    if(m_color)
    {
        valueView.m_data->m_color = *m_color;
    }


    const auto m_meshBase = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_meshBase)>("m_meshBase");

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

    const auto m_base = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_base)>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }


    const auto m_radius = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_radius)>("m_radius");

    if(m_radius)
    {
        valueView.m_data->m_radius = *m_radius;
    }


    const auto m_angleIncrement = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_angleIncrement)>("m_angleIncrement");

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

    const auto m_lastSize = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastSize)>("m_lastSize");

    if(m_lastSize)
    {
        valueView.m_data->m_lastSize = *m_lastSize;
    }


    const auto m_base = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_base)>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }


    const auto m_size = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_size)>("m_size");

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

    const auto m_base = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_base)>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }


    const auto m_start = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_start)>("m_start");

    if(m_start)
    {
        valueView.m_data->m_start = *m_start;
    }


    const auto m_end = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_end)>("m_end");

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

    const auto m_sunPosition = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_sunPosition)>("m_sunPosition");

    if(m_sunPosition)
    {
        valueView.m_data->m_sunPosition = *m_sunPosition;
    }


    const auto m_lastMieScaleHeight = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastMieScaleHeight)>("m_lastMieScaleHeight");

    if(m_lastMieScaleHeight)
    {
        valueView.m_data->m_lastMieScaleHeight = *m_lastMieScaleHeight;
    }


    const auto m_lastRayleighScatteringCoeff = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastRayleighScatteringCoeff)>("m_lastRayleighScatteringCoeff");

    if(m_lastRayleighScatteringCoeff)
    {
        valueView.m_data->m_lastRayleighScatteringCoeff = *m_lastRayleighScatteringCoeff;
    }


    const auto m_lastSunRotation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastSunRotation)>("m_lastSunRotation");

    if(m_lastSunRotation)
    {
        valueView.m_data->m_lastSunRotation = *m_lastSunRotation;
    }


    const auto m_sunAmbient = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_sunAmbient)>("m_sunAmbient");

    if(m_sunAmbient)
    {
        valueView.m_data->m_sunAmbient = *m_sunAmbient;
    }


    const auto m_rayleighScatteringCoeff = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rayleighScatteringCoeff)>("m_rayleighScatteringCoeff");

    if(m_rayleighScatteringCoeff)
    {
        valueView.m_data->m_rayleighScatteringCoeff = *m_rayleighScatteringCoeff;
    }


    const auto m_atmosphereRadius = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_atmosphereRadius)>("m_atmosphereRadius");

    if(m_atmosphereRadius)
    {
        valueView.m_data->m_atmosphereRadius = *m_atmosphereRadius;
    }


    const auto m_sunRotation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_sunRotation)>("m_sunRotation");

    if(m_sunRotation)
    {
        valueView.m_data->m_sunRotation = *m_sunRotation;
    }


    const auto m_lastSunAmbient = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastSunAmbient)>("m_lastSunAmbient");

    if(m_lastSunAmbient)
    {
        valueView.m_data->m_lastSunAmbient = *m_lastSunAmbient;
    }


    const auto m_mieScatteringCoeff = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_mieScatteringCoeff)>("m_mieScatteringCoeff");

    if(m_mieScatteringCoeff)
    {
        valueView.m_data->m_mieScatteringCoeff = *m_mieScatteringCoeff;
    }


    const auto m_lastMieScatteringCoeff = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastMieScatteringCoeff)>("m_lastMieScatteringCoeff");

    if(m_lastMieScatteringCoeff)
    {
        valueView.m_data->m_lastMieScatteringCoeff = *m_lastMieScatteringCoeff;
    }


    const auto m_rayleighScaleHeight = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rayleighScaleHeight)>("m_rayleighScaleHeight");

    if(m_rayleighScaleHeight)
    {
        valueView.m_data->m_rayleighScaleHeight = *m_rayleighScaleHeight;
    }


    const auto m_lastRayOrigin = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastRayOrigin)>("m_lastRayOrigin");

    if(m_lastRayOrigin)
    {
        valueView.m_data->m_lastRayOrigin = *m_lastRayOrigin;
    }


    const auto m_lastRayleighScaleHeight = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastRayleighScaleHeight)>("m_lastRayleighScaleHeight");

    if(m_lastRayleighScaleHeight)
    {
        valueView.m_data->m_lastRayleighScaleHeight = *m_lastRayleighScaleHeight;
    }


    const auto m_mieScaleHeight = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_mieScaleHeight)>("m_mieScaleHeight");

    if(m_mieScaleHeight)
    {
        valueView.m_data->m_mieScaleHeight = *m_mieScaleHeight;
    }


    const auto m_sunIntensity = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_sunIntensity)>("m_sunIntensity");

    if(m_sunIntensity)
    {
        valueView.m_data->m_sunIntensity = *m_sunIntensity;
    }


    const auto m_lastSunIntensity = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastSunIntensity)>("m_lastSunIntensity");

    if(m_lastSunIntensity)
    {
        valueView.m_data->m_lastSunIntensity = *m_lastSunIntensity;
    }


    const auto m_planetRadius = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_planetRadius)>("m_planetRadius");

    if(m_planetRadius)
    {
        valueView.m_data->m_planetRadius = *m_planetRadius;
    }


    const auto m_lastPlanetRadius = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastPlanetRadius)>("m_lastPlanetRadius");

    if(m_lastPlanetRadius)
    {
        valueView.m_data->m_lastPlanetRadius = *m_lastPlanetRadius;
    }


    const auto m_lastAtmosphereRadius = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastAtmosphereRadius)>("m_lastAtmosphereRadius");

    if(m_lastAtmosphereRadius)
    {
        valueView.m_data->m_lastAtmosphereRadius = *m_lastAtmosphereRadius;
    }


    const auto m_miePreferredScatteringDirection = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_miePreferredScatteringDirection)>("m_miePreferredScatteringDirection");

    if(m_miePreferredScatteringDirection)
    {
        valueView.m_data->m_miePreferredScatteringDirection = *m_miePreferredScatteringDirection;
    }


    const auto m_lastMiePreferredScatteringDirection = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_lastMiePreferredScatteringDirection)>("m_lastMiePreferredScatteringDirection");

    if(m_lastMiePreferredScatteringDirection)
    {
        valueView.m_data->m_lastMiePreferredScatteringDirection = *m_lastMiePreferredScatteringDirection;
    }


    const auto m_rayOrigin = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rayOrigin)>("m_rayOrigin");

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

    // TODO:
    // valueView.getValueContainer().addMember("m_material", valueView.m_data->getMaterial());




    valueView.getValueContainer().addMember("m_meshDataRenderInfo", valueView.m_data->m_meshDataRenderInfo);




    // TODO:
    // valueView.getValueContainer().addMember("m_meshData", valueView.m_data->getMeshData());


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept
{

    // TODO:
    /*const auto m_material = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMaterial())>>>("m_material");

    if(m_material)
    {
        valueView.m_data->setMaterial(*m_material);
    }*/


    const auto m_meshDataRenderInfo = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_meshDataRenderInfo)>("m_meshDataRenderInfo");

    if(m_meshDataRenderInfo)
    {
        valueView.m_data->m_meshDataRenderInfo = *m_meshDataRenderInfo;
    }


    // TODO:
    /*const auto m_meshData = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMeshData())>>>("m_meshData");

    if(m_meshData)
    {
        valueView.m_data->setMeshData(*m_meshData);
    }*/

}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::AtmosphereUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AtmosphereUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::AtmosphereUpdater, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AtmosphereUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::AtmosphereUpdater, TFormatType>& valueView) noexcept
{
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

    const auto m_color = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_color)>("m_color");

    if(m_color)
    {
        valueView.m_data->m_color = *m_color;
    }


    const auto m_intensity = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_intensity)>("m_intensity");

    if(m_intensity)
    {
        valueView.m_data->m_intensity = *m_intensity;
    }


    const auto m_samplesCount = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_samplesCount)>("m_samplesCount");

    if(m_samplesCount)
    {
        valueView.m_data->m_samplesCount = *m_samplesCount;
    }

}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::Controllables3DUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Controllables3DUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Controllables3DUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept
{
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

    const auto m_base = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_base)>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }

}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::PhysicsWorld3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::PhysicsWorld3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::PhysicsWorld3D, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::PhysicsWorld3D, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::PhysicsWorld3D, TFormatType>& valueView) noexcept
{
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

    const auto m_movementSpeed = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_movementSpeed)>("m_movementSpeed");

    if(m_movementSpeed)
    {
        valueView.m_data->m_movementSpeed = *m_movementSpeed;
    }


    const auto m_rotationSensitive = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rotationSensitive)>("m_rotationSensitive");

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




// SERDE IMPL FOR struct 'SGCore::ISystem'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::ISystem, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::ISystem, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::ISystem, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::ISystem, TFormatType>& valueView) noexcept
{
}
// =================================================================================






// SERDE IMPL FOR struct 'SGCore::IParallelSystem'
// =================================================================================
template<
        typename ParallelSystemT,
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept
{
}

template<
        typename ParallelSystemT,
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::RenderingBasesUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderingBasesUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::RenderingBasesUpdater, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderingBasesUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderingBasesUpdater, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::DirectionalLightsUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::DirectionalLightsUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::DirectionalLightsUpdater, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_maxLightsCount", valueView.m_data->m_maxLightsCount);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::DirectionalLightsUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::DirectionalLightsUpdater, TFormatType>& valueView) noexcept
{

    const auto m_maxLightsCount = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_maxLightsCount)>("m_maxLightsCount");

    if(m_maxLightsCount)
    {
        valueView.m_data->m_maxLightsCount = *m_maxLightsCount;
    }

}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::TransformationsUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::TransformationsUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::TransformationsUpdater, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::TransformationsUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransformationsUpdater, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::BoxGizmosRenderer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmosRenderer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::BoxGizmosRenderer, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmosRenderer, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::BoxGizmosRenderer, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::LineGizmosRenderer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LineGizmosRenderer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LineGizmosRenderer, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::BatchesRenderer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BatchesRenderer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::BatchesRenderer, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BatchesRenderer, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::BatchesRenderer, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::SphereGizmosUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmosUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmosUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept
{
}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::DebugDraw'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::DebugDraw, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::DebugDraw, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_mode", valueView.m_data->m_mode);




    valueView.getValueContainer().addMember("m_linesRenderInfo", valueView.m_data->m_linesRenderInfo);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::DebugDraw, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::DebugDraw, TFormatType>& valueView) noexcept
{

    const auto m_mode = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_mode)>("m_mode");

    if(m_mode)
    {
        valueView.m_data->m_mode = *m_mode;
    }


    const auto m_linesRenderInfo = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_linesRenderInfo)>("m_linesRenderInfo");

    if(m_linesRenderInfo)
    {
        valueView.m_data->m_linesRenderInfo = *m_linesRenderInfo;
    }

}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::OctreesSolver'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::OctreesSolver, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::OctreesSolver, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::OctreesSolver, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::OctreesSolver, TFormatType>& valueView) noexcept
{
}
// =================================================================================






// SERDE IMPL FOR struct 'SGCore::AudioProcessor'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AudioProcessor, TFormatType>::serialize(SGCore::Serde::SerializableValueView<SGCore::AudioProcessor, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AudioProcessor, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioProcessor, TFormatType>& valueView) noexcept
{
}
// =================================================================================

#pragma endregion Generated

namespace SGCore::Serde
{
    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::unique_ptr<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<std::unique_ptr<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            if(!*valueView.m_data)
            {
                valueView.getValueContainer().setAsNull();
                return;
            }

            SerializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = valueView.m_data->get();

            // serializing values of T with passing shared data
            Serde::Serializer::invokeSerdeSpecSerialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::unique_ptr<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            // if value is null then doing nothing
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            if constexpr(!std::is_abstract_v<T>)
            {
                // allocating object
                *valueView.m_data = std::make_unique<T>();
            }

            // creating temporary value view that contains object with type T
            DeserializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = valueView.m_data->get();

            // deserializing values of T
            Serde::Serializer::invokeSerdeSpecDeserialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView)
        {
            return (*valueView.m_data).get();
        }

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
            *valueView.m_data = std::move(std::unique_ptr<T0>(pointer));
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::shared_ptr<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<std::shared_ptr<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            if(!*valueView.m_data)
            {
                valueView.getValueContainer().setAsNull();
                return;
            }

            SerializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = valueView.m_data->get();

            // serializing values of T with passing shared data
            Serde::Serializer::invokeSerdeSpecSerialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::shared_ptr<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            // if value is null then doing nothing
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            if constexpr(!std::is_abstract_v<T>)
            {
                // allocating object
                *valueView.m_data = std::make_shared<T>();
            }

            // creating temporary value view that contains object with type T
            DeserializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = valueView.m_data->get();

            // deserializing values of T
            Serde::Serializer::invokeSerdeSpecDeserialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView)
        {
            return (*valueView.m_data).get();
        }

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
            *valueView.m_data = std::move(std::shared_ptr<T0>(pointer));
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<T*, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<T*, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            // if value is null then doing nothing
            if(!*valueView.m_data)
            {
                valueView.getValueContainer().setAsNull();
                return;
            }

            SerializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = *valueView.m_data;

            // serializing values of T with passing shared data
            Serde::Serializer::invokeSerdeSpecSerialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<T*, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            // if value is null then doing nothing
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            if constexpr(!std::is_abstract_v<T>)
            {
                // allocating object
                *valueView.m_data = new T();
            }

            // creating temporary value view that contains object with type T
            DeserializableValueView<T, TFormatType> tmpView = { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_data = *valueView.m_data;

            // deserializing values of T
            Serde::Serializer::invokeSerdeSpecDeserialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView)
        {
            return (*valueView.m_data);
        }

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
            *valueView.m_data = pointer;
        }
    };

    // ====================================================================================

    template<std::floating_point FloatingT, FormatType TFormatType>
    struct SerdeSpec<FloatingT, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "float";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<FloatingT, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsFloat(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<FloatingT, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().getAsFloat();
        }
    };

    // ====================================================================================

    template<std::integral IntegralT, FormatType TFormatType>
    struct SerdeSpec<IntegralT, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "int";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<IntegralT, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<IntegralT, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().getAsInt64();
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::vector<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::vector";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            valueView.getValueContainer().setAsArray();

            for(const auto& v : *valueView.m_data)
            {
                valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
            }
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            *valueView.m_data = valueView.getValueContainer().template getAsArray<T>(std::forward<SharedDataT>(sharedData)...);
        }
    };

    // ====================================================================================

    template<typename CharT, FormatType TFormatType>
    struct SerdeSpec<std::basic_string<CharT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::basic_string";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<std::basic_string<CharT>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsString(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<std::basic_string<CharT>, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().template getAsString<CharT>();
        }
    };

    // ====================================================================================

    template<glm::length_t Length, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::vec<Length, T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::vec";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(std::size_t i = 0; i < Length; ++i)
            {
                valueView.getValueContainer().pushBack((*valueView.m_data)[i]);
            }
        }

        static void deserialize(DeserializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            for(std::size_t i = 0; i < Length; ++i)
            {
                (*valueView.m_data)[i] = vec[i];
            }
        }
    };

    // ====================================================================================

    template<typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::qua<T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::qua";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<glm::qua<T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            valueView.getValueContainer().pushBack(valueView.m_data->x);
            valueView.getValueContainer().pushBack(valueView.m_data->y);
            valueView.getValueContainer().pushBack(valueView.m_data->z);
            valueView.getValueContainer().pushBack(valueView.m_data->w);
        }

        static void deserialize(DeserializableValueView<glm::qua<T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            valueView.m_data->x = vec[0];
            valueView.m_data->y = vec[1];
            valueView.m_data->z = vec[2];
            valueView.m_data->w = vec[3];
        }
    };

    // ====================================================================================

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::mat<C, R, T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::mat";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(std::size_t c = 0; c < C; ++c)
            {
                for(std::size_t r = 0; r < R; ++r)
                {
                    valueView.getValueContainer().pushBack((*valueView.m_data)[c][r]);
                }
            }
        }

        static void deserialize(DeserializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            size_t i = 0;
            for(std::size_t c = 0; c < C; ++c)
            {
                for(std::size_t r = 0; r < R; ++r)
                {
                    (*valueView.m_data)[c][r] = vec[i];
                    ++i;
                }
            }
        }
    };

    // ====================================================================================

    /**
     * KeyT REQUIRES AN IMPLICIT CONVERSION OPERATOR TO std::string OR OTHER TYPES FROM
     * WHICH std::string CAN BE CONSTRUCTED OR WHICH CAN BE IMPLICITLY CONVERTED TO std::string
     * OR CAN BE CONVERTED TO std::string USING std::to_string.
     *
    **/
    template<typename KeyT, typename ValueT, FormatType TFormatType>
    struct SerdeSpec<std::unordered_map<KeyT, ValueT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::unordered_map";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            for(const auto& [key, value] : *valueView.m_data)
            {
                std::string resultKey;
                if constexpr(std::is_convertible_v<KeyT, std::string>)
                {
                    resultKey = key;
                }
                else if constexpr(requires {
                    resultKey = std::to_string(key);
                })
                {
                    resultKey = std::to_string(key);
                }
                else
                {
                    static_assert(always_false<KeyT>::value, "KeyT in std::unordered_map can not be casted to std::string.");
                }

                valueView.getValueContainer().addMember(resultKey, value, std::forward<SharedDataT>(sharedData)...);
            }
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            for(auto it = valueView.getValueContainer().memberBegin(); it != valueView.getValueContainer().memberEnd(); ++it)
            {
                const auto val = valueView.getValueContainer().template getMember<ValueT>(it, std::forward<SharedDataT>(sharedData)...);

                if(val)
                {
                    const std::string memberName = valueView.getValueContainer().getMemberName(it);
                    KeyT resultKey;
                    if constexpr(std::is_convertible_v<std::string, KeyT>)
                    {
                        resultKey = memberName;
                    }
                    else if constexpr(std::numeric_limits<KeyT>::is_integer && std::numeric_limits<KeyT>::is_signed)
                    {
                        resultKey = std::stoll(memberName);
                    }
                    else if constexpr(std::numeric_limits<KeyT>::is_integer && !std::numeric_limits<KeyT>::is_signed)
                    {
                        resultKey = std::stoull(memberName);
                    }
                    else if constexpr(std::is_floating_point_v<KeyT>)
                    {
                        resultKey = std::stold(memberName);
                    }
                    else
                    {
                        static_assert(always_false<KeyT>::value, "std::string can not be casted to KeyT in std::unordered_map.");
                    }

                    (*valueView.m_data)[resultKey] = *val;
                }
            }
        }
    };

    // ====================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<bool, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "bool";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<bool, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsBool(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<bool, TFormatType>& valueView)
        {
            *valueView.m_data = valueView.getValueContainer().getAsBool();
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    requires(std::is_enum_v<T>)
    struct SerdeSpec<T, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "enum";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<T, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(static_cast<std::underlying_type_t<T>>(*valueView.m_data));
        }

        static void deserialize(DeserializableValueView<T, TFormatType>& valueView)
        {
            *valueView.m_data = (T) valueView.getValueContainer().getAsInt64();
        }
    };

    // ====================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<std::filesystem::path, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::filesystem::path";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<std::filesystem::path, TFormatType>& valueView)
        {
            const std::string u8Path = SGCore::Utils::toUTF8(valueView.m_data->u16string());
            valueView.getValueContainer().setAsString(u8Path);
        }

        static void deserialize(DeserializableValueView<std::filesystem::path, TFormatType>& valueView)
        {
            const std::u16string tmpPath = valueView.getValueContainer().template getAsString<char16_t>();
            *valueView.m_data = tmpPath;
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SceneEntitySaveInfo, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SceneEntitySaveInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<SceneEntitySaveInfo, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            auto& serializableScene = *valueView.m_data->m_serializableScene;
            auto& serializableEntity = valueView.m_data->m_serializableEntity;

            auto* entityBaseInfo = serializableScene.getECSRegistry()->template try_get<SGCore::EntityBaseInfo>(serializableEntity);
            if(entityBaseInfo)
            {
                // saving all children entities
                for(const auto& childEntity : entityBaseInfo->getChildren())
                {
                    LOG_I("GENERATED", "Saving CHILD entity '{}'...", std::to_underlying(childEntity));

                    SGCore::SceneEntitySaveInfo childSaveInfo;
                    childSaveInfo.m_serializableScene = &serializableScene;
                    childSaveInfo.m_serializableEntity = childEntity;

                    valueView.getValueContainer().pushBack(childSaveInfo);
                }
            }

            // trying to save standard components ===============================================
            // ==================================================================================

            #pragma region Generated
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::AudioSource>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::Ref<SGCore::Transform>>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::AABB<float>>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::EntityBaseInfo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::RenderingBase>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::Camera3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::SphereGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::BoxGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::LineGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::Atmosphere>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::DirectionalLight>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::Controllable3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template try_get<SGCore::UICamera>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            #pragma endregion Generated

            // ==================================================================================
            // ==================================================================================

            // calling event to serialize user-provided components
            Scene::getOnEntitySerializeEvent<TFormatType>()(
                    valueView,
                    *valueView.m_data->m_serializableScene,
                    valueView.m_data->m_serializableEntity
            );
        }

        static void deserialize(DeserializableValueView<SceneEntitySaveInfo, TFormatType>& valueView, registry_t& toRegistry)
        {
            std::vector<entity_t> childrenEntities;

            // creating entity
            auto entity = toRegistry.create();
            // storing created entity in value view to allow adding components to this entity in event subscriber`s functions
            valueView.m_data->m_serializableEntity = entity;

            // iterating through all elements of entityView
            for(auto componentsIt = valueView.getValueContainer().begin(); componentsIt != valueView.getValueContainer().end(); ++componentsIt)
            {
                // trying to deserialize current element of array (valueView is array) as child SceneEntitySaveInfo
                const std::optional<SceneEntitySaveInfo> asChild =
                        valueView.getValueContainer().template getMember<SceneEntitySaveInfo, custom_derived_types<>>(componentsIt, toRegistry);
                if(asChild)
                {
                    childrenEntities.push_back(asChild->m_serializableEntity);

                    continue;
                }

                // trying to load standard components ===============================================
                // ==================================================================================

                #pragma region Generated

                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::AudioSource>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::AudioSource>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Ref<SGCore::Transform>>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Ref<SGCore::Transform>>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::AABB<float>>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::AABB<float>>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::EntityBaseInfo>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::EntityBaseInfo>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::RenderingBase>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::RenderingBase>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Camera3D>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Camera3D>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::SphereGizmo>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::SphereGizmo>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::BoxGizmo>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::BoxGizmo>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::LineGizmo>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::LineGizmo>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Atmosphere>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Atmosphere>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::DirectionalLight>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::DirectionalLight>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Controllable3D>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Controllable3D>(entity, *component);

                        continue;
                    }
                }
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::UICamera>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::UICamera>(entity, *component);

                        continue;
                    }
                }

                #pragma endregion Generated

                // ==================================================================================
                // ==================================================================================

                // calling event to deserialize user-provided components
                Scene::getOnEntityDeserializeEvent<TFormatType>()(
                        valueView,
                        componentsIt,
                        toRegistry
                );
            }

            // getting EntityBaseInfo of current entity to add all children entities
            EntityBaseInfo* entityBaseInfo = toRegistry.template try_get<EntityBaseInfo>(entity);
            if(!entityBaseInfo)
            {
                return;
            }

            // creating EntityBaseInfo component with 'entity' as identifier in arguments
            *entityBaseInfo = EntityBaseInfo(entity);

            // adding all children
            for(const auto& childEntity : childrenEntities)
            {
                entityBaseInfo->addChild(childEntity, toRegistry);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<registry_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::registry_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<registry_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            SceneEntitySaveInfo sceneEntitySaveInfo;
            sceneEntitySaveInfo.m_serializableScene = &serializableScene;

            auto entitiesView = valueView.m_data->template view<EntityBaseInfo>();
            for(const auto& entity : entitiesView)
            {
                // if entity has component 'NonSavable' then skipping this entity
                if(serializableScene.getECSRegistry()->all_of<NonSavable>(entity)) return;

                // if current savable entity has parent the skip saving this entity because parent saves children entities himself
                EntityBaseInfo* entityBaseInfo = serializableScene.getECSRegistry()->try_get<EntityBaseInfo>(entity);
                if(entityBaseInfo &&
                   entityBaseInfo->getParent() != entt::null &&
                   serializableScene.getECSRegistry()->valid(entityBaseInfo->getParent()))
                {
                    continue;
                }

                sceneEntitySaveInfo.m_serializableEntity = entity;

                valueView.getValueContainer().pushBack(sceneEntitySaveInfo);
            }

            Scene::getOnSceneSavedEvent()(serializableScene);
        }

        static void deserialize(DeserializableValueView<registry_t, TFormatType>& valueView)
        {
            for(auto entityIt = valueView.getValueContainer().begin(); entityIt != valueView.getValueContainer().end(); ++entityIt)
            {
                // deserializing entity and passing registry to getMember to put entity in scene
                valueView.getValueContainer().template getMember<SceneEntitySaveInfo, custom_derived_types<>>(entityIt, *valueView.m_data);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene::systems_container_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Scene::systems_container_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<Scene::systems_container_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            // serializing systems
            for(const Ref<ISystem>& system : *valueView.m_data)
            {
                auto systemPtr = system.get();

                // if system is instance of standard systems then serializing system using standard SerdeSpecs
                if(SG_INSTANCEOF(systemPtr, Controllables3DUpdater) ||
                   SG_INSTANCEOF(systemPtr, PhysicsWorld3D) ||
                   SG_INSTANCEOF(systemPtr, RenderingBasesUpdater) ||
                   SG_INSTANCEOF(systemPtr, AtmosphereUpdater) ||
                   SG_INSTANCEOF(systemPtr, DirectionalLightsUpdater) ||
                   SG_INSTANCEOF(systemPtr, TransformationsUpdater) ||
                   SG_INSTANCEOF(systemPtr, BoxGizmosRenderer) ||
                   SG_INSTANCEOF(systemPtr, LineGizmosRenderer) ||
                   SG_INSTANCEOF(systemPtr, BatchesRenderer) ||
                   SG_INSTANCEOF(systemPtr, SphereGizmosUpdater) ||
                   SG_INSTANCEOF(systemPtr, DebugDraw) ||
                   SG_INSTANCEOF(systemPtr, OctreesSolver) ||
                   SG_INSTANCEOF(systemPtr, AudioProcessor))
                {
                    valueView.getValueContainer().pushBack(system);
                    continue;
                }

                // else if it is not standard system then
                Scene::getOnSystemSerializeEvent<TFormatType>()(valueView, serializableScene, system);
            }
        }

        static void deserialize(DeserializableValueView<Scene::systems_container_t, TFormatType>& valueView)
        {
            // type of auto is equals to Serde::FormatInfo<TFormatType>::array_iterator_t
            for(auto systemsIt = valueView.getValueContainer().begin(); systemsIt != valueView.getValueContainer().end(); ++systemsIt)
            {
                // trying to deserialize current scene using standard SerdeSpecs
                const auto system = valueView.getValueContainer().template getMember<SGCore::Ref<SGCore::ISystem>>(systemsIt);
                if(system)
                {
                    LOG_D(SGCORE_TAG, "LOADING SYSTEM {}", std::string(typeid(**system).name()));

                    valueView.m_data->emplace_back(std::move(*system));
                }
                else // else if we can not deserialize system using standard SerdeSpecs (that means that current system is not standard (user-provided))
                {    // then calling event to try to deserialize current system
                    Scene::getOnSystemDeserializeEvent<TFormatType>()(valueView, systemsIt);
                }
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        // sg_validate_serdespec_supported_formats(TFormatType, FormatType::JSON, FormatType::BSON)

        static inline const std::string type_name = "SGCore::Scene";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<Scene, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_ecsRegistry", *valueView.m_data->m_ecsRegistry, *valueView.m_data);
            valueView.getValueContainer().addMember("m_systems", valueView.m_data->m_systems, *valueView.m_data);
        }

        static void deserialize(DeserializableValueView<Scene, TFormatType>& valueView)
        {
            auto sceneName = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(sceneName)
            {
                valueView.m_data->m_name = std::move(*sceneName);
            }

            auto ecsRegistry = valueView.getValueContainer().template getMember<registry_t>("m_ecsRegistry");
            if(ecsRegistry)
            {
                (*valueView.m_data->getECSRegistry()) = std::move(*ecsRegistry);
            }

            auto systems = valueView.getValueContainer().template getMember<Scene::systems_container_t>("m_systems");
            if(systems)
            {
                for(const auto& system : *systems)
                {
                    valueView.m_data->addSystem(system);
                }
            }
        }
    };

    // ===================================================================================================================
    // ====================================== Standard assets SerdeSpecs impl ============================================
    // ===================================================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<IAsset, TFormatType> : BaseTypes<>, DerivedTypes<ITexture2D>
    {
        static inline const std::string type_name = "SGCore::IAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_path", valueView.m_data->getPath());
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
        }

        static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto assetName = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(assetName)
            {
                valueView.m_data->m_name = std::move(*assetName);
            }

            auto assetPath = valueView.getValueContainer().template getMember<std::string>("m_path");
            if(assetPath)
            {
                valueView.m_data->m_path = std::move(*assetPath);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ITexture2D, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ITexture2D";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            // if we are serializing data too
            if(assetsPackage.isDataSerde())
            {
                AssetsPackage::DataMarkup textureDataMarkup =
                        assetsPackage.addData(valueView.m_data->m_textureData.get(),
                                              valueView.m_data->m_width * valueView.m_data->m_height *
                                              valueView.m_data->m_channelsCount
                        );

                valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
            }

            valueView.getValueContainer().addMember("m_width", valueView.m_data->m_width);
            valueView.getValueContainer().addMember("m_height", valueView.m_data->m_height);
            valueView.getValueContainer().addMember("m_channelsCount", valueView.m_data->m_channelsCount);
            valueView.getValueContainer().addMember("m_internalFormat", valueView.m_data->m_internalFormat);
            valueView.getValueContainer().addMember("m_format", valueView.m_data->m_format);
        }

        static void deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto width = valueView.getValueContainer().template getMember<std::int32_t>("m_width");
            auto height = valueView.getValueContainer().template getMember<std::int32_t>("m_height");
            auto channelsCount = valueView.getValueContainer().template getMember<int>("m_channelsCount");
            auto internalFormat = valueView.getValueContainer().template getMember<SGGColorInternalFormat>("m_internalFormat");
            auto format = valueView.getValueContainer().template getMember<SGGColorFormat>("m_format");

            // if we are deserializing data too
            if(assetsPackage.isDataSerde())
            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

                if(dataOffsetOpt && dataSizeInBytesOpt && width && height && channelsCount && internalFormat && format)
                {
                    char* textureData = FileUtils::readBytesBlock(assetsPackage.getPath(), *dataOffsetOpt, *dataSizeInBytesOpt);

                    valueView.m_data->moveAndCreate(textureData, *width, *height, *channelsCount, *internalFormat, *format);
                }
            }

            // assigning ==============================================

            if(width)
            {
                valueView.m_data->m_width = *width;
            }

            if(height)
            {
                valueView.m_data->m_height = *height;
            }

            if(channelsCount)
            {
                valueView.m_data->m_channelsCount = *channelsCount;
            }

            if(internalFormat)
            {
                valueView.m_data->m_internalFormat = *internalFormat;
            }

            if(format)
            {
                valueView.m_data->m_format = *format;
            }
        }
    };
}

#endif //SUNGEARENGINE_STANDARDSERDESPECS_H
