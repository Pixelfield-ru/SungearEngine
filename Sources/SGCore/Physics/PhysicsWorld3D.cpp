//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

// for glm/gtx/matrix_decompose.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

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
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto ecsRegistry = lockedScene->getECSRegistry();

    // ============================= set rigidbodies world position

    auto rigidbodiesView = ecsRegistry->view<EntityBaseInfo, Rigidbody3D, Transform>();

    rigidbodiesView.each([](const auto&, const Ref<Rigidbody3D>& rigidbody, const Ref<Transform>& transform) {
        const auto worldPosition = transform->m_finalTransform.m_position;
        const auto worldRotation = transform->m_finalTransform.m_rotation;

        auto& bodyTransform = rigidbody->m_body->getWorldTransform();

        bodyTransform.setOrigin({ worldPosition.x, worldPosition.y, worldPosition.z });
        bodyTransform.setRotation({ worldRotation.x, worldRotation.y, worldRotation.z, worldRotation.w });
    });

    // =============================

    updateWorld(dt, fixedDt);

    // ============================= set transform world position
    rigidbodiesView.each([&ecsRegistry](const EntityBaseInfo& baseInfo, const Ref<Rigidbody3D>& rigidbody, const Ref<Transform>& transform) {
        Transform* parentTransform {};
        if(baseInfo.getParent() != entt::null)
        {
            auto tmpTransform = ecsRegistry->tryGet<Transform>(baseInfo.getParent());
            parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
        }

        Rigidbody3D* parentRigidbody {};
        if(baseInfo.getParent() != entt::null)
        {
            auto tmpRigidbody = ecsRegistry->tryGet<Rigidbody3D>(baseInfo.getParent());
            parentRigidbody = tmpRigidbody ? tmpRigidbody->get() : nullptr;
        }

        const auto& bodyWorldTransform = rigidbody->m_body->getWorldTransform();

        const auto btBodyPosition = bodyWorldTransform.getOrigin();
        const auto btBodyRotation = bodyWorldTransform.getRotation();

        const glm::vec3 bodyPos {
            btBodyPosition.x(),
            btBodyPosition.y(),
            btBodyPosition.z()
        };

        /*const glm::quat bodyRot {
            btBodyRotation.w(),
            btBodyRotation.x(),
            btBodyRotation.y(),
            btBodyRotation.z()
        };*/

        auto& ownTransform = transform->m_ownTransform;

        if(parentRigidbody && parentTransform)
        {
            const auto& parentBodyWorldTransform = parentRigidbody->m_body->getWorldTransform();

            const auto btParentBodyPosition = parentBodyWorldTransform.getOrigin();
            const auto btParentBodyRotation = parentBodyWorldTransform.getRotation();

            const glm::vec3 parentBodyPos {
                btParentBodyPosition.x(),
                btParentBodyPosition.y(),
                btParentBodyPosition.z()
            };

            // ownTransform.m_position = glm::inverse(parentTransform->m_finalTransform.m_rotation) * (bodyPos - parentTransform->m_finalTransform.m_position);
            // todo: учесть поворот
            ownTransform.m_position = (bodyPos - parentBodyPos) / parentTransform->m_finalTransform.m_scale;
            // todo: сделать отмену поворота
        }
        else if(parentTransform)
        {
            // todo: учесть поворот
            ownTransform.m_position = (bodyPos - parentTransform->m_finalTransform.m_position) / parentTransform->m_finalTransform.m_scale;
            // todo: сделать отмену поворота
        }
        else
        {
            ownTransform.m_position = bodyPos;
        }
    });

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
