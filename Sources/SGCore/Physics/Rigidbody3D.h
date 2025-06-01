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

namespace SGCore
{
    struct PhysicsWorld3D;

    struct Rigidbody3D : ECS::Component<Ref<Rigidbody3D>, Ref<const Rigidbody3D>>
    {
    public:
        Ref<btRigidBody> m_body;
        
    private:
        std::vector<Ref<btCollisionShape>> m_shapes;
        Ref<btCompoundShape> m_finalShape;
        
    public:
        Rigidbody3D(const Ref<PhysicsWorld3D>& physicsWorld);
        Rigidbody3D(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D(Rigidbody3D&& other) noexcept = default;
        
        ~Rigidbody3D();
        
        Ref<btMotionState> m_state;
        Marker<int> m_bodyFlags;
        
        [[nodiscard]] Ref<const btCompoundShape> getFinalShape() const noexcept;

        void addShape(const btTransform& shapeTransform, const Ref<btCollisionShape>& shape) noexcept;
        void removeShape(const Ref<btCollisionShape>& shape) noexcept;
        void removeAllShapes() noexcept;
        size_t getShapesCount() const noexcept;
        
        void setParentWorld(const Ref<PhysicsWorld3D>& world) noexcept;
        [[nodiscard]] Weak<PhysicsWorld3D> getParentPhysicsWorld() const noexcept;
        
        void updateFlags() noexcept;
        
        void reAddToWorld() const noexcept;
        void removeFromWorld() const noexcept;
        
        Rigidbody3D& operator=(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D& operator=(Rigidbody3D&& other) noexcept = default;
        
    private:
        Weak<PhysicsWorld3D> m_parentPhysicsWorld;
    };
}

#endif //SUNGEARENGINE_RIGIDBODY3D_H
