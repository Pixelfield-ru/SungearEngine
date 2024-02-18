//
// Created by ilya on 19.02.24.
//

#ifndef SUNGEARENGINE_RIGIDBODY3D_H
#define SUNGEARENGINE_RIGIDBODY3D_H

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class PhysicsWorld;
    
    struct Rigidbody3D
    {
        Rigidbody3D(const Ref<PhysicsWorld>& physicsWorld);
        ~Rigidbody3D();
        
        btMotionState* m_motionState = nullptr;
        btRigidBody* m_rigidBody = nullptr;
        
    private:
        Weak<PhysicsWorld> m_parentPhysicsWorld;
    };
}

#endif //SUNGEARENGINE_RIGIDBODY3D_H
