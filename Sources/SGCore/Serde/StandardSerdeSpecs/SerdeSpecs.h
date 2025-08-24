//
// Created by Ilya on 18.08.2024.
//

#ifndef SUNGEARENGINE_STANDARDSERDESPECS_H
#define SUNGEARENGINE_STANDARDSERDESPECS_H

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include "SGCore/Serde/Serde.h"
#include <glm/glm.hpp>

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
#include "SGCore/Render/Lighting/LightBase.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/UICamera.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"

#include "SGCore/Serde/Components/NonSavable.h"

#include "SGCore/Motion/MotionPlanner.h"
#include "SGCore/Motion/MotionPlannerConnection.h"

#include "SGCore/Main/Config.h"
#include "SGCore/Physics/Rigidbody3D.h"

#include "FWD/ECSSystems.h"
#include "FWD/Actions.h"
#include "FWD/Assets.h"
#include "FWD/STD.h"
#include "FWD/ECSComponents.h"

#include "Impl/ECSSystems.h"
#include "Impl/Actions.h"
#include "Impl/Assets.h"
#include "Impl/STD.h"
#include "Impl/ECSComponents.h"

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
