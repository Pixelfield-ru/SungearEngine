//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

#include <glm/gtx/matrix_decompose.hpp>

#include "PhysicsDebugDraw.h"
#include "Rigidbody3D.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Threading/WrappedObject.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/TransformUtils.h"

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

    m_dynamicsWorld->getDispatchInfo().m_useContinuous = true;

    m_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_dynamicsWorld->setDebugDrawer(m_debugDraw.get());

    m_dynamicsWorld->setGravity({ 0, -30.0, 0 });

    m_thread->setSleepTime(std::chrono::milliseconds(0));
    m_thread->start();
}

void SGCore::PhysicsWorld3D::addBody(const SGCore::Ref<btRigidBody>& rigidBody) noexcept
{
    std::lock_guard guard(m_bodiesCountChangeMutex);
    m_dynamicsWorld->addRigidBody(rigidBody.get());
    rigidBody->activate();
}

void SGCore::PhysicsWorld3D::removeBody(const Ref<btRigidBody>& rigidBody) noexcept
{
    std::lock_guard guard(m_bodiesCountChangeMutex);
    int num = rigidBody->getNumConstraintRefs();
    for (int j = 0; j < num; ++j)
    {
        m_dynamicsWorld->removeConstraint(rigidBody->getConstraintRef(0));
    }
    m_dynamicsWorld->removeRigidBody(rigidBody.get());
}

void SGCore::PhysicsWorld3D::parallelUpdate(const double& dt, const double& fixedDt) noexcept
{
    // updateWorld(dt, fixedDt);
}

void SGCore::PhysicsWorld3D::update(const double& dt, const double& fixedDt) noexcept
{
    updateWorld(dt, fixedDt);

    auto lockedScene = getScene();

    if(!lockedScene) return;

    m_dynamicsWorld->debugDrawWorld();
}

void SGCore::PhysicsWorld3D::onAddToScene(const Scene* scene)
{
    if (!scene) return;

    auto rigidbodies3DView = scene->getECSRegistry()->view<Rigidbody3D>();

    rigidbodies3DView.each([this](Ref<Rigidbody3D> rigidbody3D) {
        this->addBody(rigidbody3D->m_body);
    });
}

void SGCore::PhysicsWorld3D::onRemoveFromScene(const Ref<Scene>& scene)
{

}

void SGCore::PhysicsWorld3D::updateWorld(double dt, double fixedDt) noexcept
{
    // if(!m_simulate) return;

    std::lock_guard guard(m_bodiesCountChangeMutex);

    if(m_simulate)
    {
        m_dynamicsWorld->stepSimulation(dt, 12, dt);
    }
}
