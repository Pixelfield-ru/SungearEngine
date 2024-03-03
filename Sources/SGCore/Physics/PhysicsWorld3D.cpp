//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

#include "PhysicsDebugDraw.h"
#include "Rigidbody3D.h"
#include "SGCore/Main/CoreMain.h"

SGCore::PhysicsWorld3D::PhysicsWorld3D()
{
    m_collisionConfig = MakeScope<btDefaultCollisionConfiguration>();
    m_collisionDispatcher = MakeScope<btCollisionDispatcher>(m_collisionConfig.get());
    m_overlappingPairCache = MakeScope<btDbvtBroadphase>();
    m_sequentialImpulseConstraintSolver = MakeScope<btSequentialImpulseConstraintSolver>();
    
    m_debugDraw = MakeScope<PhysicsDebugDraw>();
    
    m_dynamicsWorld = MakeScope<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(),
                                                         m_overlappingPairCache.get(),
                                                         m_sequentialImpulseConstraintSolver.get(),
                                                         m_collisionConfig.get());
    
    m_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_dynamicsWorld->setDebugDrawer(m_debugDraw.get());
    
    m_dynamicsWorld->setGravity({ 0, -120.0, 0 });
}

void SGCore::PhysicsWorld3D::addBody(const SGCore::Ref<btRigidBody>& rigidBody) noexcept
{
    m_bodiesToAdd.insert(rigidBody);
}

void SGCore::PhysicsWorld3D::removeBody(const Ref<btRigidBody>& rigidBody) noexcept
{
    m_bodiesToRemove.insert(rigidBody);
}

void SGCore::PhysicsWorld3D::update(const double& dt) noexcept
{
    auto lockedScene = m_scene.lock();
    if(lockedScene && m_debugDraw->getDebugMode() != btIDebugDraw::DBG_NoDebug)
    {
        m_dynamicsWorld->debugDrawWorld();
        m_debugDraw->drawAll(lockedScene);
    }
}

void SGCore::PhysicsWorld3D::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    auto bodiesToRemoveIt = m_bodiesToRemove.begin();
    while(bodiesToRemoveIt != m_bodiesToRemove.end())
    {
        const auto& bodyRef = (*bodiesToRemoveIt);
        
        int num = bodyRef->getNumConstraintRefs();
        for(int j = 0; j < num; ++j)
        {
            m_dynamicsWorld->removeConstraint(bodyRef->getConstraintRef(0));
        }
        m_dynamicsWorld->removeRigidBody(bodyRef.get());
        
        bodiesToRemoveIt = m_bodiesToRemove.erase(bodiesToRemoveIt);
    }
    
    auto bodiesToAddIt = m_bodiesToAdd.begin();
    while(bodiesToAddIt != m_bodiesToAdd.end())
    {
        m_dynamicsWorld->addRigidBody(bodiesToAddIt->get());
        bodiesToAddIt = m_bodiesToAdd.erase(bodiesToAddIt);
    }
    
    m_dynamicsWorld->stepSimulation(fixedDt, 12, fixedDt);
}

void SGCore::PhysicsWorld3D::onAddToScene()
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    auto rigidbodies3DView = lockedScene->getECSRegistry().view<Rigidbody3D>();
    
    rigidbodies3DView.each([this](Rigidbody3D& rigidbody3D) {
        this->addBody(rigidbody3D.m_body);
    });
}
