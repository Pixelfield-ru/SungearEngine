//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld.h"

#include "PhysicsDebugDraw.h"

SGCore::PhysicsWorld::PhysicsWorld()
{
    m_collisionConfig = new btDefaultCollisionConfiguration;
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfig);
    m_overlappingPairCache = new btDbvtBroadphase;
    m_sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;
    
    m_debugDraw = new PhysicsDebugDraw;
    
    m_discreteDynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher,
                                                          m_overlappingPairCache,
                                                          m_sequentialImpulseConstraintSolver,
                                                          m_collisionConfig);
    
    m_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_discreteDynamicsWorld->setDebugDrawer(m_debugDraw);
    
    m_discreteDynamicsWorld->setGravity({ 0, -10, 0 });
}

SGCore::PhysicsWorld::~PhysicsWorld()
{
    // needs to be deleted in the correct order
    delete m_discreteDynamicsWorld;
    delete m_overlappingPairCache;
    delete m_collisionConfig;
    delete m_collisionDispatcher;
    delete m_sequentialImpulseConstraintSolver;
}

void SGCore::PhysicsWorld::update() noexcept
{
    auto lockedScene = m_scene.lock();
    if(lockedScene && m_debugDraw->getDebugMode() != btIDebugDraw::DBG_NoDebug)
    {
        m_debugDraw->drawAll(lockedScene);
    }
}

void SGCore::PhysicsWorld::onAddToScene()
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    
}
