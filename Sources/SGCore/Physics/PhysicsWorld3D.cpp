//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

#include "PhysicsDebugDraw.h"
#include "Rigidbody3D.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Threading/SafeObject.h"
#include "glm/gtc/type_ptr.hpp"
#include "SGCore/Transformations/TransformationsUpdater.h"

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
    
    Ref<TimerCallback> callback = MakeRef<TimerCallback>();
    callback->setUpdateFunction([this](const double& dt, const double& fixedDt) {
        worldUpdate(dt, fixedDt);
    });
    
    m_worldUpdateTimer.setTargetFrameRate(60);
    m_worldUpdateTimer.m_cyclic = true;
    m_worldUpdateTimer.addCallback(callback);
    
    /*m_physicsWorldThread = std::thread([this]() {
        while(m_isAlive)
        {
            if(m_active)
            {
                m_worldUpdateTimer.startFrame();
            }
        }
    });*/
    
    // m_physicsWorldThread.detach();
}

SGCore::PhysicsWorld3D::~PhysicsWorld3D()
{
    m_isAlive = false;
    m_physicsWorldThread.join();
}

void SGCore::PhysicsWorld3D::addBody(const SGCore::Ref<btRigidBody>& rigidBody) noexcept
{
    m_bodiesToAdd.lock();
    m_bodiesToAdd.getObject().insert(rigidBody);
    m_bodiesToAdd.unlock();
}

void SGCore::PhysicsWorld3D::removeBody(const Ref<btRigidBody>& rigidBody) noexcept
{
    m_bodiesToRemove.lock();
    m_bodiesToRemove.getObject().insert(rigidBody);
    m_bodiesToRemove.unlock();
}

void SGCore::PhysicsWorld3D::worldUpdate(const double& dt, const double& fixedDt) noexcept
{
    if(!m_bodiesToRemove.isLocked())
    {
        auto bodiesToRemoveIt = m_bodiesToRemove.getObject().begin();
        while(bodiesToRemoveIt != m_bodiesToRemove.getObject().end())
        {
            const auto& bodyRef = (*bodiesToRemoveIt);
            
            int num = bodyRef->getNumConstraintRefs();
            for(int j = 0; j < num; ++j)
            {
                m_dynamicsWorld->removeConstraint(bodyRef->getConstraintRef(0));
            }
            m_dynamicsWorld->removeRigidBody(bodyRef.get());
            
            bodiesToRemoveIt = m_bodiesToRemove.getObject().erase(bodiesToRemoveIt);
        }
    }
    
    if(!m_bodiesToAdd.isLocked())
    {
        auto bodiesToAddIt = m_bodiesToAdd.getObject().begin();
        while(bodiesToAddIt != m_bodiesToAdd.getObject().end())
        {
            m_dynamicsWorld->addRigidBody(bodiesToAddIt->get());
            bodiesToAddIt = m_bodiesToAdd.getObject().erase(bodiesToAddIt);
        }
    }
    
    auto lockedScene = m_scene.lock();
    
    Ref<TransformationsUpdater> transformationsUpdater = lockedScene->getSystem<TransformationsUpdater>();
    
    /*if(transformationsUpdater->m_changedModelMatricesForPhysics.isLocked())
    {
        for(EntityComponentMember<glm::mat4>& matrix : transformationsUpdater->m_changedModelMatricesForPhysics.getObject())
        {
            if(lockedScene->getECSRegistry().valid(matrix.m_owner))
            {
                Rigidbody3D* rigidbody3D = lockedScene->getECSRegistry().try_get<Rigidbody3D>(matrix.m_owner);
                if(rigidbody3D)
                {
                    btTransform initialTransform;
                    initialTransform.setIdentity();
                    initialTransform.setFromOpenGLMatrix(glm::value_ptr(matrix.m_memberValue));
                    rigidbody3D->m_body->setWorldTransform(initialTransform);
                    
                    // rigidbody3D->m_body->getCollisionShape()->setLocalScaling({ scale.x, scale.y, scale.z });
                }
            }
        }
        
        transformationsUpdater->m_changedModelMatricesForPhysics.getObject().clear();
        transformationsUpdater->m_changedModelMatricesForPhysics.unlock();
    }
    */
    m_dynamicsWorld->stepSimulation(dt, 12, dt);
    
    /*if(!lockedScene) return;
    
    if(!m_physicalMatricesVector.isLocked())
    {
        auto rigidbodiesView = lockedScene->getECSRegistry().view<Rigidbody3D>();
        
        rigidbodiesView.each([this](const entt::entity& entity, const Rigidbody3D& rigidbody3D) {
            float rigidbody3DMatrix[16];
            rigidbody3D.m_body->getWorldTransform().getOpenGLMatrix(rigidbody3DMatrix);
            
            glm::mat4 glmRigidbody3DOwnModelMatrix = glm::make_mat4(rigidbody3DMatrix);
            
            m_physicalMatricesVector.getObject().emplace_back(entity, glmRigidbody3DOwnModelMatrix);
        });
        
        m_physicalMatricesVector.lock();
    }*/
}

void SGCore::PhysicsWorld3D::update(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = m_scene.lock();
    if(lockedScene && m_debugDraw->getDebugMode() != btIDebugDraw::DBG_NoDebug)
    {
        if(m_bodiesToAdd.getObject().empty() && m_bodiesToRemove.getObject().empty())
        {
            m_dynamicsWorld->debugDrawWorld();
        }
        m_debugDraw->drawAll(lockedScene);
    }
}

void SGCore::PhysicsWorld3D::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    worldUpdate(dt, fixedDt);
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
