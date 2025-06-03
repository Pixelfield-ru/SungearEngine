//
// Created by ilya on 19.02.24.
//

#ifndef SUNGEARENGINE_RIGIDBODY3D_H
#define SUNGEARENGINE_RIGIDBODY3D_H

#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Marker.h"
#include "SGCore/ECS/Component.h"
#include "Common.h"
#include "SGCore/Serde/Defines.h"

sg_predeclare_serde()

namespace SGCore
{
    struct PhysicsWorld3D;

    struct Rigidbody3D : ECS::Component<Ref<Rigidbody3D>, Ref<const Rigidbody3D>>
    {
        sg_serde_as_friend()

    public:
        Ref<btRigidBody> m_body;
        
    public:
        Rigidbody3D(const Ref<PhysicsWorld3D>& physicsWorld);
        Rigidbody3D();
        Rigidbody3D(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D(Rigidbody3D&& other) noexcept = default;
        
        ~Rigidbody3D();
        
        Ref<btMotionState> m_state;
        
        [[nodiscard]] Ref<const btCompoundShape> getFinalShape() const noexcept;

        void addShape(const btTransform& shapeTransform, const Ref<btCollisionShape>& shape) noexcept;
        void removeShape(const Ref<btCollisionShape>& shape) noexcept;
        void removeAllShapes() noexcept;
        size_t getShapesCount() const noexcept;
        [[nodiscard]] const std::vector<Ref<btCollisionShape>>& getShapes() const noexcept;

        [[nodiscard]] btTransform& getShapeTransform(size_t index) noexcept;
        [[nodiscard]] const btTransform& getShapeTransform(size_t index) const noexcept;
        void updateShapeTransform(size_t index, const btTransform& newTransform, bool recalculateLocalAABB = true) noexcept;
        void updateShapeTransform(size_t index, bool recalculateLocalAABB = true) noexcept;

        void setParentWorld(const Ref<PhysicsWorld3D>& world) noexcept;
        [[nodiscard]] Weak<PhysicsWorld3D> getParentPhysicsWorld() const noexcept;

        void setType(PhysicalObjectType type) noexcept;
        PhysicalObjectType getType() const noexcept;

        void reAddToWorld() const noexcept;
        void removeFromWorld() const noexcept;

        void stop() const noexcept;
        
        Rigidbody3D& operator=(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D& operator=(Rigidbody3D&& other) noexcept = default;
        
    private:
        std::vector<Ref<btCollisionShape>> m_shapes;
        Ref<btCompoundShape> m_finalShape;
        Weak<PhysicsWorld3D> m_parentPhysicsWorld;

        PhysicalObjectType m_type = PhysicalObjectType::OT_STATIC;
    };
}

#endif //SUNGEARENGINE_RIGIDBODY3D_H
