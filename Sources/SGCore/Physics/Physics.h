//
// Created by ilya on 18.02.24.
//

#ifndef SUNGEARENGINE_PHYSICS_H
#define SUNGEARENGINE_PHYSICS_H

#include "SGCore/Main/CoreGlobals.h"

#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <LinearMath/btIDebugDraw.h>

namespace SGCore
{
    struct Physics
    {
        static void init() noexcept;
        
        static auto& getCollisionConfig() noexcept
        {
            return m_collisionConfig;
        }
        
        static auto& getCollisionDispatcher() noexcept
        {
            return m_collisionDispatcher;
        }
        
        static auto& getOverlappingPairCache() noexcept
        {
            return m_overlappingPairCache;
        }
        
        static auto& getSequentialImpulseConstraintSolver() noexcept
        {
            return m_sequentialImpulseConstraintSolver;
        }
        
        static auto& getDiscreteDynamicsWorld() noexcept
        {
            return m_discreteDynamicsWorld;
        }
        
        static auto& getDebugDraw() noexcept
        {
            return m_debugDraw;
        }
    
    private:
        static inline Scope<btCollisionConfiguration> m_collisionConfig;
        static inline Scope<btCollisionDispatcher> m_collisionDispatcher;
        static inline Scope<btBroadphaseInterface> m_overlappingPairCache;
        static inline Scope<btSequentialImpulseConstraintSolver> m_sequentialImpulseConstraintSolver;
        static inline Scope<btDiscreteDynamicsWorld> m_discreteDynamicsWorld;
        static inline Scope<btIDebugDraw> m_debugDraw;
    };
}

#endif // SUNGEARENGINE_PHYSICS_H
