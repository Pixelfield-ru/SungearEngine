//
// Created by ilya on 19.02.24.
//

#ifndef SUNGEARENGINE_RIGIDBODY3D_H
#define SUNGEARENGINE_RIGIDBODY3D_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Marker.h"

namespace SGCore
{
    struct PhysicsWorld3D;

    struct Rigidbody3D
    {
    public:
        Ref<btRigidBody> m_body;
        
    private:
        Ref<btCollisionShape> m_shape;
        
    public:
        Rigidbody3D(const Ref<PhysicsWorld3D>& physicsWorld);
        Rigidbody3D(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D(Rigidbody3D&& other) noexcept = default;
        
        ~Rigidbody3D();
        
        Ref<btMotionState> m_state;
        Marker<int> m_bodyFlags;
        
        void setShape(const Ref<btCollisionShape>& shape) noexcept;
        
        [[nodiscard]] Ref<btCollisionShape>& getShape() noexcept;
        
        template<typename ShapeT>
        requires(std::is_base_of_v<btCollisionShape, ShapeT>)
        [[nodiscard]] Ref<ShapeT> getShapeAs() const noexcept
        {
            return std::dynamic_pointer_cast<ShapeT>(m_shape);
        }
        
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
