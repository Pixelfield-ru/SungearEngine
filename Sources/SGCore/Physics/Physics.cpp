//
// Created by ilya on 18.02.24.
//
#include "Physics.h"
#include "PhysicsDebugDraw.h"

void SGCore::Physics::init() noexcept
{
    m_collisionConfig = MakeScope<btDefaultCollisionConfiguration>();
    m_collisionDispatcher = MakeScope<btCollisionDispatcher>(m_collisionConfig.get());
    m_overlappingPairCache = MakeScope<btDbvtBroadphase>();
    m_sequentialImpulseConstraintSolver = MakeScope<btSequentialImpulseConstraintSolver>();
    
    m_debugDraw = Scope<btIDebugDraw>(new PhysicsDebugDraw);
    
    m_discreteDynamicsWorld = MakeScope<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(),
                                                                 m_overlappingPairCache.get(),
                                                                 m_sequentialImpulseConstraintSolver.get(),
                                                                 m_collisionConfig.get());
    
    m_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_discreteDynamicsWorld->setDebugDrawer(m_debugDraw.get());
    
    m_discreteDynamicsWorld->setGravity({ 0, -10, 0 });
}

