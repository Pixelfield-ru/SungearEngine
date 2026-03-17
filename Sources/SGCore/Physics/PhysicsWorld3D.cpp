//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

// for glm/gtx/matrix_decompose.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "PhysicsDebugDraw.h"
#include "Rigidbody3D.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Threading/WrappedObject.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/RootEntityTag.h"
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
    m_dynamicsWorld->debugDrawWorld();

    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto ecsRegistry = lockedScene->getECSRegistry();

    // ============================= set rigidbodies world position (even if the world is not simulated (to synchronize))

    auto rigidbodiesView = ecsRegistry->view<EntityBaseInfo, Rigidbody3D, Transform>();

    rigidbodiesView.each([](const auto&, const Ref<Rigidbody3D>& rigidbody, const Ref<Transform>& transform) {
        const auto worldPosition = transform->m_finalTransform.m_position;
        const auto worldRotation = transform->m_finalTransform.m_rotation;

        auto& bodyTransform = rigidbody->m_body->getWorldTransform();

        bodyTransform.setOrigin({ worldPosition.x, worldPosition.y, worldPosition.z });
        bodyTransform.setRotation({ worldRotation.x, worldRotation.y, worldRotation.z, worldRotation.w });
    });

    // =============================

    // skipping iteration now
    if(!m_simulate) return;

    // ============================= updating world
    {
        std::lock_guard guard(m_bodiesCountChangeMutex);

        m_dynamicsWorld->stepSimulation(dt, 12, dt);
    }

    // ============================= calculating post-physics transform to have actual positions and rotations
    auto transformsView = ecsRegistry->view<EntityBaseInfo, Transform, RootEntityTag>();

    transformsView.each([&](ECS::entity_t entity,
                            EntityBaseInfo& entityBaseInfo,
                            const Transform::reg_t& transform, auto) {
        calculatePostPhysicsEntityTransform(entityBaseInfo, entity, transform, nullptr, ecsRegistry);
    });

    // ============================= set transform world position
    // rigidbodies are ignoring parent rigidbodies. rigidbodies are independent of parent rigidbodies in physics world
    rigidbodiesView.each([&ecsRegistry](const EntityBaseInfo& baseInfo, const Ref<Rigidbody3D>& rigidbody, const Ref<Transform>& transform) {
        Transform* parentTransform {};

        if(baseInfo.getParent() != entt::null)
        {
            auto tmpTransform = ecsRegistry->tryGet<Transform>(baseInfo.getParent());
            parentTransform = tmpTransform ? tmpTransform->get() : nullptr;
        }

        auto& ownTransform = transform->m_ownTransform;
        auto& finalTransform = transform->m_finalTransform;

        if(parentTransform)
        {
            // calculating local position & rotation to ignore parent rigidbody transform
            ownTransform.m_position = glm::inverse(parentTransform->m_finalTransform.m_rotation) * ((finalTransform.m_position - parentTransform->m_finalTransform.m_position) / parentTransform->m_finalTransform.m_scale);
            ownTransform.m_rotation = glm::inverse(parentTransform->m_finalTransform.m_rotation) * finalTransform.m_rotation;
        }
        else
        {
            // no parent then using transform from rigidbody (finalTransform)
            ownTransform.m_position = finalTransform.m_position;
            ownTransform.m_rotation = finalTransform.m_rotation;
        }
    });
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
    std::lock_guard guard(m_bodiesCountChangeMutex);

    m_dynamicsWorld->stepSimulation(dt, 12, dt);
}

void SGCore::PhysicsWorld3D::calculatePostPhysicsEntityTransform(const EntityBaseInfo::reg_t& currentEntityBaseInfo,
                                                                 const ECS::entity_t& currentEntity,
                                                                 const Transform::reg_t& currentEntityTransform,
                                                                 const Transform::reg_t& parentTransform,
                                                                 const Ref<ECS::registry_t>& inRegistry) noexcept
{
    Rigidbody3D* childRigidbody {};

    {
        auto tmpRigidbody = inRegistry->tryGet<Rigidbody3D>(currentEntity);
        childRigidbody = tmpRigidbody ? tmpRigidbody->get() : nullptr;
    }

    auto& childFinalTransform = currentEntityTransform->m_finalTransform;

    if(parentTransform && !childRigidbody) // calculating child position and rotation relating to parent
    {
        const auto& parentFinalTransform = parentTransform->m_finalTransform;
        const auto& childOwnTransform = currentEntityTransform->m_ownTransform;

        childFinalTransform.m_position = parentFinalTransform.m_position + parentFinalTransform.m_rotation * (childOwnTransform.m_position * parentFinalTransform.m_scale);
        childFinalTransform.m_rotation = parentFinalTransform.m_rotation * childOwnTransform.m_rotation;
    }
    else if(childRigidbody) // using rigidbody`s position and rotation as final position and rotation
    {
        const auto& bodyTransform = childRigidbody->m_body->getWorldTransform();

        const auto btPosition = bodyTransform.getOrigin();
        const auto btRotation = bodyTransform.getRotation();

        const glm::vec3 bodyPos {
            btPosition.x(),
            btPosition.y(),
            btPosition.z()
        };

        const glm::quat bodyRot {
            btRotation.w(),
            btRotation.x(),
            btRotation.y(),
            btRotation.z()
        };

        childFinalTransform.m_position = bodyPos;
        childFinalTransform.m_rotation = bodyRot;
    }
    // else skipping child because it does not have parent and rigidbody. transform was not changed by parent

    // iterating through all children
    for(const auto& childEntity : currentEntityBaseInfo.getChildren())
    {
        const auto& childBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
        const auto* childTransform = inRegistry->tryGet<Transform>(childEntity);
        calculatePostPhysicsEntityTransform(childBaseInfo, childEntity, childTransform ? *childTransform : nullptr,
                                            currentEntityTransform, inRegistry);
    }
}
