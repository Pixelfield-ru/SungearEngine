//
// Created by stuka on 28.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Physics/Rigidbody3D.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace SGCore::Serde
{
    // ======================================================== impl btBoxShape
    template<FormatType TFormatType>
    void SerdeSpec<btBoxShape, TFormatType>::serialize(
        SerializableValueView<const btBoxShape, TFormatType>& valueView,
        const btTransform& shapeTransform,
        const Rigidbody3D& parentRigidbody3D) noexcept
    {
    }

    template<FormatType TFormatType>
    void SerdeSpec<btBoxShape, TFormatType>::deserialize(
        DeserializableValueView<btBoxShape, TFormatType>& valueView,
        btTransform& shapeTransform,
        Rigidbody3D& parentRigidbody3D) noexcept
    {
    }

    // ======================================================== impl btCompoundShape

    template<FormatType TFormatType>
    void SerdeSpec<btCompoundShape, TFormatType>::serialize(
        SerializableValueView<const btCompoundShape, TFormatType>& valueView,
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

    template<FormatType TFormatType>
    void SerdeSpec<btCompoundShape, TFormatType>::deserialize(
        DeserializableValueView<btCompoundShape, TFormatType>& valueView,
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

    // ======================================================== impl btCollisionShape

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<btCollisionShape, TFormatType>::serialize(
        SerializableValueView<const btCollisionShape, TFormatType>& valueView,
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

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<btCollisionShape, TFormatType>::deserialize(
        DeserializableValueView<btCollisionShape, TFormatType>& valueView,
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

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<btCollisionShape, TFormatType>::serialize(
        SerializableValueView<const btCollisionShape, TFormatType>& valueView,
        const btTransform& shapeTransform,
        const Rigidbody3D& parentRigidbody3D) noexcept
    {
        serialize(valueView, parentRigidbody3D);

        valueView.getValueContainer().addMember("m_transform", shapeTransform);
    }

    template<FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<btCollisionShape, TFormatType>::deserialize(
        DeserializableValueView<btCollisionShape, TFormatType>& valueView,
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

    template<FormatType TFormatType>
    btCollisionShape* SerdeSpec<btCollisionShape, TFormatType>::allocateObject(DeserializableValueView<btCollisionShape, TFormatType>& valueView) noexcept
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

    // ======================================================== impl btTransform

    template<FormatType TFormatType>
    void SerdeSpec<btTransform, TFormatType>::serialize(SerializableValueView<const btTransform, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_origin", valueView.m_data->getOrigin());
        valueView.getValueContainer().addMember("m_rotation", valueView.m_data->getRotation());
    }

    template<FormatType TFormatType>
    void SerdeSpec<btTransform, TFormatType>::deserialize(DeserializableValueView<btTransform, TFormatType>& valueView) noexcept
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

    // ======================================================== impl btVector3

    template<FormatType TFormatType>
    void SerdeSpec<btVector3, TFormatType>::serialize(SerializableValueView<const btVector3, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("x", valueView.m_data->getX());
        valueView.getValueContainer().addMember("y", valueView.m_data->getY());
        valueView.getValueContainer().addMember("z", valueView.m_data->getZ());
    }

    template<FormatType TFormatType>
    void SerdeSpec<btVector3, TFormatType>::deserialize(DeserializableValueView<btVector3, TFormatType>& valueView) noexcept
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

    // ======================================================== impl btQuaternion

    template<FormatType TFormatType>
    void SerdeSpec<btQuaternion, TFormatType>::serialize(SerializableValueView<const btQuaternion, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("x", valueView.m_data->getX());
        valueView.getValueContainer().addMember("y", valueView.m_data->getY());
        valueView.getValueContainer().addMember("z", valueView.m_data->getZ());
        valueView.getValueContainer().addMember("w", valueView.m_data->getW());
    }

    template<FormatType TFormatType>
    void SerdeSpec<btQuaternion, TFormatType>::deserialize(DeserializableValueView<btQuaternion, TFormatType>& valueView) noexcept
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
}
