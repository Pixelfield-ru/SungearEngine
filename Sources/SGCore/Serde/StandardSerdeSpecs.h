//
// Created by Ilya on 18.08.2024.
//

#ifndef SUNGEARENGINE_STANDARDSERDESPECS_H
#define SUNGEARENGINE_STANDARDSERDESPECS_H

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include "Serde.h"
#include <glm/glm.hpp>

#include "SGCore/Actions/AlwaysFalseAction.h"
#include "SGCore/Actions/KeyboardKeyAction.h"
#include "SGCore/Actions/KeyboardKeyDownAction.h"
#include "SGCore/Actions/KeyboardKeyPressedAction.h"
#include "SGCore/Actions/KeyboardKeyReleasedAction.h"

#include "SGCore/Memory/Assets/AnimationsFile.h"
#include "SGCore/Scene/EntityRef.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Render/Picking/Pickable.h"

#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Math/AABB.h"
#include "SGCore/Scene/Layer.h"
#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/Gizmos/GizmoBase.h"
#include "SGCore/Render/Gizmos/SphereGizmo.h"
#include "SGCore/Render/Gizmos/BoxGizmo.h"
#include "SGCore/Render/Gizmos/LineGizmo.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Render/MeshBase.h"
#include "SGCore/Render/Mesh.h"
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
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/SpacePartitioning/OctreesSolver.h"
#include "SGCore/Audio/AudioProcessor.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"

#include "SGCore/Serde/Components/NonSavable.h"

#include "SGCore/Animation/MotionPlanner.h"
#include "SGCore/Animation/MotionPlannerConnection.h"
#include "SGCore/Animation/MotionPlannersResolver.h"

#include "SGCore/Main/Config.h"
#include "SGCore/Physics/Rigidbody3D.h"

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;
};
// =================================================================================


// SERDE FORWARD DECL FOR struct 'SGCore::ShaderTextureBinding'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::ShaderTextureBinding, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::ShaderTextureBinding";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderTextureBinding, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::ShaderTextureBinding, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::ShaderDefine'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::ShaderDefine, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::ShaderDefine";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderDefine, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::ShaderDefine, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::EntityRef'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::EntityRef, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::EntityRef";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::EntityRef, TFormatType>& valueView,
                          const ECS::entity_t& deserializedEntity,
                          ECS::registry_t& toRegistry) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityRef, TFormatType>& valueView,
                            const ECS::entity_t& deserializedEntity,
                            ECS::registry_t& toRegistry) noexcept;
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::AudioSource, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioSource, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::Rigidbody3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Rigidbody3D, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Rigidbody3D";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Rigidbody3D, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Rigidbody3D, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'btBoxShape'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<btBoxShape, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<

                                   >
{
    static inline const std::string type_name = "btBoxShape";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const btBoxShape, TFormatType>& valueView,
                          const btTransform& shapeTransform,
                          const Rigidbody3D& parentRigidbody3D) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<btBoxShape, TFormatType>& valueView,
                            btTransform& shapeTransform,
                            Rigidbody3D& parentRigidbody3D) noexcept;
};


// SERDE FORWARD DECL FOR struct 'btCompoundShape'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<btCompoundShape, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<

                                   >
{
    static inline const std::string type_name = "btCompoundShape";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const btCompoundShape, TFormatType>& valueView,
                          const btTransform& shapeTransform,
                          const Rigidbody3D& parentRigidbody3D) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<btCompoundShape, TFormatType>& valueView,
                            btTransform& shapeTransform,
                            Rigidbody3D& parentRigidbody3D) noexcept;
};


// SERDE FORWARD DECL FOR struct 'btCollisionShape'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<btCollisionShape, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<

                                   >
{
    static inline const std::string type_name = "btCollisionShape";
    static inline constexpr bool is_pointer_type = false;

    template<typename... SharedDataT>
    static void serialize(SGCore::Serde::SerializableValueView<const btCollisionShape, TFormatType>& valueView, const Rigidbody3D& parentRigidbody3D) noexcept;

    template<typename... SharedDataT>
    static void deserialize(SGCore::Serde::DeserializableValueView<btCollisionShape, TFormatType>& valueView, Rigidbody3D& parentRigidbody3D) noexcept;

    template<typename... SharedDataT>
    static void serialize(SGCore::Serde::SerializableValueView<const btCollisionShape, TFormatType>& valueView, const btTransform& shapeTransform, const Rigidbody3D& parentRigidbody3D) noexcept;

    template<typename... SharedDataT>
    static void deserialize(SGCore::Serde::DeserializableValueView<btCollisionShape, TFormatType>& valueView, btTransform& shapeTransform, Rigidbody3D& parentRigidbody3D) noexcept;

    static btCollisionShape* allocateObject(DeserializableValueView<btCollisionShape, TFormatType>& valueView) noexcept
    {
        const auto shapeType = valueView.getValueContainer().template getMember<int>("m_shapeType");

        if(!shapeType)
        {
            SG_ASSERT(false, "Can not allocate btCollisionShape: can not detect field 'm_shapeType'!");
            return nullptr;
        }

        if(*shapeType == COMPOUND_SHAPE_PROXYTYPE)
        {
            return new btCompoundShape;
        }
        if(*shapeType == BOX_SHAPE_PROXYTYPE)
        {
            return new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
        }
        if(*shapeType == SPHERE_SHAPE_PROXYTYPE)
        {
            return new btSphereShape(1.0f);
        }

        SG_ASSERT(false, fmt::format("Can not allocate btCollisionShape: unknown shape type: '{}'!", *shapeType).c_str());
        return nullptr;
    }
};


// SERDE FOR struct 'btTransform'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<btTransform, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<

                                   >
{
    static inline const std::string type_name = "btTransform";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const btTransform, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_origin", valueView.m_data->getOrigin());

        valueView.getValueContainer().addMember("m_rotation", valueView.m_data->getRotation());
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<btTransform, TFormatType>& valueView) noexcept
    {
        const auto origin = valueView.getValueContainer().template getMember<btVector3>("m_origin");
        if(origin)
        {
            valueView.m_data->setOrigin(*origin);
        }

        const auto rotation = valueView.getValueContainer().template getMember<btQuaternion>("m_rotation");
        if(rotation)
        {
            valueView.m_data->setRotation(*rotation);
        }
    }
};


// SERDE FOR struct 'btVector3'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<btVector3, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<

                                   >
{
    static inline const std::string type_name = "btVector3";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const btVector3, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("x", valueView.m_data->getX());
        valueView.getValueContainer().addMember("y", valueView.m_data->getY());
        valueView.getValueContainer().addMember("z", valueView.m_data->getZ());
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<btVector3, TFormatType>& valueView) noexcept
    {
        const auto x = valueView.getValueContainer().template getMember<btScalar>("x");
        if(x)
        {
            valueView.m_data->setX(*x);
        }

        const auto y = valueView.getValueContainer().template getMember<btScalar>("y");
        if(y)
        {
            valueView.m_data->setY(*y);
        }

        const auto z = valueView.getValueContainer().template getMember<btScalar>("z");
        if(z)
        {
            valueView.m_data->setZ(*z);
        }
    }
};

// SERDE FOR struct 'btQuaternion'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<btQuaternion, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<

                                   >
{
    static inline const std::string type_name = "btQuaternion";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const btQuaternion, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("x", valueView.m_data->getX());
        valueView.getValueContainer().addMember("y", valueView.m_data->getY());
        valueView.getValueContainer().addMember("z", valueView.m_data->getZ());
        valueView.getValueContainer().addMember("w", valueView.m_data->getW());
    }

    static void deserialize(SGCore::Serde::DeserializableValueView<btQuaternion, TFormatType>& valueView) noexcept
    {
        const auto x = valueView.getValueContainer().template getMember<btScalar>("x");
        if(x)
        {
            valueView.m_data->setX(*x);
        }

        const auto y = valueView.getValueContainer().template getMember<btScalar>("y");
        if(y)
        {
            valueView.m_data->setY(*y);
        }

        const auto z = valueView.getValueContainer().template getMember<btScalar>("z");
        if(z)
        {
            valueView.m_data->setZ(*z);
        }

        const auto w = valueView.getValueContainer().template getMember<btScalar>("w");
        if(w)
        {
            valueView.m_data->setW(*w);
        }
    }
};


// SERDE IMPL FOR struct 'btCompoundShape'
// =================================================================================
template<SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<btCompoundShape, TFormatType>::serialize(
    SGCore::Serde::SerializableValueView<const btCompoundShape, TFormatType>& valueView,
    const btTransform& shapeTransform,
    const Rigidbody3D& parentRigidbody3D) noexcept
{
    const auto* value = valueView.m_data;

    valueView.getValueContainer().setAsArray();

    for(size_t i = 0; i < value->getNumChildShapes(); ++i)
    {
        const btCollisionShape* shape = value->getChildShape(i);

        valueView.getValueContainer().pushBack(shape, value->getChildTransform(i), parentRigidbody3D);
    }
}

template<SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<btCompoundShape, TFormatType>::deserialize(
    SGCore::Serde::DeserializableValueView<btCompoundShape, TFormatType>& valueView,
    btTransform& shapeTransform,
    Rigidbody3D& parentRigidbody3D) noexcept
{
    btTransform childShapeTransform;

    auto* value = valueView.m_data;

    for(auto it = valueView.getValueContainer().begin(); it != valueView.getValueContainer().end(); ++it)
    {
        childShapeTransform.setIdentity();
        const auto shape = valueView.getValueContainer().template getMember<Ref<btCollisionShape>>(it, childShapeTransform, parentRigidbody3D);

        if(shape)
        {
            value->addChildShape(childShapeTransform, shape->get());
            // saving reference to shape to avoid deletion of current child shape
            parentRigidbody3D.m_shapes.push_back(*shape);
            // WRONG!
            // parentRigidbody3D.addShape(childShapeTransform, *shape);
        }
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'btBoxShape'
// =================================================================================
template<SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<btBoxShape, TFormatType>::serialize(
    SGCore::Serde::SerializableValueView<const btBoxShape, TFormatType>& valueView,
    const btTransform& shapeTransform,
    const Rigidbody3D& parentRigidbody3D) noexcept
{

}

template<SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<btBoxShape, TFormatType>::deserialize(
    SGCore::Serde::DeserializableValueView<btBoxShape, TFormatType>& valueView,
    btTransform& shapeTransform,
    Rigidbody3D& parentRigidbody3D) noexcept
{

}
// =================================================================================


// SERDE IMPL FOR struct 'btCollisionShape'
// =================================================================================

template<SGCore::Serde::FormatType TFormatType>
template<typename... SharedDataT>
void SGCore::Serde::SerdeSpec<btCollisionShape, TFormatType>::serialize(
    SGCore::Serde::SerializableValueView<const btCollisionShape, TFormatType>& valueView,
    const Rigidbody3D& parentRigidbody3D) noexcept
{
    const btCollisionShape* data = valueView.m_data;
    const auto shapeType = data->getShapeType();

    valueView.getValueContainer().addMember("m_localScaling", valueView.m_data->getLocalScaling());
    valueView.getValueContainer().addMember("m_shapeType", shapeType);

    // direct saving shape of actual type

    if(shapeType == COMPOUND_SHAPE_PROXYTYPE)
    {
        const auto* compoundShape = static_cast<const btCompoundShape*>(data);

        valueView.getValueContainer().addMember("m_shapeObject", *compoundShape, btTransform {}, parentRigidbody3D);
    }
    else if(shapeType == BOX_SHAPE_PROXYTYPE)
    {
        const auto* boxShape = static_cast<const btBoxShape*>(data);

        valueView.getValueContainer().addMember("m_shapeObject", *boxShape, btTransform {}, parentRigidbody3D);
    }
    else if(shapeType == SPHERE_SHAPE_PROXYTYPE)
    {
        const auto* sphereShape = static_cast<const btSphereShape*>(data);

        // todo: add saving
    }
    else
    {
        LOG_E(SGCORE_TAG, "Error while serializing physical collision shape: unknown shape type: '{}'", shapeType);
    }
}

template<SGCore::Serde::FormatType TFormatType>
template<typename... SharedDataT>
void SGCore::Serde::SerdeSpec<btCollisionShape, TFormatType>::deserialize(
    SGCore::Serde::DeserializableValueView<btCollisionShape, TFormatType>& valueView,
    Rigidbody3D& parentRigidbody3D) noexcept
{
    const auto localScaling = valueView.getValueContainer().template getMember<btVector3>("m_localScaling");
    if(localScaling)
    {
        valueView.m_data->setLocalScaling(*localScaling);
    }

    const auto shapeType = valueView.getValueContainer().template getMember<int>("m_shapeType");
    if(!shapeType)
    {
        LOG_E(SGCORE_TAG, "Error while deserializing physical collision shape: no 'm_shapeType' field detected!");
        return;
    }

    btTransform shapeTransform; // unused

    if(*shapeType == COMPOUND_SHAPE_PROXYTYPE)
    {
        const auto shape = valueView.getValueContainer().template getMember<Ref<btCompoundShape>>("m_shapeObject", shapeTransform, parentRigidbody3D);
        if(!shape)
        {
            LOG_E(SGCORE_TAG, "Error while deserializing physical collision shape: no 'm_shapeObject' field detected!");
            return;
        }

        auto* thisCompoundShape = static_cast<btCompoundShape*>(valueView.m_data);
        auto* otherCompoundShape = shape->get();

        for(int i = 0; i < otherCompoundShape->getNumChildShapes(); ++i)
        {
            thisCompoundShape->addChildShape(otherCompoundShape->getChildTransform(i), otherCompoundShape->getChildShape(i));
        }
    }
    else if(*shapeType == BOX_SHAPE_PROXYTYPE)
    {

    }
    else if(*shapeType == SPHERE_SHAPE_PROXYTYPE)
    {

    }
    else
    {
        LOG_E(SGCORE_TAG, "Error while deserializing physical collision shape: unknown shape type: '{}'", *shapeType);
    }
}

template<SGCore::Serde::FormatType TFormatType>
template<typename... SharedDataT>
void SGCore::Serde::SerdeSpec<btCollisionShape, TFormatType>::serialize(
    SGCore::Serde::SerializableValueView<const btCollisionShape, TFormatType>& valueView,
    const btTransform& shapeTransform,
    const Rigidbody3D& parentRigidbody3D) noexcept
{
    serialize(valueView, parentRigidbody3D);

    valueView.getValueContainer().addMember("m_transform", shapeTransform);
}

template<SGCore::Serde::FormatType TFormatType>
template<typename... SharedDataT>
void SGCore::Serde::SerdeSpec<btCollisionShape, TFormatType>::deserialize(
    SGCore::Serde::DeserializableValueView<btCollisionShape, TFormatType>& valueView,
    btTransform& shapeTransform,
    Rigidbody3D& parentRigidbody3D) noexcept
{
    deserialize(valueView, parentRigidbody3D);

    const auto transform = valueView.getValueContainer().template getMember<btTransform>("m_transform");
    if(transform)
    {
        shapeTransform = *transform;
    }
}
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Transform, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueName, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Layer, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::TransformBase, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::Pickable'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Pickable, TFormatType> :
        SGCore::Serde::BaseTypes<
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Pickable";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Pickable, TFormatType>& valueView,
                          const ECS::entity_t& deserializableEntity,
                          ECS::registry_t& toRegistry) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Pickable, TFormatType>& valueView,
                            const ECS::entity_t& deserializableEntity,
                            ECS::registry_t& toRegistry) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::RenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::RenderState, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::RenderState";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderState, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderState, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::MeshRenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::MeshRenderState, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::MeshRenderState";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::MeshRenderState, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshRenderState, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::BlendingState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::BlendingState, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::BlendingState";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::BlendingState, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::BlendingState, TFormatType>& valueView) noexcept;
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderingBase, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Camera3D, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::GizmoBase, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::SphereGizmo, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::BoxGizmo, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::LineGizmo, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Atmosphere, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::MeshBase, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::Mesh'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Mesh, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Mesh";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Mesh, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Mesh, TFormatType>& valueView) noexcept;
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::AtmosphereUpdater, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::LightBase, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::IShader'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::IShader, TFormatType> :
        SGCore::Serde::BaseTypes<
                        IAsset
                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::IShader";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::IShader, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::IShader, TFormatType>& valueView) noexcept;

    static IShader* allocateObject(DeserializableValueView<IShader, TFormatType>& valueView) noexcept
    {
        return CoreMain::getRenderer()->createShader();
    }
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::DirectionalLight, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::PhysicsWorld3D, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Controllable3D, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::UICamera, TFormatType>& valueView) noexcept;

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
                SGCore::Controllables3DUpdater,
                SGCore::PhysicsWorld3D,
                SGCore::RenderingBasesUpdater,
                SGCore::AtmosphereUpdater,
                SGCore::DirectionalLightsUpdater,
                SGCore::TransformationsUpdater,
                SGCore::BoxGizmosRenderer,
                SGCore::LineGizmosRenderer,
                SGCore::SphereGizmosUpdater,
                SGCore::OctreesSolver,
                SGCore::AudioProcessor,
                SGCore::MotionPlannersResolver
                                   >
{
    static inline const std::string type_name = "SGCore::ISystem";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::ISystem, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderingBasesUpdater, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::DirectionalLightsUpdater, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::TransformationsUpdater, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::BoxGizmosRenderer, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept;
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept;
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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::OctreesSolver, TFormatType>& valueView) noexcept;

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

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::AudioProcessor, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioProcessor, TFormatType>& valueView) noexcept;
};
// =================================================================================



// implementations



// SERDE IMPL FOR struct 'SGCore::UniqueNameWrapper'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::AudioSource, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::AudioSource, TFormatType>& valueView) noexcept
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


// SERDE IMPL FOR struct 'SGCore::Rigidbody3D'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Rigidbody3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Rigidbody3D, TFormatType>& valueView) noexcept
{
    auto& valueContainer = valueView.getValueContainer();
    auto& value = valueView.m_data;

    valueContainer.addMember("m_mass", value->m_body->getMass());

    const auto inertia = value->m_body->getLocalInertia();
    valueContainer.addMember("m_inertia", inertia);

    valueContainer.addMember("m_finalShape", static_cast<btCollisionShape*>(value->m_finalShape.get()), btTransform {}, *valueView.m_data);

    valueContainer.addMember("m_type", value->getType());
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Rigidbody3D, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Rigidbody3D, TFormatType>& valueView) noexcept
{
    const auto mass = valueView.getValueContainer().template getMember<btScalar>("m_mass");
    const auto localInertia = valueView.getValueContainer().template getMember<btVector3>("m_inertia");

    if(mass && localInertia)
    {
        valueView.m_data->m_body->setMassProps(*mass, *localInertia);
    }

    // deserializing final shape (compound shape)
    btTransform finalShapeTransform; // unused
    auto finalShape = valueView.getValueContainer().template getMember<Ref<btCollisionShape>>("m_finalShape", finalShapeTransform, *valueView.m_data);
    if(finalShape && (*finalShape)->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
    {
       valueView.m_data->m_finalShape = std::move(std::static_pointer_cast<btCompoundShape>(*finalShape));
       valueView.m_data->m_body->setCollisionShape(finalShape->get());
    }

    const auto type = valueView.getValueContainer().template getMember<PhysicalObjectType>("m_type");
    if(type)
    {
        valueView.m_data->setType(*type);
    }
}

// =================================================================================



// SERDE IMPL FOR struct 'SGCore::Transform'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Transform, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Transform, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueName, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::AABB<ScalarT>, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::AABB<ScalarT>, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Layer, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::TransformBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::TransformBase, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_thisEntity", valueView.m_data->getThisEntity());
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityBaseInfo, TFormatType>& valueView) noexcept
{
    const auto m_deserializedThisEntity = valueView.getValueContainer().template getMember<ECS::entity_t>("m_thisEntity");

    if(m_deserializedThisEntity)
    {
        valueView.m_data->m_deserializedThisEntity = static_cast<ECS::entity_t>(*m_deserializedThisEntity);
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::Pickable'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Pickable, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Pickable, TFormatType>& valueView,
                                                                        const ECS::entity_t& deserializableEntity,
                                                                        ECS::registry_t& toRegistry) noexcept
{
    valueView.getValueContainer().addMember("m_pickableForCameras", valueView.m_data->m_pickableForCameras,
                                            deserializableEntity, toRegistry);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Pickable, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Pickable, TFormatType>& valueView,
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
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::RenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderState, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderState, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_globalBlendingState", valueView.m_data->m_globalBlendingState);

    valueView.getValueContainer().addMember("m_useDepthTest", valueView.m_data->m_useDepthTest);
    valueView.getValueContainer().addMember("m_depthFunc", valueView.m_data->m_depthFunc);
    valueView.getValueContainer().addMember("m_depthMask", valueView.m_data->m_depthMask);

    valueView.getValueContainer().addMember("m_useStencilTest", valueView.m_data->m_useStencilTest);
    valueView.getValueContainer().addMember("m_stencilFunc", valueView.m_data->m_stencilFunc);
    valueView.getValueContainer().addMember("m_stencilFuncRef", valueView.m_data->m_stencilFuncRef);
    valueView.getValueContainer().addMember("m_stencilFuncMask", valueView.m_data->m_stencilFuncMask);
    valueView.getValueContainer().addMember("m_stencilMask", valueView.m_data->m_stencilMask);
    valueView.getValueContainer().addMember("m_stencilFailOp", valueView.m_data->m_stencilFailOp);
    valueView.getValueContainer().addMember("m_stencilZFailOp", valueView.m_data->m_stencilZFailOp);
    valueView.getValueContainer().addMember("m_stencilZPassOp", valueView.m_data->m_stencilZPassOp);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderState, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderState, TFormatType>& valueView) noexcept
{
    const auto m_globalBlendingState = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_globalBlendingState)>("m_globalBlendingState");
    if(m_globalBlendingState)
    {
        valueView.m_data->m_globalBlendingState = *m_globalBlendingState;
    }

    // ===================================

    const auto m_useDepthTest = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useDepthTest)>("m_useDepthTest");
    if(m_useDepthTest)
    {
        valueView.m_data->m_useDepthTest = *m_useDepthTest;
    }

    const auto m_depthFunc = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_depthFunc)>("m_depthFunc");
    if(m_depthFunc)
    {
        valueView.m_data->m_depthFunc = *m_depthFunc;
    }

    const auto m_depthMask = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_depthMask)>("m_depthMask");
    if(m_depthMask)
    {
        valueView.m_data->m_depthMask = *m_depthMask;
    }

    // ===================================

    const auto m_useStencilTest = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useStencilTest)>("m_useStencilTest");
    if(m_useStencilTest)
    {
        valueView.m_data->m_useStencilTest = *m_useStencilTest;
    }

    const auto m_stencilFunc = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFunc)>("m_stencilFunc");
    if(m_stencilFunc)
    {
        valueView.m_data->m_stencilFunc = *m_stencilFunc;
    }

    const auto m_stencilFuncRef = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFuncRef)>("m_stencilFuncRef");
    if(m_stencilFuncRef)
    {
        valueView.m_data->m_stencilFuncRef = *m_stencilFuncRef;
    }

    const auto m_stencilFuncMask = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFuncMask)>("m_stencilFuncMask");
    if(m_stencilFuncMask)
    {
        valueView.m_data->m_stencilFuncMask = *m_stencilFuncMask;
    }

    const auto m_stencilMask = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilMask)>("m_stencilMask");
    if(m_stencilMask)
    {
        valueView.m_data->m_stencilMask = *m_stencilMask;
    }

    const auto m_stencilFailOp = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFailOp)>("m_stencilFailOp");
    if(m_stencilFailOp)
    {
        valueView.m_data->m_stencilFailOp = *m_stencilFailOp;
    }

    const auto m_stencilZFailOp = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilZFailOp)>("m_stencilZFailOp");
    if(m_stencilZFailOp)
    {
        valueView.m_data->m_stencilZFailOp = *m_stencilZFailOp;
    }

    const auto m_stencilZPassOp = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilZPassOp)>("m_stencilZPassOp");
    if(m_stencilZPassOp)
    {
        valueView.m_data->m_stencilZPassOp = *m_stencilZPassOp;
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::MeshRenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshRenderState, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::MeshRenderState, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_useIndices", valueView.m_data->m_useIndices);

    valueView.getValueContainer().addMember("m_useFacesCulling", valueView.m_data->m_useFacesCulling);
    valueView.getValueContainer().addMember("m_facesCullingFaceType", valueView.m_data->m_facesCullingFaceType);
    valueView.getValueContainer().addMember("m_facesCullingPolygonsOrder", valueView.m_data->m_facesCullingPolygonsOrder);

    valueView.getValueContainer().addMember("m_drawMode", valueView.m_data->m_drawMode);

    valueView.getValueContainer().addMember("m_linesWidth", valueView.m_data->m_linesWidth);
    valueView.getValueContainer().addMember("m_pointsSize", valueView.m_data->m_pointsSize);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshRenderState, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshRenderState, TFormatType>& valueView) noexcept
{

    const auto m_useIndices = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useIndices)>("m_useIndices");
    if(m_useIndices)
    {
        valueView.m_data->m_useIndices = *m_useIndices;
    }

    const auto m_enableFacesCulling = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useFacesCulling)>("m_useFacesCulling");
    if(m_enableFacesCulling)
    {
        valueView.m_data->m_useFacesCulling = *m_enableFacesCulling;
    }

    const auto m_facesCullingFaceType = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_facesCullingFaceType)>("m_facesCullingFaceType");
    if(m_facesCullingFaceType)
    {
        valueView.m_data->m_facesCullingFaceType = *m_facesCullingFaceType;
    }

    const auto m_facesCullingPolygonsOrder = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_facesCullingPolygonsOrder)>("m_facesCullingPolygonsOrder");
    if(m_facesCullingPolygonsOrder)
    {
        valueView.m_data->m_facesCullingPolygonsOrder = *m_facesCullingPolygonsOrder;
    }

    const auto m_drawMode = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_drawMode)>("m_drawMode");
    if(m_drawMode)
    {
        valueView.m_data->m_drawMode = *m_drawMode;
    }

    const auto m_linesWidth = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_linesWidth)>("m_linesWidth");
    if(m_linesWidth)
    {
        valueView.m_data->m_linesWidth = *m_linesWidth;
    }

    const auto m_pointsSize = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_pointsSize)>("m_pointsSize");
    if(m_pointsSize)
    {
        valueView.m_data->m_pointsSize = *m_pointsSize;
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::BlendingState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BlendingState, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::BlendingState, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_useBlending", valueView.m_data->m_useBlending);

    valueView.getValueContainer().addMember("m_forAttachment", valueView.m_data->m_forAttachment);

    valueView.getValueContainer().addMember("m_sFactor", valueView.m_data->m_sFactor);
    valueView.getValueContainer().addMember("m_dFactor", valueView.m_data->m_dFactor);
    valueView.getValueContainer().addMember("m_blendingEquation", valueView.m_data->m_blendingEquation);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BlendingState, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::BlendingState, TFormatType>& valueView) noexcept
{

    const auto m_useBlending = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useBlending)>("m_useBlending");
    if(m_useBlending)
    {
        valueView.m_data->m_useBlending = *m_useBlending;
    }

    const auto m_forAttachment = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_forAttachment)>("m_forAttachment");
    if(m_forAttachment)
    {
        valueView.m_data->m_forAttachment = *m_forAttachment;
    }

    const auto m_sFactor = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_sFactor)>("m_sFactor");
    if(m_sFactor)
    {
        valueView.m_data->m_sFactor = *m_sFactor;
    }

    const auto m_dFactor = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_dFactor)>("m_dFactor");
    if(m_dFactor)
    {
        valueView.m_data->m_dFactor = *m_dFactor;
    }

    const auto m_blendingEquation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_blendingEquation)>("m_blendingEquation");
    if(m_blendingEquation)
    {
        valueView.m_data->m_blendingEquation = *m_blendingEquation;
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::RenderingBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderingBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderingBase, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::Camera3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Camera3D, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::GizmoBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::GizmoBase, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::SphereGizmo, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::BoxGizmo, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_base", valueView.m_data->m_base);




    valueView.getValueContainer().addMember("m_size", valueView.m_data->m_size);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmo, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::BoxGizmo, TFormatType>& valueView) noexcept
{
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
void SGCore::Serde::SerdeSpec<SGCore::LineGizmo, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::LineGizmo, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::Atmosphere, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Atmosphere, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_sunPosition", valueView.m_data->m_sunPosition);




    valueView.getValueContainer().addMember("m_sunAmbient", valueView.m_data->m_sunAmbient);




    valueView.getValueContainer().addMember("m_rayleighScatteringCoeff", valueView.m_data->m_rayleighScatteringCoeff);




    valueView.getValueContainer().addMember("m_atmosphereRadius", valueView.m_data->m_atmosphereRadius);




    valueView.getValueContainer().addMember("m_sunRotation", valueView.m_data->m_sunRotation);




    valueView.getValueContainer().addMember("m_mieScatteringCoeff", valueView.m_data->m_mieScatteringCoeff);




    valueView.getValueContainer().addMember("m_rayleighScaleHeight", valueView.m_data->m_rayleighScaleHeight);




    valueView.getValueContainer().addMember("m_mieScaleHeight", valueView.m_data->m_mieScaleHeight);




    valueView.getValueContainer().addMember("m_sunIntensity", valueView.m_data->m_sunIntensity);




    valueView.getValueContainer().addMember("m_planetRadius", valueView.m_data->m_planetRadius);




    valueView.getValueContainer().addMember("m_miePreferredScatteringDirection", valueView.m_data->m_miePreferredScatteringDirection);




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


    const auto m_mieScatteringCoeff = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_mieScatteringCoeff)>("m_mieScatteringCoeff");

    if(m_mieScatteringCoeff)
    {
        valueView.m_data->m_mieScatteringCoeff = *m_mieScatteringCoeff;
    }


    const auto m_rayleighScaleHeight = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rayleighScaleHeight)>("m_rayleighScaleHeight");

    if(m_rayleighScaleHeight)
    {
        valueView.m_data->m_rayleighScaleHeight = *m_rayleighScaleHeight;
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


    const auto m_planetRadius = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_planetRadius)>("m_planetRadius");

    if(m_planetRadius)
    {
        valueView.m_data->m_planetRadius = *m_planetRadius;
    }


    const auto m_miePreferredScatteringDirection = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_miePreferredScatteringDirection)>("m_miePreferredScatteringDirection");

    if(m_miePreferredScatteringDirection)
    {
        valueView.m_data->m_miePreferredScatteringDirection = *m_miePreferredScatteringDirection;
    }


    const auto m_rayOrigin = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rayOrigin)>("m_rayOrigin");

    if(m_rayOrigin)
    {
        valueView.m_data->m_rayOrigin = *m_rayOrigin;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::GizmoBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Mesh, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Mesh, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_base", valueView.m_data->m_base);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Mesh, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Mesh, TFormatType>& valueView) noexcept
{
    const auto m_base = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_base)>("m_base");

    if(m_base)
    {
        valueView.m_data->m_base = *m_base;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::MeshBase'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::MeshBase, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_material", valueView.m_data->getMaterial());




    // valueView.getValueContainer().addMember("m_meshDataRenderInfo", valueView.m_data->m_meshDataRenderInfo);




    valueView.getValueContainer().addMember("m_meshData", valueView.m_data->getMeshData());
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshBase, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshBase, TFormatType>& valueView) noexcept
{
    /*const auto m_meshDataRenderInfo = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_meshDataRenderInfo)>("m_meshDataRenderInfo");

    if(m_meshDataRenderInfo)
    {
        valueView.m_data->m_meshDataRenderInfo = *m_meshDataRenderInfo;
    }*/


    // TODO:
    const auto m_meshData = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMeshData())>>>("m_meshData");

    if(m_meshData)
    {
        LOG_D(SGCORE_TAG, "Deserializing meshdata from mesh base...");
        valueView.m_data->setMeshData(*m_meshData);
    }

    // WE DESERIALIZING MATERIAL STRICTLY AFTER m_meshData BECAUSE MATERIAL CAN BE CUSTOM
    // TODO:
    const auto m_material = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->getMaterial())>>>("m_material");

    if(m_material)
    {
        valueView.m_data->setMaterial(*m_material);
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::IShader'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::IShader, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::IShader, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_fileAssetPath", valueView.m_data->getFile()->getPath());
    valueView.getValueContainer().addMember("m_autoRecompile", valueView.m_data->m_autoRecompile);
    valueView.getValueContainer().addMember("m_textureBindings", valueView.m_data->m_textureBindings);
    valueView.getValueContainer().addMember("m_defines", valueView.m_data->m_defines);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::IShader, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::IShader, TFormatType>& valueView) noexcept
{
    const auto m_fileAssetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_fileAssetPath");

    if(m_fileAssetPath)
    {
        auto shaderFile = valueView.m_data->getParentAssetManager()->template loadAsset<TextFileAsset>(*m_fileAssetPath);
        auto shaderAnalyzedFile = valueView.m_data->getParentAssetManager()->template loadAsset<ShaderAnalyzedFile>(*m_fileAssetPath);
        valueView.m_data->m_fileAsset = shaderFile;
        valueView.m_data->m_shaderAnalyzedFile = shaderAnalyzedFile;
    }

    const auto m_autoRecompile = valueView.getValueContainer().template getMember<bool>("m_autoRecompile");
    if(m_autoRecompile)
    {
        valueView.m_data->m_autoRecompile = *m_autoRecompile;
    }

    auto m_textureBindings = valueView.getValueContainer().template getMember<std::vector<ShaderTextureBinding>>("m_textureBindings");
    if(m_textureBindings)
    {
        valueView.m_data->m_textureBindings = std::move(*m_textureBindings);
    }

    auto m_defines = valueView.getValueContainer().template getMember<decltype(IShader::m_defines)>("m_defines");
    if(m_defines)
    {
        valueView.m_data->m_defines = std::move(*m_defines);
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::AtmosphereUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AtmosphereUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::AtmosphereUpdater, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::LightBase, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::LightBase, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::Controllables3DUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Controllables3DUpdater, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::DirectionalLight, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::DirectionalLight, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::PhysicsWorld3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::PhysicsWorld3D, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::Controllable3D, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Controllable3D, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::UICamera, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::UICamera, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::ISystem, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::ISystem, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::IParallelSystem<ParallelSystemT>, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::IParallelSystem<ParallelSystemT>, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::RenderingBasesUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderingBasesUpdater, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::DirectionalLightsUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::DirectionalLightsUpdater, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::TransformationsUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::TransformationsUpdater, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::BoxGizmosRenderer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::BoxGizmosRenderer, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::LineGizmosRenderer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::LineGizmosRenderer, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::LineGizmosRenderer, TFormatType>& valueView) noexcept
{
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::SphereGizmosUpdater'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmosUpdater, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::SphereGizmosUpdater, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::SphereGizmosUpdater, TFormatType>& valueView) noexcept
{
}
// =================================================================================





// SERDE IMPL FOR struct 'SGCore::OctreesSolver'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::OctreesSolver, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::OctreesSolver, TFormatType>& valueView) noexcept
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
void SGCore::Serde::SerdeSpec<SGCore::AudioProcessor, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::AudioProcessor, TFormatType>& valueView) noexcept
{
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::AudioProcessor, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::AudioProcessor, TFormatType>& valueView) noexcept
{
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::EntityRef'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityRef, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::EntityRef, TFormatType>& valueView,
                                                                         const ECS::entity_t& deserializedEntity,
                                                                         ECS::registry_t& toRegistry) noexcept
{
    // if referenced entity is non savable then we do not serialize reference to referenced entity
    if(toRegistry.anyOf<NonSavable>(*valueView.m_data->m_referencedEntity))
    {
        valueView.discard();
        return;
    }

    valueView.getValueContainer().addMember("m_referencedEntity", valueView.m_data->m_referencedEntity);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityRef, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityRef, TFormatType>& valueView,
                                                                           const ECS::entity_t& deserializedEntity,
                                                                           ECS::registry_t& toRegistry) noexcept
{
    auto m_referencedEntity = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_referencedEntity)>("m_referencedEntity");

    if(m_referencedEntity)
    {
        valueView.m_data->m_referencedEntity = std::move(*m_referencedEntity);

        // IT IS GUARANTEED THAT ENTITY BASE INFO OF deserializedEntity IS ALREADY EXIST!!!
        auto* entityBaseInfo = toRegistry.tryGet<EntityBaseInfo>(deserializedEntity);
        SG_ASSERT(entityBaseInfo != nullptr,
                  fmt::format("Can not mark EntityRef (points to entity '{}') as needing to resolve! "
                              "Entity '{}' that contains component that contains this EntityRef does not have EntityBaseInfo!",
                              std::to_underlying(*valueView.m_data->m_referencedEntity),
                              std::to_underlying(deserializedEntity)).c_str());

        entityBaseInfo->m_entitiesRefsToResolve.push_back(valueView.m_data->m_referencedEntity);
    }
}
// =================================================================================

#pragma endregion Generated

namespace SGCore::Serde
{
    template<FormatType TFormatType>
    struct SerdeSpec<LoadablePluginConfig, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::LoadablePluginConfig";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const LoadablePluginConfig, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_pluginName", valueView.m_data->m_pluginName);
            valueView.getValueContainer().addMember("m_pluginPath", valueView.m_data->m_pluginPath);
            valueView.getValueContainer().addMember("m_pluginEntryArgs", valueView.m_data->m_pluginEntryArgs);
            valueView.getValueContainer().addMember("m_pluginCMakeBuildDir", valueView.m_data->m_pluginCMakeBuildDir);
            valueView.getValueContainer().addMember("m_enabled", valueView.m_data->m_enabled);
        }

        static void deserialize(DeserializableValueView<LoadablePluginConfig, TFormatType>& valueView)
        {
            auto m_pluginName = valueView.getValueContainer().template getMember<std::string>("m_pluginName");
            if(m_pluginName)
            {
                valueView.m_data->m_pluginName = std::move(*m_pluginName);
            }

            auto m_pluginPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_pluginPath");
            if(m_pluginPath)
            {
                valueView.m_data->m_pluginPath = std::move(*m_pluginPath);
            }

            auto m_pluginEntryArgs = valueView.getValueContainer().template getMember<std::vector<std::string>>("m_pluginEntryArgs");
            if(m_pluginEntryArgs)
            {
                valueView.m_data->m_pluginEntryArgs = std::move(*m_pluginEntryArgs);
            }

            auto m_pluginCMakeBuildDir = valueView.getValueContainer().template getMember<std::string>("m_pluginCMakeBuildDir");
            if(m_pluginCMakeBuildDir)
            {
                valueView.m_data->m_pluginCMakeBuildDir = std::move(*m_pluginCMakeBuildDir);
            }

            auto m_enabled = valueView.getValueContainer().template getMember<bool>("m_enabled");
            if(m_enabled)
            {
                valueView.m_data->m_enabled = *m_enabled;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Config, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Config";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Config, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_loadablePlugins", valueView.m_data->m_loadablePlugins);
        }

        static void deserialize(DeserializableValueView<Config, TFormatType>& valueView)
        {
            auto m_loadablePlugins = valueView.getValueContainer().template getMember<std::vector<LoadablePluginConfig>>("m_loadablePlugins");
            if(m_loadablePlugins)
            {
                valueView.m_data->m_loadablePlugins = std::move(*m_loadablePlugins);
            }
        }
    };

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

        static std::unique_ptr<T> allocateObject(DeserializableValueView<std::unique_ptr<T>, TFormatType>& valueView)
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

        static std::shared_ptr<T> allocateObject(DeserializableValueView<std::shared_ptr<T>, TFormatType>& valueView)
        {
            // if we have public access to constructor of T
            if constexpr(requires { T::T(); })
            {
                return std::make_shared<T>();
            }
            else
            {
                // else trying to allocate using raw pointer
                return std::shared_ptr<T>(new T);
            }
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

        static T* allocateObject(DeserializableValueView<T*, TFormatType>& valueView)
        {
            return new T();
        }
    };

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<T const*, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename ValueViewT>
        static const T* getObjectRawPointer(ValueViewT& valueView)
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

        static T* allocateObject(DeserializableValueView<T const*, TFormatType>& valueView)
        {
            return new T();
        }
    };

    // ====================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<AssetsPackage::DataMarkup, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AssetsPackage::DataMarkup";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AssetsPackage::DataMarkup, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_offset", valueView.m_data->m_offset);
            valueView.getValueContainer().addMember("m_sizeInBytes", valueView.m_data->m_sizeInBytes);
        }

        static void deserialize(DeserializableValueView<AssetsPackage::DataMarkup, TFormatType>& valueView)
        {
            const auto m_offset = valueView.getValueContainer().template getMember<std::streamsize>("m_offset");
            if(m_offset)
            {
                valueView.m_data->m_offset = *m_offset;
            }

            const auto m_sizeInBytes = valueView.getValueContainer().template getMember<std::streamsize>("m_sizeInBytes");
            if(m_sizeInBytes)
            {
                valueView.m_data->m_sizeInBytes = *m_sizeInBytes;
            }
        }
    };

    // ====================================================================================

    template<std::floating_point FloatingT, FormatType TFormatType>
    struct SerdeSpec<FloatingT, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "float";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const FloatingT, TFormatType>& valueView)
        {
            const FloatingT finalValue = std::isfinite(*valueView.m_data) ? *valueView.m_data : 0.0;
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

        static void serialize(SerializableValueView<const IntegerT, TFormatType>& valueView)
        {
            const IntegerT finalVal = std::isfinite(*valueView.m_data) ? *valueView.m_data : 0;

            valueView.getValueContainer().setAsInt64(finalVal);
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
        static void serialize(SerializableValueView<const std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
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

    // SERDE FORWARD DECL FOR struct 'SGCore::TransparentEntityTag'
    // =================================================================================

    template<
            SGCore::Serde::FormatType TFormatType
    >
    struct SerdeSpec<SGCore::TransparentEntityTag, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TransparentEntityTag";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SGCore::Serde::SerializableValueView<const SGCore::TransparentEntityTag, TFormatType>& valueView) noexcept
        {

        }

        static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::TransparentEntityTag, TFormatType>& valueView) noexcept
        {

        }
    };

    // =================================================================================

    // SERDE FORWARD DECL FOR struct 'SGCore::OpaqueEntityTag'
    // =================================================================================

    template<
            SGCore::Serde::FormatType TFormatType
    >
    struct SerdeSpec<SGCore::OpaqueEntityTag, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::OpaqueEntityTag";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SGCore::Serde::SerializableValueView<const SGCore::OpaqueEntityTag, TFormatType>& valueView) noexcept
        {

        }

        static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::OpaqueEntityTag, TFormatType>& valueView) noexcept
        {

        }
    };

    // =================================================================================

    template<typename T, size_t Size, FormatType TFormatType>
    struct SerdeSpec<std::array<T, Size>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::array";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::array<T, Size>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            valueView.getValueContainer().setAsArray();

            for(const auto& v : *valueView.m_data)
            {
                valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
            }
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::array<T, Size>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            auto vec = valueView.getValueContainer().template getAsArray<T>(std::forward<SharedDataT>(sharedData)...);
            for(size_t i = 0; i < Size; ++i)
            {
                (*valueView.m_data)[i] = std::move(vec[i]);
            }
        }
    };

    // ====================================================================================

    template<typename T, typename HashT, typename EqualT, FormatType TFormatType>
    struct SerdeSpec<std::unordered_set<T, HashT, EqualT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        using collection_t = std::unordered_set<T, HashT, EqualT>;

        static inline const std::string type_name = "std::unordered_set";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const collection_t, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            valueView.getValueContainer().setAsArray();

            for(const auto& v : *valueView.m_data)
            {
                valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
            }
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<collection_t, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            auto valuesVec = valueView.getValueContainer().template getAsArray<T>(std::forward<SharedDataT>(sharedData)...);
            for(auto&& v : valuesVec)
            {
                valueView.m_data->emplace(std::forward<T>(v));
            }
        }
    };

    // ====================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::list<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::list";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::list<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            valueView.getValueContainer().setAsArray();

            for(const auto& v : *valueView.m_data)
            {
                valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
            }
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::list<T>, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            *valueView.m_data = valueView.getValueContainer().template getAsArray<T, std::list>(std::forward<SharedDataT>(sharedData)...);
        }
    };

    // ====================================================================================

    template<typename CharT, FormatType TFormatType>
    struct SerdeSpec<std::basic_string<CharT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "std::basic_string";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const std::basic_string<CharT>, TFormatType>& valueView)
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

        static void serialize(SerializableValueView<const glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
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

        static void serialize(SerializableValueView<const glm::qua<T, Qualifier>, TFormatType>& valueView)
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

        static void serialize(SerializableValueView<const glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
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
        static void serialize(SerializableValueView<const std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData)
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
                else if constexpr(std::is_enum_v<KeyT>)
                {
                    resultKey = std::to_string(std::to_underlying(key));
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

        static void serialize(SerializableValueView<const bool, TFormatType>& valueView)
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

        static void serialize(SerializableValueView<const T, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(std::to_underlying(*valueView.m_data));
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

        static void serialize(SerializableValueView<const std::filesystem::path, TFormatType>& valueView)
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
    struct SerdeSpec<InterpolatedPath, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::InterpolatedPath";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const InterpolatedPath, TFormatType>& valueView)
        {
            const std::string u8Path = SGCore::Utils::toUTF8(valueView.m_data->raw().u16string());
            valueView.getValueContainer().setAsString(u8Path);
        }

        static void deserialize(DeserializableValueView<InterpolatedPath, TFormatType>& valueView)
        {
            const std::u16string tmpPath = valueView.getValueContainer().template getAsString<char16_t>();
            *valueView.m_data = tmpPath;
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ECS::entity_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ECS::entity_t";
        static inline constexpr bool is_pointer_type = false;

        // behaviour as just enum value
        static void serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(std::to_underlying(*valueView.m_data));
        }

        // behaviour as just enum value
        static void deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView)
        {
            *valueView.m_data = static_cast<ECS::entity_t>(valueView.getValueContainer().getAsInt64());
        }

        // behaviour as serializable entity for scene
        static void serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            auto& serializableEntity = *valueView.m_data;

            auto* entityBaseInfo = serializableScene.getECSRegistry()->template tryGet<SGCore::EntityBaseInfo>(serializableEntity);
            if(entityBaseInfo)
            {
                // saving all children entities
                for(const auto& childEntity : entityBaseInfo->getChildren())
                {
                    // if entity has component 'NonSavable' then skipping this entity
                    if(serializableScene.getECSRegistry()->template allOf<NonSavable>(childEntity)) continue;

                    LOG_I("GENERATED", "Saving CHILD entity '{}'...", std::to_underlying(childEntity));

                    valueView.getValueContainer().pushBack(childEntity, serializableScene);
                }
            }

            // trying to save standard components ===============================================
            // ==================================================================================

            #pragma region Components

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::EntityBaseInfo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::AudioSource>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Transform>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::AABB<float>>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::RenderingBase>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Camera3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::SphereGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::BoxGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::LineGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Atmosphere>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Mesh>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::DirectionalLight>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Controllable3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::UICamera>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::TransparentEntityTag>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::OpaqueEntityTag>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Pickable>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component,
                                                           serializableEntity, *serializableScene.getECSRegistry());
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::MotionPlanner>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component,
                                                           serializableEntity, *serializableScene.getECSRegistry());
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Rigidbody3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component,
                                                           serializableEntity, *serializableScene.getECSRegistry());
                }
            }

            #pragma endregion Components

            // ==================================================================================
            // ==================================================================================

            // calling event to serialize user-provided components
            Scene::getOnEntitySerializeEvent<TFormatType>()(
                    valueView,
                    serializableScene
            );
        }

        // behaviour as deserializable entity for scene
        static void deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView, Scene& deserializableScene)
        {
            std::vector<ECS::entity_t> childrenEntities;

            auto& toRegistry = *deserializableScene.getECSRegistry();

            // creating entity
            auto entity = toRegistry.create();
            // storing created entity in value view to allow adding components to this entity in event subscriber`s functions
            *valueView.m_data = entity;

            // getting EntityBaseInfo of current entity to add all children entities
            auto& entityBaseInfo = toRegistry.get<EntityBaseInfo>(entity);

            LOG_I(SGCORE_TAG, "Loading entity: {}", std::to_underlying(*valueView.m_data));

            // iterating through all elements of entityView
            for(auto componentsIt = valueView.getValueContainer().begin(); componentsIt != valueView.getValueContainer().end(); ++componentsIt)
            {
                const auto& currentElementTypeName = valueView.getValueContainer().getMemberTypeName(componentsIt);

                // deserializing iterator as child entity only if current iterator is SceneEntitySaveInfo
                if(currentElementTypeName == type_name)
                {
                    // trying to deserialize current element of array (valueView is array) as child SceneEntitySaveInfo
                    const std::optional<ECS::entity_t> asChild =
                            valueView.getValueContainer().template getMember<ECS::entity_t>(componentsIt,
                                deserializableScene);
                    if(asChild)
                    {
                        childrenEntities.push_back(*asChild);
                        LOG_E(SGCORE_TAG, "Loaded child entity: {}",
                              std::to_underlying(*asChild));

                    }
                    continue;
                }

                // trying to load standard components ===============================================
                // ==================================================================================

                #pragma region Components

                if(currentElementTypeName == SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::type_name)
                {
                    auto component = valueView.getValueContainer().template getMember<SGCore::EntityBaseInfo::reg_t>(componentsIt);

                    if(component)
                    {
                        entityBaseInfo = std::move(*component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::AudioSource, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::AudioSource::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::AudioSource>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Transform, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Transform::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Transform>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::AABB<float>, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::AABB<float>::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::AABB<float>>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::RenderingBase, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::RenderingBase::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::RenderingBase>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Camera3D, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Camera3D::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Camera3D>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::SphereGizmo, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::SphereGizmo::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::SphereGizmo>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::BoxGizmo, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::BoxGizmo::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::BoxGizmo>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::LineGizmo, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::LineGizmo::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::LineGizmo>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Atmosphere, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Atmosphere::reg_t>(componentsIt);

                    if(component)
                    {
                        LOG_D(SGCORE_TAG, "Atmosphere component deserializing");
                        toRegistry.emplace<SGCore::Atmosphere>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Mesh, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Mesh::reg_t>(componentsIt);

                    if(component)
                    {
                        LOG_D(SGCORE_TAG, "Mesh component deserializing");
                        toRegistry.emplace<SGCore::Mesh>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::DirectionalLight, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::DirectionalLight::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::DirectionalLight>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Controllable3D, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Controllable3D::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Controllable3D>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::UICamera, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::UICamera::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::UICamera>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Pickable, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Pickable::reg_t>(componentsIt, entity, toRegistry);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Pickable>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::TransparentEntityTag, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::TransparentEntityTag::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::TransparentEntityTag>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::OpaqueEntityTag, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::OpaqueEntityTag::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::OpaqueEntityTag>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::MotionPlanner, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::MotionPlanner::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::MotionPlanner>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Rigidbody3D, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Rigidbody3D::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Rigidbody3D>(entity, *component)->setParentWorld(deserializableScene.getSystem<PhysicsWorld3D>());

                        continue;
                    }
                }

                #pragma endregion Components

                // ==================================================================================
                // ==================================================================================

                // calling event to deserialize user-provided components
                Scene::getOnEntityDeserializeEvent<TFormatType>()(
                        valueView,
                        componentsIt,
                        deserializableScene
                );
            }

            // creating EntityBaseInfo component with 'entity' as identifier in arguments
            entityBaseInfo.setThisEntity(entity);

            // adding all children
            for(const auto& childEntity : childrenEntities)
            {
                LOG_I(SGCORE_TAG, "Adding child entity '{}' to parent entity '{}'...", std::to_underlying(childEntity), std::to_underlying(entity));
                entityBaseInfo.addChild(childEntity, toRegistry);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ECS::registry_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ECS::registry_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ECS::registry_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            auto entitiesView = valueView.m_data->template view<EntityBaseInfo>();
            for(const auto& entity : entitiesView)
            {
                LOG_I(SGCORE_TAG, "Trying to save entity '{}'", std::to_underlying(entity))

                // if entity has component 'NonSavable' then skipping this entity
                if(serializableScene.getECSRegistry()->anyOf<NonSavable>(entity)) continue;

                // if current savable entity has parent then
                // skip saving this entity because parent saves children entities himself
                auto* entityBaseInfo = serializableScene.getECSRegistry()->tryGet<EntityBaseInfo>(entity);
                if(entityBaseInfo &&
                   entityBaseInfo->getParent() != entt::null &&
                   serializableScene.getECSRegistry()->valid(entityBaseInfo->getParent()))
                {
                    continue;
                }

                LOG_I(SGCORE_TAG, "Saving ROOT entity '{}'", std::to_underlying(entity))
                valueView.getValueContainer().pushBack(entity, serializableScene);
            }

            Scene::getOnSceneSavedEvent()(serializableScene);
        }

        static void deserialize(DeserializableValueView<ECS::registry_t, TFormatType>& valueView, Scene& serializableScene)
        {
            for(auto entityIt = valueView.getValueContainer().begin(); entityIt != valueView.getValueContainer().end(); ++entityIt)
            {
                // deserializing entity and passing registry to getMember to put entity in scene
                valueView.getValueContainer().template getMember<ECS::entity_t>(entityIt, serializableScene);
            }

            // resolving all EntityRef`s after deserialization ================

            serializableScene.resolveAllEntitiesRefs();

            // =================================================================
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene::systems_container_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Scene::systems_container_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Scene::systems_container_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            // serializing systems
            for(const Ref<ISystem>& system : *valueView.m_data)
            {
                auto systemPtr = system.get();

                // if system is instance of standard systems then serializing system using standard SerdeSpecs
                // TODO: THIS IS SO SHITTY. REMOVE WHEN CUSTOM DERIVED TYPES IN EXTERNAL PROJECTS WILL BE AVAILABLE IN SGCore::Serde
                if(SG_INSTANCEOF(systemPtr, Controllables3DUpdater) ||
                   SG_INSTANCEOF(systemPtr, PhysicsWorld3D) ||
                   SG_INSTANCEOF(systemPtr, RenderingBasesUpdater) ||
                   SG_INSTANCEOF(systemPtr, AtmosphereUpdater) ||
                   SG_INSTANCEOF(systemPtr, DirectionalLightsUpdater) ||
                   SG_INSTANCEOF(systemPtr, TransformationsUpdater) ||
                   SG_INSTANCEOF(systemPtr, BoxGizmosRenderer) ||
                   SG_INSTANCEOF(systemPtr, LineGizmosRenderer) ||
                   SG_INSTANCEOF(systemPtr, SphereGizmosUpdater) ||
                   SG_INSTANCEOF(systemPtr, OctreesSolver) ||
                   SG_INSTANCEOF(systemPtr, AudioProcessor) ||
                   SG_INSTANCEOF(systemPtr, MotionPlannersResolver))
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
    struct SerdeSpec<SceneMetaInfo, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SceneMetaInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SceneMetaInfo, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_sceneName", valueView.m_data->m_sceneName);
            valueView.getValueContainer().addMember("m_sceneLocalPath", valueView.m_data->m_sceneLocalPath);
        }

        static void deserialize(DeserializableValueView<SceneMetaInfo, TFormatType>& valueView)
        {
            auto sceneName = valueView.getValueContainer().template getMember<std::string>("m_sceneName");
            if(sceneName)
            {
                valueView.m_data->m_sceneName = std::move(*sceneName);
            }

            auto sceneLocalPath = valueView.getValueContainer().template getMember<std::filesystem::path>("m_sceneLocalPath");
            if(sceneLocalPath)
            {
                valueView.m_data->m_sceneLocalPath = std::move(*sceneLocalPath);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        // sg_validate_serdespec_supported_formats(TFormatType, FormatType::JSON, FormatType::BSON)

        static inline const std::string type_name = "SGCore::Scene";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Scene, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_metaInfo", valueView.m_data->m_metaInfo);
            valueView.getValueContainer().addMember("m_systems", valueView.m_data->m_systems, *valueView.m_data);
            valueView.getValueContainer().addMember("m_ecsRegistry", *valueView.m_data->m_ecsRegistry, *valueView.m_data);
        }

        static void deserialize(DeserializableValueView<Scene, TFormatType>& valueView)
        {
            auto metaInfo = valueView.getValueContainer().template getMember<SceneMetaInfo>("m_metaInfo");
            if(metaInfo)
            {
                valueView.m_data->m_metaInfo = std::move(*metaInfo);
            }

            auto systems = valueView.getValueContainer().template getMember<Scene::systems_container_t>("m_systems");
            if(systems)
            {
                for(const auto& system : *systems)
                {
                    valueView.m_data->addSystem(system);
                }
            }

            valueView.getValueContainer().template getMember<ECS::registry_t>("m_ecsRegistry", *valueView.m_data);
            /*if(ecsRegistry)
            {
                (*valueView.m_data->getECSRegistry()) = std::move(*ecsRegistry);
            }*/
        }
    };

    // ===================================================================================================================
    // ====================================== Standard assets SerdeSpecs impl ============================================
    // ===================================================================================================================

    template<typename AssetT, FormatType TFormatType>
    struct SerdeSpec<AssetRef<AssetT>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AssetRef";
        static inline constexpr bool is_pointer_type = false;

        // WE ARE SERIALIZING ONLY META INFO OF ASSET BECAUSE IT IS ASSET REFERENCE. WE DO NOT NEED TO DO SERIALIZATION OF DATA
        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const AssetRef<AssetT>, TFormatType>& valueView, SharedDataT&&...)
        {
            if(!valueView.m_data->m_asset)
            {
                valueView.getValueContainer().setAsNull();
                return;
            }

            valueView.getValueContainer().addMember("m_path", (*valueView.m_data)->getPath());
            valueView.getValueContainer().addMember("m_assetTypeID", (*valueView.m_data)->getTypeID());
            valueView.getValueContainer().addMember("m_alias", (*valueView.m_data)->getAlias());
            valueView.getValueContainer().addMember("m_storedBy", (*valueView.m_data)->storedByWhat());
            valueView.getValueContainer().addMember("m_parentAssetManagerName", (*valueView.m_data)->getParentAssetManager()->getName());
        }

        // WE ARE DESERIALIZING ONLY META INFO OF ASSET BECAUSE IT IS ASSET REFERENCE. WE DO NOT NEED TO DO DESERIALIZATION OF DATA
        static void deserialize(DeserializableValueView<AssetRef<AssetT>, TFormatType>& valueView, AssetsPackage&)
        {
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
            const auto assetTypeID = valueView.getValueContainer().template getMember<size_t>("m_assetTypeID");
            auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
            const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
            const auto parentAssetManagerName = valueView.getValueContainer().template getMember<std::string>("m_parentAssetManagerName");

            if(!parentAssetManagerName) return;

            auto parentAssetManager = AssetManager::getAssetManager(*parentAssetManagerName);

            // WE DO NOT set parent asset manager because we are getting already existing asset from asset manager
            // and this asset is already has parent asset manager

            // checking if asset is already exists
            // TRYING TO RESOLVE REFERENCE AUTOMATICALLY
            if (parentAssetManager->isAssetExists(*assetAlias, *assetPath, *assetStorageType, *assetTypeID))
            {
                // setting m_asset to asset from parent asset manager
                LOG_I(SGCORE_TAG, "Asset is already exist. Reference was resolved automatically.");
                valueView.m_data->m_asset =
                        std::static_pointer_cast<AssetT>(
                                parentAssetManager->loadExistingAsset(*assetAlias, *assetPath, *assetStorageType, *assetTypeID).m_asset);
            }

            // assigning values only after getting asset from asset manager
            valueView.m_data->m_deserializedAssetAlias = std::move(*assetAlias);
            valueView.m_data->m_deserializedAssetPath = std::move(*assetPath);
            valueView.m_data->m_deserializedAssetStoredBy = *assetStorageType;
            valueView.m_data->m_deserializedAssetTypeID = *assetTypeID;
            valueView.m_data->m_deserializedParentAssetManager = parentAssetManager;
        }

        static void deserialize(DeserializableValueView<AssetRef<AssetT>, TFormatType>& valueView)
        {
            if(valueView.getValueContainer().isNull())
            {
                return;
            }

            auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
            const auto assetTypeID = valueView.getValueContainer().template getMember<size_t>("m_assetTypeID");
            auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
            const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
            const auto parentAssetManagerName = valueView.getValueContainer().template getMember<std::string>("m_parentAssetManagerName");

            LOG_I(SGCORE_TAG, "Deserializing AssetRef... AssetRef data: path: '{}', alias: '{}', asset type ID: '{}', stored by: '{}'",
                  SGCore::Utils::toUTF8(assetPath->resolved().u16string()),
                  *assetAlias,
                  *assetTypeID,
                  std::to_underlying(*assetStorageType));

            if(!parentAssetManagerName) return;

            auto parentAssetManager = AssetManager::getAssetManager(*parentAssetManagerName);

            // WE DO NOT set parent asset manager because we are getting already existing asset from asset manager
            // and this asset is already has parent asset manager

            // checking if asset is already exists
            // TRYING TO RESOLVE REFERENCE AUTOMATICALLY
            bool assetResolvedAutomatically = false;
            if (parentAssetManager->isAssetExists(*assetAlias, *assetPath, *assetStorageType, *assetTypeID))
            {
                // setting m_asset to asset from parent asset manager
                LOG_I(SGCORE_TAG, "Asset is already exist. Reference was resolved automatically.");
                valueView.m_data->m_asset =
                        std::static_pointer_cast<AssetT>(
                                parentAssetManager->loadExistingAsset(*assetAlias, *assetPath, *assetStorageType, *assetTypeID).m_asset);

                assetResolvedAutomatically = true;
            }

            // assigning values only after getting asset from asset manager
            valueView.m_data->m_deserializedAssetAlias = *assetAlias;
            valueView.m_data->m_deserializedAssetPath = *assetPath;
            valueView.m_data->m_deserializedAssetStoredBy = *assetStorageType;
            valueView.m_data->m_deserializedAssetTypeID = *assetTypeID;
            valueView.m_data->m_deserializedParentAssetManager = parentAssetManager;

            if(assetResolvedAutomatically)
            {
                return;
            }

            // if asset ref was serialized by path and path is valid and if we are deserializing this ref
            // not from assets package then we are trying to load asset from filesystem
            if(*assetStorageType == AssetStorageType::BY_PATH && std::filesystem::exists(assetPath->resolved()))
            {
                LOG_I(SGCORE_TAG, "Loading AssetRef by path... AssetRef data: path: '{}', alias: '{}', type ID: '{}'",
                      SGCore::Utils::toUTF8(assetPath->resolved().u16string()),
                      *assetAlias,
                      *assetTypeID);

                valueView.m_data->m_asset = parentAssetManager->template loadAsset<AssetT>(*assetPath).m_asset;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<IAsset, TFormatType> : BaseTypes<>, DerivedTypes<
            ITexture2D,
            TextFileAsset,
            ShaderAnalyzedFile,
            ModelAsset,
            IMaterial,
            IMeshData,
            IShader,
            Skeleton,
            AnimationsFile,
            Bone,
            SkeletalAnimationAsset
            >
    {
        static inline const std::string type_name = "SGCore::IAsset";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const IAsset, TFormatType>& valueView, SharedDataT&&...)
        {
            valueView.getValueContainer().addMember("m_path", valueView.m_data->getPath());
            valueView.getValueContainer().addMember("m_alias", valueView.m_data->getAlias());
            valueView.getValueContainer().addMember("m_storedBy", valueView.m_data->storedByWhat());
            valueView.getValueContainer().addMember("m_useDataSerde", true);
        }

        /// This function is used only when deserializing an asset manager package.
        static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
            if(assetAlias)
            {
                valueView.m_data->m_alias = std::move(*assetAlias);
            }

            auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
            if(assetPath)
            {
                valueView.m_data->m_path = std::move(*assetPath);
            }

            const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
            if(assetPath)
            {
                valueView.m_data->m_storedBy = *assetStorageType;
            }

            const auto useDataSerde = valueView.getValueContainer().template getMember<bool>("m_useDataSerde");
            if(useDataSerde)
            {
                valueView.m_data->m_isSavedInBinaryFile = *useDataSerde;
            }
            else
            {
                valueView.m_data->m_isSavedInBinaryFile = false;
            }

            valueView.m_data->m_parentAssetManager = assetsPackage.getParentAssetManager()->shared_from_this();
        }

        /// This function is used when deserializing directly from file that contains only this asset.
        static void deserialize(DeserializableValueView<IAsset, TFormatType>& valueView)
        {
            auto assetAlias = valueView.getValueContainer().template getMember<std::string>("m_alias");
            if(assetAlias)
            {
                valueView.m_data->m_alias = std::move(*assetAlias);
            }

            auto assetPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_path");
            if(assetPath)
            {
                valueView.m_data->m_path = std::move(*assetPath);
            }

            const auto assetStorageType = valueView.getValueContainer().template getMember<AssetStorageType>("m_storedBy");
            if(assetPath)
            {
                valueView.m_data->m_storedBy = *assetStorageType;
            }

            const auto useDataSerde = valueView.getValueContainer().template getMember<bool>("m_useDataSerde");
            if(useDataSerde)
            {
                valueView.m_data->m_isSavedInBinaryFile = *useDataSerde;
            }
            else
            {
                valueView.m_data->m_isSavedInBinaryFile = false;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ITexture2D, TFormatType> : BaseTypes<IAsset>, DerivedTypes<ICubemapTexture>
    {
        /// We indicate the type we are working with.
        static inline const std::string type_name = "SGCore::ITexture2D";
        /// ITexture2D is not pointer type. That is, ITexture2D is not a type that uses RAII to store the pointer and to wrap it.
        static inline constexpr bool is_pointer_type = false;

        /// Required function: This function is ONLY used when serializing assets into a package.\n
        /// You can implement serialize function that does not accept AssetsPackage for serializing asset directly to file.\n
        /// The second argument is the current package being serialized.
        static void serialize(SerializableValueView<const ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
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
        // static void serialize(SerializableValueView<const ITexture2D, TFormatType>& valueView);

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
        /// So for each member with heavy data in your class inheriting IAsset you must store the offset in the binary file (in bytes) and the size of the data in bytes.\n\n
        /// You can implement \p deserialize function that does not accept \p AssetsPackage for serializing asset directly to file.
        /// \param valueView
        /// \param assetsPackage
        static void deserialize(DeserializableValueView<ITexture2D, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
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

        static ITexture2D* allocateObject(DeserializableValueView<ITexture2D, TFormatType>& valueView) noexcept
        {
            return CoreMain::getRenderer()->createTexture2D();
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ICubemapTexture, TFormatType> : BaseTypes<ITexture2D>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ICubemapTexture";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_parts", valueView.m_data->m_parts, assetsPackage);
        }

        static void deserialize(DeserializableValueView<ICubemapTexture, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto parts = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_parts)>("m_parts", assetsPackage);
            if(parts)
            {
                valueView.m_data->m_parts = std::move(*parts);
            }
        }

        static ICubemapTexture* allocateObject(DeserializableValueView<ICubemapTexture, TFormatType>& valueView) noexcept
        {
            return CoreMain::getRenderer()->createCubemapTexture();
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<TextFileAsset, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::TextFileAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            AssetsPackage::DataMarkup textureDataMarkup = assetsPackage.addData(valueView.m_data->m_data);

            valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
        }

        static void deserialize(DeserializableValueView<TextFileAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
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
    struct SerdeSpec<SGSLESubShader, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SGSLESubShader";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_type", valueView.m_data->m_type);

            AssetsPackage::DataMarkup textureDataMarkup = assetsPackage.addData(valueView.m_data->m_code);

            valueView.getValueContainer().addMember("m_dataOffset", textureDataMarkup.m_offset);
            valueView.getValueContainer().addMember("m_dataSizeInBytes", textureDataMarkup.m_sizeInBytes);
        }

        static void deserialize(DeserializableValueView<SGSLESubShader, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            const auto typeOpt = valueView.getValueContainer().template getMember<SGSLESubShaderType>("m_type");
            auto dataOffsetOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataOffset");
            auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>("m_dataSizeInBytes");

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

        static void serialize(SerializableValueView<const ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_subPassName", valueView.m_data->m_subPassName);
            valueView.getValueContainer().addMember("m_attributes", valueView.m_data->m_attributes);
            valueView.getValueContainer().addMember("m_subShaders", valueView.m_data->m_subShaders, assetsPackage);
        }

        static void deserialize(DeserializableValueView<ShaderAnalyzedFile, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto m_subPassName = valueView.getValueContainer().template getMember<std::string>("m_subPassName");
            if(m_subPassName)
            {
                valueView.m_data->m_subPassName = std::move(*m_subPassName);
            }

            auto m_attributes = valueView.getValueContainer().template getMember<decltype(ShaderAnalyzedFile::m_attributes)>("m_attributes");
            if(m_attributes)
            {
                valueView.m_data->m_attributes = std::move(*m_attributes);
            }

            auto m_subShaders = valueView.getValueContainer().template getMember<decltype(ShaderAnalyzedFile::m_subShaders)>("m_subShaders", assetsPackage);
            if(m_subShaders)
            {
                valueView.m_data->m_subShaders = std::move(*m_subShaders);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<IMaterial, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IMaterial";
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const IMaterial, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_transparencyType", valueView.m_data->m_transparencyType);
            valueView.getValueContainer().addMember("m_shader", valueView.m_data->m_shader, std::forward<SharedDataT>(sharedData)...);
            valueView.getValueContainer().addMember("m_meshRenderState", valueView.m_data->m_meshRenderState);

            valueView.getValueContainer().addMember("m_textures", valueView.m_data->m_textures, std::forward<SharedDataT>(sharedData)...);

            valueView.getValueContainer().addMember("m_diffuseColor", valueView.m_data->m_diffuseColor);
            valueView.getValueContainer().addMember("m_specularColor", valueView.m_data->m_specularColor);
            valueView.getValueContainer().addMember("m_ambientColor", valueView.m_data->m_ambientColor);
            valueView.getValueContainer().addMember("m_emissionColor", valueView.m_data->m_emissionColor);
            valueView.getValueContainer().addMember("m_transparentColor", valueView.m_data->m_transparentColor);
            valueView.getValueContainer().addMember("m_shininess", valueView.m_data->m_shininess);
            valueView.getValueContainer().addMember("m_metallicFactor", valueView.m_data->m_metallicFactor);
            valueView.getValueContainer().addMember("m_roughnessFactor", valueView.m_data->m_roughnessFactor);
        }

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<IMaterial, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(name)
            {
                valueView.m_data->m_name = std::move(*name);
            }

            auto transparencyType = valueView.getValueContainer().template getMember<MaterialTransparencyType>("m_transparencyType");
            if(transparencyType)
            {
                valueView.m_data->m_transparencyType = std::move(*transparencyType);
            }

            auto shader = valueView.getValueContainer().template getMember<AssetRef<IShader>>("m_shader", std::forward<SharedDataT>(sharedData)...);
            if(shader)
            {
                valueView.m_data->m_shader = std::move(*shader);
            }

            const auto meshRenderState = valueView.getValueContainer().template getMember<MeshRenderState>("m_meshRenderState");
            if(meshRenderState)
            {
                valueView.m_data->m_meshRenderState = *meshRenderState;
            }

            auto textures = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_textures)>("m_textures", std::forward<SharedDataT>(sharedData)...);
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
    struct SerdeSpec<IMeshData, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::IMeshData";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IMeshData, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_aabb", valueView.m_data->m_aabb);
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_indices);

                valueView.getValueContainer().addMember("m_indicesOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_indicesSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_vertices);

                valueView.getValueContainer().addMember("m_verticesOffset", dataMarkup.m_offset);
                valueView.getValueContainer().addMember("m_verticesSizeInBytes", dataMarkup.m_sizeInBytes);
            }

            // TODO: VERTEX COLORS SERIALIZING IN PACKAGE

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
                        "m_verticesOffset"
                );
                auto dataSizeInBytesOpt = valueView.getValueContainer().template getMember<std::streamsize>(
                        "m_verticesSizeInBytes"
                );

                if(dataOffsetOpt)
                {
                    valueView.m_data->m_verticesOffsetInPackage = *dataOffsetOpt;
                }

                if(dataSizeInBytesOpt)
                {
                    valueView.m_data->m_verticesSizeInPackage = *dataSizeInBytesOpt;
                }
            }

            // TODO: VERTEX COLORS DESERIALIZING FROM PACKAGE

            auto generatePhysicalMesh = valueView.getValueContainer().template getMember<bool>("m_generatePhysicalMesh");
            if(generatePhysicalMesh)
            {
                valueView.m_data->generatePhysicalMesh();
            }

            auto material = valueView.getValueContainer().template getMember<AssetRef<IMaterial>>("m_material");
            if(material)
            {
                valueView.m_data->m_material = std::move(*material);
            }
        }

        static IMeshData* allocateObject(DeserializableValueView<IMeshData, TFormatType>& valueView) noexcept
        {
            return CoreMain::getRenderer()->createMeshData();
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Node, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Node";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Node, TFormatType>& valueView, AssetsPackage& assetsPackage)
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

            auto meshesData = valueView.getValueContainer().template getMember<std::vector<AssetRef<IMeshData>>>("m_meshesData", assetsPackage);
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

        static void serialize(SerializableValueView<const ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_importerFlags", valueView.m_data->m_importerFlags);
            valueView.getValueContainer().addMember("m_modelName", valueView.m_data->m_modelName);
            valueView.getValueContainer().addMember("m_rootNode", valueView.m_data->m_rootNode, assetsPackage);
        }

        static void deserialize(DeserializableValueView<ModelAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
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

            auto rootNode = valueView.getValueContainer().template getMember<Ref<Node>>("m_rootNode", assetsPackage);
            if(rootNode)
            {
                valueView.m_data->m_rootNode = std::move(*rootNode);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Skeleton, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Skeleton";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Skeleton, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_rootBone", valueView.m_data->m_rootBone, assetsPackage);
            valueView.getValueContainer().addMember("m_allBones", valueView.m_data->m_allBones, assetsPackage);
        }

        static void deserialize(DeserializableValueView<Skeleton, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto rootBone = valueView.getValueContainer().template getMember<AssetRef<Bone>>("m_rootBone", assetsPackage);
            if(rootBone)
            {
                valueView.m_data->m_rootBone = std::move(*rootBone);
            }

            auto allBones = valueView.getValueContainer().template getMember<decltype(Skeleton::m_allBones)>("m_allBones", assetsPackage);
            if(allBones)
            {
                valueView.m_data->m_allBones = std::move(*allBones);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Bone, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Bone";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Bone, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_id", valueView.m_data->m_id);
            valueView.getValueContainer().addMember("m_boneName", valueView.m_data->m_boneName);
            valueView.getValueContainer().addMember("m_offsetMatrix", valueView.m_data->m_offsetMatrix);
            valueView.getValueContainer().addMember("m_affectedMeshesBoneData", valueView.m_data->m_affectedMeshesBoneData, assetsPackage);
            valueView.getValueContainer().addMember("m_children", valueView.m_data->m_children, assetsPackage);
        }

        static void deserialize(DeserializableValueView<Bone, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto id = valueView.getValueContainer().template getMember<decltype(Bone::m_id)>("m_id");
            if(id)
            {
                valueView.m_data->m_id = std::move(*id);
            }

            auto boneName = valueView.getValueContainer().template getMember<std::string>("m_boneName");
            if(boneName)
            {
                valueView.m_data->m_boneName = std::move(*boneName);
            }

            auto offsetMatrix = valueView.getValueContainer().template getMember<glm::mat4>("m_offsetMatrix");
            if(offsetMatrix)
            {
                valueView.m_data->m_offsetMatrix = std::move(*offsetMatrix);
            }

            auto affectedMeshesBoneData = valueView.getValueContainer().template getMember<decltype(Bone::m_affectedMeshesBoneData)>("m_affectedMeshesBoneData", assetsPackage);
            if(affectedMeshesBoneData)
            {
                valueView.m_data->m_affectedMeshesBoneData = std::move(*affectedMeshesBoneData);
            }

            auto children = valueView.getValueContainer().template getMember<decltype(Bone::m_children)>("m_children", assetsPackage);
            if(children)
            {
                valueView.m_data->m_children = std::move(*children);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<MeshBoneData, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MeshBoneData";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MeshBoneData, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_affectedMesh", valueView.m_data->m_affectedMesh, assetsPackage);

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_weights);

                valueView.getValueContainer().addMember("m_weightsDataMarkupInPackage", dataMarkup);
            }
        }

        static void deserialize(DeserializableValueView<MeshBoneData, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto affectedMesh = valueView.getValueContainer().template getMember<AssetRef<IMeshData>>("m_affectedMesh", assetsPackage);
            if(affectedMesh)
            {
                valueView.m_data->m_affectedMesh = std::move(*affectedMesh);
            }

            {
                auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                        "m_weightsDataMarkupInPackage"
                );

                if(dataMarkupOpt)
                {
                    valueView.m_data->m_weightsDataMarkupInPackage = *dataMarkupOpt;
                }
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<AnimationsFile, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AnimationsFile";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AnimationsFile, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_skeletalAnimations", valueView.m_data->m_skeletalAnimations, assetsPackage);
        }

        static void deserialize(DeserializableValueView<AnimationsFile, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto skeletalAnimations = valueView.getValueContainer().template getMember<decltype(AnimationsFile::m_skeletalAnimations)>("m_skeletalAnimations", assetsPackage);
            if(skeletalAnimations)
            {
                valueView.m_data->m_skeletalAnimations = std::move(*skeletalAnimations);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SkeletalAnimationAsset, TFormatType> : BaseTypes<IAsset>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SkeletalAnimationAsset";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SkeletalAnimationAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_animationName", valueView.m_data->m_animationName);
            valueView.getValueContainer().addMember("m_duration", valueView.m_data->m_duration);
            valueView.getValueContainer().addMember("m_ticksPerSecond", valueView.m_data->m_ticksPerSecond);
            valueView.getValueContainer().addMember("m_bonesAnimations", valueView.m_data->m_bonesAnimations, assetsPackage);
        }

        static void deserialize(DeserializableValueView<SkeletalAnimationAsset, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto animationName = valueView.getValueContainer().template getMember<std::string>("m_animationName");
            if(animationName)
            {
                valueView.m_data->m_animationName = std::move(*animationName);
            }

            auto duration = valueView.getValueContainer().template getMember<float>("m_duration");
            if(duration)
            {
                valueView.m_data->m_duration = std::move(*duration);
            }

            auto ticksPerSecond = valueView.getValueContainer().template getMember<float>("m_ticksPerSecond");
            if(ticksPerSecond)
            {
                valueView.m_data->m_ticksPerSecond = std::move(*ticksPerSecond);
            }

            auto bonesAnimations = valueView.getValueContainer().template getMember<decltype(SkeletalAnimationAsset::m_bonesAnimations)>("m_bonesAnimations", assetsPackage);
            if(bonesAnimations)
            {
                valueView.m_data->m_bonesAnimations = std::move(*bonesAnimations);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SkeletalBoneAnimation, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SkeletalBoneAnimation";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SkeletalBoneAnimation, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_boneName", valueView.m_data->m_boneName);

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_positionKeys);

                valueView.getValueContainer().addMember("m_positionKeysMarkupInPackage", dataMarkup);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_rotationKeys);

                valueView.getValueContainer().addMember("m_rotationKeysMarkupInPackage", dataMarkup);
            }

            {
                AssetsPackage::DataMarkup dataMarkup = assetsPackage.addData(valueView.m_data->m_scaleKeys);

                valueView.getValueContainer().addMember("m_scaleKeysMarkupInPackage", dataMarkup);
            }
        }

        static void deserialize(DeserializableValueView<SkeletalBoneAnimation, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto boneName = valueView.getValueContainer().template getMember<std::string>("m_boneName");
            if(boneName)
            {
                valueView.m_data->m_boneName = std::move(*boneName);
            }

            {
                auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                        "m_positionKeysMarkupInPackage"
                );

                if(dataMarkupOpt)
                {
                    valueView.m_data->m_positionKeysMarkupInPackage = *dataMarkupOpt;
                }
            }

            {
                auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                        "m_rotationKeysMarkupInPackage"
                );

                if(dataMarkupOpt)
                {
                    valueView.m_data->m_rotationKeysMarkupInPackage = *dataMarkupOpt;
                }
            }

            {
                auto dataMarkupOpt = valueView.getValueContainer().template getMember<AssetsPackage::DataMarkup>(
                        "m_scaleKeysMarkupInPackage"
                );

                if(dataMarkupOpt)
                {
                    valueView.m_data->m_scaleKeysMarkupInPackage = *dataMarkupOpt;
                }
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlanner, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlanner";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlanner, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_rootNodes", valueView.m_data->m_rootNodes);
            valueView.getValueContainer().addMember("m_skeleton", valueView.m_data->m_skeleton);
            valueView.getValueContainer().addMember("m_maxBonesPerMesh", valueView.m_data->m_maxBonesPerMesh);
        }

        static void deserialize(DeserializableValueView<MotionPlanner, TFormatType>& valueView)
        {
            auto rootNodes = valueView.getValueContainer().template getMember<decltype(MotionPlanner::m_rootNodes)>("m_rootNodes");
            if(rootNodes)
            {
                valueView.m_data->m_rootNodes = std::move(*rootNodes);
            }

            auto skeleton = valueView.getValueContainer().template getMember<decltype(MotionPlanner::m_skeleton)>("m_skeleton");
            if(skeleton)
            {
                valueView.m_data->m_skeleton = std::move(*skeleton);
            }

            auto maxBonesPerMesh = valueView.getValueContainer().template getMember<std::int32_t>("m_maxBonesPerMesh");
            if(maxBonesPerMesh)
            {
                valueView.m_data->m_maxBonesPerMesh = std::move(*maxBonesPerMesh);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlannerNode, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlannerNode";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlannerNode, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_connections", valueView.m_data->m_connections);
            valueView.getValueContainer().addMember("m_skeletalAnimation", valueView.m_data->m_skeletalAnimation);
            valueView.getValueContainer().addMember("m_isActive", valueView.m_data->m_isActive);
            // valueView.getValueContainer().addMember("m_isPaused", valueView.m_data->m_isPaused);
            // valueView.getValueContainer().addMember("m_isPlaying", valueView.m_data->m_isPlaying);
            valueView.getValueContainer().addMember("m_isRepeated", valueView.m_data->m_isRepeated);
            valueView.getValueContainer().addMember("m_animationSpeed", valueView.m_data->m_animationSpeed);
        }

        static void deserialize(DeserializableValueView<MotionPlannerNode, TFormatType>& valueView)
        {
            auto connections = valueView.getValueContainer().template getMember<decltype(MotionPlannerNode::m_connections)>("m_connections");
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

            auto skeletalAnimation = valueView.getValueContainer().template getMember<decltype(MotionPlannerNode::m_skeletalAnimation)>("m_skeletalAnimation");
            if(skeletalAnimation)
            {
                valueView.m_data->m_skeletalAnimation = std::move(*skeletalAnimation);
            }

            auto isActive = valueView.getValueContainer().template getMember<bool>("m_isActive");
            if(isActive)
            {
                valueView.m_data->m_isActive = std::move(*isActive);
            }

            auto isRepeated = valueView.getValueContainer().template getMember<bool>("m_isRepeated");
            if(isRepeated)
            {
                valueView.m_data->m_isRepeated = std::move(*isRepeated);
            }

            auto animationSpeed = valueView.getValueContainer().template getMember<float>("m_animationSpeed");
            if(animationSpeed)
            {
                valueView.m_data->m_animationSpeed = std::move(*animationSpeed);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlannerConnection, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlannerConnection";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlannerConnection, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_blendTime", valueView.m_data->m_blendTime);
            valueView.getValueContainer().addMember("m_blendSpeed", valueView.m_data->m_blendSpeed);
            valueView.getValueContainer().addMember("m_activationAction", valueView.m_data->m_activationAction);
            // serializing only next node because m_previousNode owns this connection
            valueView.getValueContainer().addMember("m_nextNode", valueView.m_data->m_nextNode);
        }

        static void deserialize(DeserializableValueView<MotionPlannerConnection, TFormatType>& valueView)
        {
            auto blendTime = valueView.getValueContainer().template getMember<float>("m_blendTime");
            if(blendTime)
            {
                valueView.m_data->m_blendTime = std::move(*blendTime);
            }

            auto blendSpeed = valueView.getValueContainer().template getMember<float>("m_blendSpeed");
            if(blendSpeed)
            {
                valueView.m_data->m_blendSpeed = std::move(*blendSpeed);
            }

            auto activationAction = valueView.getValueContainer().template getMember<decltype(MotionPlannerConnection::m_activationAction)>("m_activationAction");
            if(activationAction)
            {
                valueView.m_data->m_activationAction = std::move(*activationAction);
            }

            auto nextNode = valueView.getValueContainer().template getMember<decltype(MotionPlannerConnection::m_nextNode)>("m_nextNode");
            if(nextNode)
            {
                valueView.m_data->m_nextNode = std::move(*nextNode);
            }
        }
    };

    template<typename ActionReturnT, typename... ExecuteArgs, FormatType TFormatType>
    struct SerdeSpec<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<
                    AlwaysTrueAction,
                    AlwaysFalseAction,
                    KeyboardKeyAction,
                    KeyboardKeyDownAction,
                    KeyboardKeyPressedAction,
                    KeyboardKeyReleasedAction
                        >
    {
        static inline const std::string type_name = "SGCore::IAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>& valueView)
        {

        }

        static void deserialize(DeserializableValueView<IAction<ActionReturnT(ExecuteArgs...)>, TFormatType>& valueView)
        {

        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<AlwaysTrueAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AlwaysTrueAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AlwaysTrueAction, TFormatType>& valueView)
        {

        }

        static void deserialize(DeserializableValueView<AlwaysTrueAction, TFormatType>& valueView)
        {

        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<AlwaysFalseAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::AlwaysFalseAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const AlwaysFalseAction, TFormatType>& valueView)
        {

        }

        static void deserialize(DeserializableValueView<AlwaysFalseAction, TFormatType>& valueView)
        {

        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyAction, TFormatType> :
            BaseTypes<IAction<KeyboardKeyActionType()>>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::KeyboardKeyAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyAction, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
        }

        static void deserialize(DeserializableValueView<KeyboardKeyAction, TFormatType>& valueView)
        {
            auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
            if(key)
            {
                valueView.m_data->m_key = std::move(*key);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyDownAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::KeyboardKeyDownAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyDownAction, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
        }

        static void deserialize(DeserializableValueView<KeyboardKeyDownAction, TFormatType>& valueView)
        {
            auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
            if(key)
            {
                valueView.m_data->m_key = std::move(*key);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyPressedAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::KeyboardKeyPressedAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyPressedAction, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
        }

        static void deserialize(DeserializableValueView<KeyboardKeyPressedAction, TFormatType>& valueView)
        {
            auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
            if(key)
            {
                valueView.m_data->m_key = std::move(*key);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<KeyboardKeyReleasedAction, TFormatType> :
            BaseTypes<IAction<bool()>>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::KeyboardKeyReleasedAction";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const KeyboardKeyReleasedAction, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_key", valueView.m_data->m_key);
        }

        static void deserialize(DeserializableValueView<KeyboardKeyReleasedAction, TFormatType>& valueView)
        {
            auto key = valueView.getValueContainer().template getMember<KeyboardKey>("m_key");
            if(key)
            {
                valueView.m_data->m_key = std::move(*key);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<MotionPlannersResolver, TFormatType> :
            BaseTypes<ISystem>,
            DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::MotionPlannersResolver";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const MotionPlannersResolver, TFormatType>& valueView)
        {

        }

        static void deserialize(DeserializableValueView<MotionPlannersResolver, TFormatType>& valueView)
        {

        }
    };

    // SERDE IMPL FOR struct 'SGCore::ShaderTextureBinding'
    // =================================================================================
    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderTextureBinding, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderTextureBinding, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_bindingName", valueView.m_data->m_bindingName);
        // todo:
        // valueView.getValueContainer().addMember("m_texture", valueView.m_data->m_texture);
    }

    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderTextureBinding, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<ShaderTextureBinding, TFormatType>& valueView) noexcept
    {
        auto m_bindingName = valueView.getValueContainer().template getMember<std::string>("m_bindingName");

        if(m_bindingName)
        {
            valueView.m_data->m_bindingName = std::move(*m_bindingName);
        }
    }
    // =================================================================================

    // SERDE IMPL FOR struct 'SGCore::ShaderDefine'
    // =================================================================================
    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderDefine, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderDefine, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
        valueView.getValueContainer().addMember("m_expression", valueView.m_data->m_expression);
    }

    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderDefine, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<ShaderDefine, TFormatType>& valueView) noexcept
    {
        auto m_name = valueView.getValueContainer().template getMember<std::string>("m_name");

        if(m_name)
        {
            valueView.m_data->m_name = std::move(*m_name);
        }

        auto m_expression = valueView.getValueContainer().template getMember<std::string>("m_expression");

        if(m_expression)
        {
            valueView.m_data->m_expression = std::move(*m_expression);
        }
    }
    // =================================================================================
}

#endif //SUNGEARENGINE_STANDARDSERDESPECS_H
