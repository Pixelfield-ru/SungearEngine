//
// Created by stuka on 28.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace SGCore
{
    struct Rigidbody3D;
}

namespace SGCore::Serde
{
    // ======================================================== btBoxShape FWD

    template<FormatType TFormatType>
    struct SerdeSpec<btBoxShape, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        sg_serde_define_type_name("btBoxShape")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const btBoxShape, TFormatType>& valueView,
                              const btTransform& shapeTransform,
                              const Rigidbody3D& parentRigidbody3D) noexcept;

        static void deserialize(DeserializableValueView<btBoxShape, TFormatType>& valueView,
                                btTransform& shapeTransform,
                                Rigidbody3D& parentRigidbody3D) noexcept;
    };

    // ======================================================== btCompoundShape FWD

    template<FormatType TFormatType>
    struct SerdeSpec<btCompoundShape, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        sg_serde_define_type_name("btCompoundShape")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const btCompoundShape, TFormatType>& valueView,
                              const btTransform& shapeTransform,
                              const Rigidbody3D& parentRigidbody3D) noexcept;

        static void deserialize(DeserializableValueView<btCompoundShape, TFormatType>& valueView,
                                btTransform& shapeTransform,
                                Rigidbody3D& parentRigidbody3D) noexcept;
    };

    // ======================================================== btCollisionShape FWD

    template<FormatType TFormatType>
    struct SerdeSpec<btCollisionShape, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        sg_serde_define_type_name("btCollisionShape")
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const btCollisionShape, TFormatType>& valueView, const Rigidbody3D& parentRigidbody3D) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<btCollisionShape, TFormatType>& valueView, Rigidbody3D& parentRigidbody3D) noexcept;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const btCollisionShape, TFormatType>& valueView, const btTransform& shapeTransform, const Rigidbody3D& parentRigidbody3D) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<btCollisionShape, TFormatType>& valueView, btTransform& shapeTransform, Rigidbody3D& parentRigidbody3D) noexcept;

        static btCollisionShape* allocateObject(DeserializableValueView<btCollisionShape, TFormatType>& valueView) noexcept;
    };

    // ======================================================== btTransform FWD

    template<FormatType TFormatType>
    struct SerdeSpec<btTransform, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        sg_serde_define_type_name("btTransform")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const btTransform, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<btTransform, TFormatType>& valueView) noexcept;
    };

    // ======================================================== btVector3 FWD

    template<FormatType TFormatType>
    struct SerdeSpec<btVector3, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        sg_serde_define_type_name("btVector3")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const btVector3, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<btVector3, TFormatType>& valueView) noexcept;
    };

    // ======================================================== btQuaternion FWD

    template<FormatType TFormatType>
    struct SerdeSpec<btQuaternion, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        sg_serde_define_type_name("btQuaternion")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const btQuaternion, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<btQuaternion, TFormatType>& valueView) noexcept;
    };
}
