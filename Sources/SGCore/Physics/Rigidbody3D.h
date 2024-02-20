//
// Created by ilya on 19.02.24.
//

#ifndef SUNGEARENGINE_RIGIDBODY3D_H
#define SUNGEARENGINE_RIGIDBODY3D_H

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct PhysicsWorld3D;

    struct Rigidbody3D
    {
        Rigidbody3D(const Ref<PhysicsWorld3D>& physicsWorld);
        Rigidbody3D(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D(Rigidbody3D&& other) noexcept = default;
        
        ~Rigidbody3D();
        
        Ref<btMotionState> m_state;
        Ref<btSphereShape> m_shape;
        Ref<btRigidBody> m_body;
        
        Rigidbody3D& operator=(const Rigidbody3D& other) noexcept = default;
        Rigidbody3D& operator=(Rigidbody3D&& other) noexcept = default;
        
        Weak<PhysicsWorld3D> getParentPhysicsWorld() const noexcept;
        
    private:
        Weak<PhysicsWorld3D> m_parentPhysicsWorld;
    };
}

#endif //SUNGEARENGINE_RIGIDBODY3D_H
