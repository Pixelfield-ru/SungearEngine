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
#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

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

        static std::unique_ptr<T> allocateObject()
        {
            return std::make_unique<T>();
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::shared_ptr<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

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

        static std::shared_ptr<T> allocateObject()
        {
            return std::make_shared<T>();
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<T*, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView)
        {
            return (*valueView.m_data);
        }

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
            if(*valueView.m_data)
            {
                delete *valueView.m_data;
                *valueView.m_data = nullptr;
            }
            *valueView.m_data = pointer;
        }

        static T* allocateObject()
        {
            return new T();
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

    template<typename IntegerT, FormatType TFormatType>
    requires(std::numeric_limits<IntegerT>::is_integer)
    struct SerdeSpec<IntegerT, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "int";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<IntegerT, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<IntegerT, TFormatType>& valueView)
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
            for(auto& [key, value] : *valueView.m_data)
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
                    KeyT resultKey = fromString<KeyT>(memberName);

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
                        valueView.getValueContainer().template getMember<SceneEntitySaveInfo>(componentsIt, toRegistry);
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
                valueView.getValueContainer().template getMember<SceneEntitySaveInfo>(entityIt, *valueView.m_data);
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
    struct SerdeSpec<IAsset, TFormatType> : BaseTypes<>, DerivedTypes<
            ITexture2D,
            TextFileAsset,
            ShaderAnalyzedFile,
            ModelAsset,
            IMaterial
            >
    {
        static inline const std::string type_name = "SGCore::IAsset";
        static inline constexpr bool is_pointer_type = false;

        /// This function is used only when serializing an asset manager package.
        static void serialize(SerializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            std::cout << "iasset: " << typeid(*valueView.m_data).name() << std::endl;

            // if we are saving data of assets and current serializable asset was not serialized earlier
            assetsPackage.m_useDataSerdeForCurrentAsset = assetsPackage.isDataSerde() && !valueView.m_data->m_hasBeenSerialized;
            // this asset is serialized
            valueView.setDataMemberValue(&IAsset::m_hasBeenSerialized, true);

            valueView.getValueContainer().addMember("m_path", valueView.m_data->getPath());
            valueView.getValueContainer().addMember("m_alias", valueView.m_data->getAlias());
            valueView.getValueContainer().addMember("m_storageType", valueView.m_data->getStorageType());
            valueView.getValueContainer().addMember("m_useDataSerde", assetsPackage.m_useDataSerdeForCurrentAsset);
        }

        /// This function is used in any other cases.
        /*static void serialize(SerializableValueView<IAsset, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_path", valueView.m_data->getPath());
            valueView.getValueContainer().addMember("m_alias", valueView.m_data->getAlias());
            valueView.getValueContainer().addMember("m_storageType", valueView.m_data->getStorageType());
            valueView.getValueContainer().addMember("m_useBinaryFileToSerde", valueView.m_data->m_useBinaryFileToSerde);
        }*/

        /// This function is used only when deserializing an asset manager package.
        static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
            if(assetAlias)
            {
                valueView.m_data->m_alias = std::move(*assetAlias);
            }

            auto assetPath = valueView.getValueContainer().template getMember<std::string>("m_path");
            if(assetPath)
            {
                valueView.m_data->m_path = std::move(*assetPath);
            }

            const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storageType");
            if(assetPath)
            {
                valueView.m_data->m_storageType = *assetStorageType;
            }

            const auto useDataSerde = valueView.getValueContainer().template getMember<bool>("m_useDataSerde");
            if(useDataSerde)
            {
                valueView.m_data->m_useDataSerde = *useDataSerde;
            }
            else
            {
                valueView.m_data->m_useDataSerde = false;
            }
            
            assetsPackage.m_useDataSerdeForCurrentAsset = valueView.m_data->m_useDataSerde;
        }

        /// This function is used in any other cases.
        /*static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView)
        {
            auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
            if(assetAlias)
            {
                valueView.m_data->m_alias = std::move(*assetAlias);
            }

            auto assetPath = valueView.getValueContainer().template getMember<std::string>("m_path");
            if(assetPath)
            {
                valueView.m_data->m_path = std::move(*assetPath);
            }

            const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storageType");
            if(assetPath)
            {
                valueView.m_data->m_storageType = *assetStorageType;
            }

            const auto useBinaryFileToDeserialize = valueView.getValueContainer().template getMember<bool>("m_useBinaryFileToSerde");
            if(useBinaryFileToDeserialize)
            {
                valueView.m_data->m_useBinaryFileToSerde = *useBinaryFileToDeserialize;
            }
            else
            {
                valueView.m_data->m_useBinaryFileToSerde = false;
            }
        }*/
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ITexture2D, TFormatType> : BaseTypes<IAsset>, DerivedTypes<ICubemapTexture>
    {
        /// We indicate the type we are working with.
        static inline const std::string type_name = "SGCore::ITexture2D";
        /// ITexture2D is not pointer type. That is, ITexture2D is not a type that uses RAII to store the pointer and to wrap it.
        static inline constexpr bool is_pointer_type = false;

        /// Required function: This function is ONLY used when serializing assets into a package.
        /// The second argument is the current package being serialized.
        static void serialize(SerializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            std::cout << "texture2d" << std::endl;

            /// The \p m_textureDate field is very large, so we save it to a binary file instead of a JSON file.\n\n
            /// But we only save fields of current asset when we have \p assetsPackage.m_useSerdeForCurrentAsset equals to \p true .
            /// \p assetsPackage.m_useBinarySerdeForCurrentAsset equals to \p true when assets must be serialized along with data (\p assetsPackage.isDataSerde() equals to \p true ),
            /// and current serialized asset DOES NOT SERIALIZED EARLIER (\p valueView.m_data->m_hasBeenSerialized equals to \p false )\n\n
            /// Requirement of necessity of serialization is expressed using the \p m_forceDataSerializing variable in the \p IAsset class.\n\n
            /// This requirement can be useful when the current serializable asset was not loaded from any file (e.g. .gltf or .obj files), but was generated via code.
            /// Thus, to save the data of such an asset, we must specify that serialization of the data of this asset is MANDATORY.\n\n
            /// We also do not save heavy data of the current asset if this asset already exists in the parent asset manager,
            /// i. e. is essentially a duplicate of an existing asset or a reference to an existing asset.
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            /// Next, we serialize the heavy data (in this case, \p m_data )
            /// into a binary package file and get the output markup,
            /// which indicates the position of the \p m_data data in the binary file,
            /// as well as the size of \p m_data in bytes in the binary file.
            AssetsPackage::DataMarkup textureDataMarkup =
                    assetsPackage.addData(valueView.m_data->m_textureData.get(),
                                          valueView.m_data->m_width * valueView.m_data->m_height *
                                          valueView.m_data->m_channelsCount
                    );

            /// Next, we write the markup of heavy data into a JSON file for further deserialization of heavy data from a binary file.
            valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);

            /// We simply add lightweight data directly to the JSON file.
            valueView.getValueContainer().addMember("m_width", valueView.m_data->m_width);
            valueView.getValueContainer().addMember("m_height", valueView.m_data->m_height);
            valueView.getValueContainer().addMember("m_channelsCount", valueView.m_data->m_channelsCount);
            valueView.getValueContainer().addMember("m_internalFormat", valueView.m_data->m_internalFormat);
            valueView.getValueContainer().addMember("m_format", valueView.m_data->m_format);
        }

        /// You should never implement this function.
        // static void serialize(SerializableValueView<ITexture2D, TFormatType>& valueView);

        /// Required function: This function is ONLY used when deserializing assets from a package.\n\n
        /// The second argument is the current package being deserialized.\n\n
        /// This function never deserializes heavy data,
        /// but deserializes the markup of heavy data (offsets and sizes) to write to variables of the currently deserialized asset,
        /// in order to later lazily deserialize the heavy asset data.\n\n
        /// The heavy data of the current asset will be deserialized when this asset is requested from the asset manager.\n
        /// If the current asset was requested from the asset manager, and the heavy data was not deserialized earlier,
        /// then the virtual function \p loadFromBinaryFile(...) is called for the requested asset if
        /// the asset was previously serialized to a binary file or \p load(...) if it was not previously serialized to a binary file,
        /// which means loading by the path to the asset (.gltf, .obj).\n\n
        /// Each implementation of the \p loadFromBinaryFile(...) function must load heavy data by offsets and sizes from a binary file.
        /// So for each member with heavy data in your class inheriting IAsset you must store the offset in the binary file (in bytes) and the size of the data in bytes.
        static void deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            /// If the current asset has not been serialized, then we will not deserialize all the data of this asset.
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            /// We just deserialize lightweight data because it is always in the JSON file (we always serialize lightweight data).
            auto width = valueView.getValueContainer().template getMember<std::int32_t>("m_width");
            auto height = valueView.getValueContainer().template getMember<std::int32_t>("m_height");
            auto channelsCount = valueView.getValueContainer().template getMember<int>("m_channelsCount");
            auto internalFormat = valueView.getValueContainer().template getMember<SGGColorInternalFormat>("m_internalFormat");
            auto format = valueView.getValueContainer().template getMember<SGGColorFormat>("m_format");

            /// Next we deserialize the required offsets and sizes of heavy data.
            auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
            auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

            /// We then assign the deserialized data to our current deserializing asset.

            if(dataOffsetOpt)
            {
                valueView.m_data->m_textureDataOffsetInPackage = *dataOffsetOpt;
            }

            if(dataSizeInBytesOpt)
            {
                valueView.m_data->m_textureDataSizeInPackage = *dataSizeInBytesOpt;
            }

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

        /// You should never implement this function.
        // static void deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView);

        static ITexture2D* allocateObject() noexcept
        {
            return CoreMain::getRenderer()->createTexture2D();
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ICubemapTexture, TFormatType> : BaseTypes<ITexture2D>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ICubemapTexture";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            valueView.getValueContainer().addMember("m_parts", valueView.m_data->m_parts, assetsPackage);
        }

        static void deserialize(DeserializableValueView<ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            auto parts = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_parts)>("m_parts", assetsPackage);
            if(parts)
            {
                valueView.m_data->m_parts = std::move(*parts);
            }
        }

        static ICubemapTexture* allocateObject() noexcept
        {
            return CoreMain::getRenderer()->createCubemapTexture();
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<TextFileAsset, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TextFileAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            AssetsPackage::DataMarkup textureDataMarkup = assetsPackage.addData(valueView.m_data->m_data);

            valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
        }

        static void deserialize(DeserializableValueView<TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
            auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

            if(dataOffsetOpt)
            {
                valueView.m_data->m_dataOffsetInPackage = *dataOffsetOpt;
            }

            if(dataSizeInBytesOpt)
            {
                valueView.m_data->m_dataSizeInPackage = *dataSizeInBytesOpt;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SGSLESubPass, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SGSLESubPass";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<SGSLESubPass, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_subShaders", valueView.m_data->m_subShaders, assetsPackage);
        }

        static void deserialize(DeserializableValueView<SGSLESubPass, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto nameOpt = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(nameOpt)
            {
                valueView.m_data->m_name = std::move(*nameOpt);
            }

            auto subShadersOpt = valueView.getValueContainer().template getMember<std::unordered_map<SGSLESubShaderType, Ref<SGSLESubShader>>>("m_subShaders", assetsPackage);
            if(subShadersOpt)
            {
                valueView.m_data->m_subShaders = std::move(*subShadersOpt);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SGSLESubShader, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SGSLESubShader";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_type", valueView.m_data->m_type);

            AssetsPackage::DataMarkup textureDataMarkup = assetsPackage.addData(valueView.m_data->m_code);

            valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
        }

        static void deserialize(DeserializableValueView<SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto nameOpt = valueView.getValueContainer().template getMember<std::string>("m_name");
            const auto typeOpt = valueView.getValueContainer().template getMember<SGSLESubShaderType>("m_type");
            auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
            auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

            if(nameOpt)
            {
                valueView.m_data->m_name = std::move(*nameOpt);
            }

            if(typeOpt)
            {
                valueView.m_data->m_type = *typeOpt;
            }

            if(dataOffsetOpt)
            {
                valueView.m_data->m_codeOffsetInPackage = *dataOffsetOpt;
            }

            if(dataSizeInBytesOpt)
            {
                valueView.m_data->m_codeSizeInPackage = *dataSizeInBytesOpt;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ShaderAnalyzedFile, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ShaderAnalyzedFile";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            valueView.getValueContainer().addMember("m_subPasses", valueView.m_data->m_subPasses, assetsPackage);
        }

        static void deserialize(DeserializableValueView<ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            auto subPasses = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_subPasses)>("m_subPasses", assetsPackage);

            if(subPasses)
            {
                valueView.m_data->m_subPasses = std::move(*subPasses);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<IMaterial, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IMaterial";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<IMaterial, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);

            valueView.getValueContainer().addMember("m_textures", valueView.m_data->m_textures, assetsPackage);

            valueView.getValueContainer().addMember("m_diffuseColor", valueView.m_data->m_diffuseColor);
            valueView.getValueContainer().addMember("m_specularColor", valueView.m_data->m_specularColor);
            valueView.getValueContainer().addMember("m_ambientColor", valueView.m_data->m_ambientColor);
            valueView.getValueContainer().addMember("m_emissionColor", valueView.m_data->m_emissionColor);
            valueView.getValueContainer().addMember("m_transparentColor", valueView.m_data->m_transparentColor);
            valueView.getValueContainer().addMember("m_shininess", valueView.m_data->m_shininess);
            valueView.getValueContainer().addMember("m_metallicFactor", valueView.m_data->m_metallicFactor);
            valueView.getValueContainer().addMember("m_roughnessFactor", valueView.m_data->m_roughnessFactor);
        }

        static void deserialize(DeserializableValueView<IMaterial, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(name)
            {
                valueView.m_data->m_name = std::move(*name);
            }

            auto textures = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_textures)>("m_textures", assetsPackage);
            if(textures)
            {
                valueView.m_data->m_textures = std::move(*textures);
            }

            const auto diffuseColor = valueView.getValueContainer().template getMember<glm::vec4>("m_diffuseColor");
            if(diffuseColor)
            {
                valueView.m_data->m_diffuseColor = *diffuseColor;
            }

            const auto specularColor = valueView.getValueContainer().template getMember<glm::vec4>("m_specularColor");
            if(specularColor)
            {
                valueView.m_data->m_specularColor = *specularColor;
            }

            const auto ambientColor = valueView.getValueContainer().template getMember<glm::vec4>("m_ambientColor");
            if(ambientColor)
            {
                valueView.m_data->m_ambientColor = *ambientColor;
            }

            const auto emissionColor = valueView.getValueContainer().template getMember<glm::vec4>("m_emissionColor");
            if(emissionColor)
            {
                valueView.m_data->m_emissionColor = *emissionColor;
            }

            const auto transparentColor = valueView.getValueContainer().template getMember<glm::vec4>("m_transparentColor");
            if(transparentColor)
            {
                valueView.m_data->m_transparentColor = *transparentColor;
            }

            const auto shininess = valueView.getValueContainer().template getMember<float>("m_shininess");
            if(shininess)
            {
                valueView.m_data->m_shininess = *shininess;
            }

            const auto metallicFactor = valueView.getValueContainer().template getMember<float>("m_metallicFactor");
            if(metallicFactor)
            {
                valueView.m_data->m_metallicFactor = *metallicFactor;
            }

            const auto roughnessFactor = valueView.getValueContainer().template getMember<float>("m_roughnessFactor");
            if(roughnessFactor)
            {
                valueView.m_data->m_roughnessFactor = *roughnessFactor;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<IMeshData, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IMeshData";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_aabb", valueView.m_data->m_aabb);
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_indices);

                valueView.getValueContainer().addMember("m_indicesOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_indicesSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_positions);

                valueView.getValueContainer().addMember("m_positionsOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_positionsSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_uv);

                valueView.getValueContainer().addMember("m_uvOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_uvSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_normals);

                valueView.getValueContainer().addMember("m_normalsOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_normalsSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_tangents);

                valueView.getValueContainer().addMember("m_tangentsOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_tangentsSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_bitangents);

                valueView.getValueContainer().addMember("m_bitangentsOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_bitangentsSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_bitangents);

                valueView.getValueContainer().addMember("m_bitangentsOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_bitangentsSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            // TODO: MAYBE SERIALIZING DATA OF PHYSICAL MESH (I DONT FUCKING KNOW HOW TO GET DATA FROM btTriangleMesh)
            valueView.getValueContainer().addMember("m_generatePhysicalMesh", valueView.m_data->m_physicalMesh != nullptr);

            valueView.getValueContainer().addMember("m_material", valueView.m_data->m_material, assetsPackage);
        }

        static void deserialize(DeserializableValueView<IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto aabb = valueView.getValueContainer().template getMember<AABB<>>("m_aabb");
            if(aabb)
            {
                valueView.m_data->m_aabb = *aabb;
            }

            auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(name)
            {
                valueView.m_data->m_name = std::move(*name);
            }

            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_indicesOffset"
                );
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_indicesSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_indicesOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_indicesSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_positionsOffset"
                );
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_positionsSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_positionsOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_positionsSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_uvOffset");
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_uvSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_uvOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_uvSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_normalsOffset"
                );
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_normalsSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_normalsOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_normalsSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_tangentsOffset"
                );
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_tangentsSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_tangentsOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_tangentsSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            {
                auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_bitangentsOffset"
                );
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_bitangentsSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_bitangentsOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_bitangentsSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            auto generatePhysicalMesh = valueView.getValueContainer().template getMember<bool>("m_generatePhysicalMesh");
            if(generatePhysicalMesh)
            {
                valueView.m_data->generatePhysicalMesh();
            }
        }

        static IMeshData* allocateObject() noexcept
        {
            return CoreMain::getRenderer()->createMeshData();
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Node, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Node";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<Node, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_children", valueView.m_data->m_children, assetsPackage);
            valueView.getValueContainer().addMember("m_meshesData", valueView.m_data->m_meshesData, assetsPackage);
            valueView.getValueContainer().addMember("m_position", valueView.m_data->m_position);
            valueView.getValueContainer().addMember("m_rotationQuaternion", valueView.m_data->m_rotationQuaternion);
            valueView.getValueContainer().addMember("m_scale", valueView.m_data->m_scale);
        }

        static void deserialize(DeserializableValueView<Node, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(name)
            {
                valueView.m_data->m_name = std::move(*name);
            }

            auto children = valueView.getValueContainer().template getMember<std::vector<Ref<Node>>>("m_children", assetsPackage);
            if(children)
            {
                valueView.m_data->m_children = std::move(*children);
            }

            auto meshesData = valueView.getValueContainer().template getMember<std::vector<Ref<IMeshData>>>("m_meshesData", assetsPackage);
            if(meshesData)
            {
                valueView.m_data->m_meshesData = std::move(*meshesData);
            }

            auto position = valueView.getValueContainer().template getMember<glm::vec3>("m_position");
            if(position)
            {
                valueView.m_data->m_position = *position;
            }

            auto rotationQuaternion = valueView.getValueContainer().template getMember<glm::quat>("m_rotationQuaternion");
            if(rotationQuaternion)
            {
                valueView.m_data->m_rotationQuaternion = *rotationQuaternion;
            }

            auto scale = valueView.getValueContainer().template getMember<glm::vec3>("m_scale");
            if(scale)
            {
                valueView.m_data->m_scale = *scale;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ModelAsset, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ModelAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            valueView.getValueContainer().addMember("m_importerFlags", valueView.m_data->m_importerFlags);
            valueView.getValueContainer().addMember("m_modelName", valueView.m_data->m_modelName);
            valueView.getValueContainer().addMember("m_nodes", valueView.m_data->m_nodes, assetsPackage);
        }

        static void deserialize(DeserializableValueView<ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            if(!assetsPackage.m_useDataSerdeForCurrentAsset) return;

            auto importerFlags = valueView.getValueContainer().template getMember<int>("m_importerFlags");
            if(importerFlags)
            {
                valueView.m_data->m_importerFlags = *importerFlags;
            }

            auto modelName = valueView.getValueContainer().template getMember<std::string>("m_modelName");
            if(modelName)
            {
                valueView.m_data->m_modelName = std::move(*modelName);
            }

            auto nodes = valueView.getValueContainer().template getMember<std::vector<Ref<Node>>>("m_nodes", assetsPackage);
            if(nodes)
            {
                valueView.m_data->m_nodes = std::move(*nodes);
            }
        }
    };
}

#endif //SUNGEARENGINE_STANDARDSERDESPECS_H
