//
// Created by ilya on 18.02.24.
//

#ifndef SUNGEARENGINE_PHYSICSWORLD_H
#define SUNGEARENGINE_PHYSICSWORLD_H

#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/BroadphaseCollision/btDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <LinearMath/btIDebugDraw.h>

#include "SGCore/Scene/Scene.h"
#include "SGCore/Main/CoreGlobals.h"
#include "PhysicsDebugDraw.h"

namespace SGCore
{
    struct PhysicsWorld : public ISystem
    {
        PhysicsWorld();
        ~PhysicsWorld();
        
        void update() noexcept override;
        
        auto* getCollisionConfig() noexcept
        {
            return m_collisionConfig;
        }
        
        auto* getCollisionDispatcher() noexcept
        {
            return m_collisionDispatcher;
        }
        
        auto* getOverlappingPairCache() noexcept
        {
            return m_overlappingPairCache;
        }
        
        auto* getSequentialImpulseConstraintSolver() noexcept
        {
            return m_sequentialImpulseConstraintSolver;
        }
        
        auto* getDiscreteDynamicsWorld() noexcept
        {
            return m_discreteDynamicsWorld;
        }
        
        auto& getDebugDraw() noexcept
        {
            return m_debugDraw;
        }
    
    private:
        btCollisionConfiguration* m_collisionConfig = nullptr;
        btCollisionDispatcher* m_collisionDispatcher = nullptr;
        btBroadphaseInterface* m_overlappingPairCache = nullptr;
        btSequentialImpulseConstraintSolver* m_sequentialImpulseConstraintSolver = nullptr;
        btDiscreteDynamicsWorld* m_discreteDynamicsWorld = nullptr;
        PhysicsDebugDraw* m_debugDraw = nullptr;
    };
}

#endif // SUNGEARENGINE_PHYSICSWORLD_H
