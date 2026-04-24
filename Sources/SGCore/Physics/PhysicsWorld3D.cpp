//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

// for glm/gtx/matrix_decompose.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <glm/gtx/quaternion.hpp>

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
#include "SGCore/Utils/Defer.h"

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

void SGCore::PhysicsWorld3D::update(double dt, double fixedDt) noexcept
{
    sg_defer [this] { m_dynamicsWorld->debugDrawWorld(); };

    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto ecsRegistry = lockedScene->getECSRegistry();

    // ============================= calculating pre-physics transform to have actual positions and rotations

    calculatePrePhysicsEntitiesTransforms(ecsRegistry);

    // ============================= set rigidbodies world position (even if the world is not simulated (to synchronize))

    auto rigidbodiesView = ecsRegistry->view<EntityBaseInfo, Rigidbody3D, Transform>();

    rigidbodiesView.each([](const auto&, const Rigidbody3D& rigidbody, const Transform& transform) {
        const auto worldPosition = transform.m_worldTransform.m_position;
        const auto worldRotation = transform.m_worldTransform.m_rotation;

        auto& bodyTransform = rigidbody.m_body->getWorldTransform();

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

    calculatePostPhysicsEntitiesTransforms(ecsRegistry);

    // ============================= set transform world position
    // rigidbodies are ignoring parent rigidbodies. rigidbodies are independent of parent rigidbodies in physics world
    rigidbodiesView.each([&ecsRegistry](const EntityBaseInfo& baseInfo, const Rigidbody3D& rigidbody, Transform& transform) {
        auto* parentTransform = ecsRegistry->tryGet<Transform>(baseInfo.getParent());

        auto& localTransform = transform.m_localTransform;
        auto& worldTransform = transform.m_worldTransform;

        if(parentTransform)
        {
            localTransform.m_position = TransformUtils::calculateLocalPosition(*parentTransform, worldTransform.m_position);
            localTransform.m_rotation = TransformUtils::calculateLocalRotation(*parentTransform, worldTransform.m_rotation);

            // calculating local position & rotation to ignore parent rigidbody transform
            /*localTransform.m_position = glm::inverse(parentTransform->m_worldTransform.m_rotation) * ((worldTransform.m_position - parentTransform->m_worldTransform.m_position) / parentTransform->m_worldTransform.m_scale);
            localTransform.m_rotation = glm::inverse(parentTransform->m_worldTransform.m_rotation) * worldTransform.m_rotation;*/
        }
        else
        {
            // no parent then using transform from rigidbody (worldTransform)
            localTransform.m_position = worldTransform.m_position;
            localTransform.m_rotation = worldTransform.m_rotation;
        }
    });
}

void SGCore::PhysicsWorld3D::onAddToScene(const Scene* scene)
{
    if (!scene) return;

    auto rigidbodies3DView = scene->getECSRegistry()->view<Rigidbody3D>();

    rigidbodies3DView.each([this](const Rigidbody3D& rigidbody3D) {
        this->addBody(rigidbody3D.m_body);
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

void SGCore::PhysicsWorld3D::calculatePrePhysicsEntitiesTransforms(const Ref<ECS::registry_t>& inRegistry) noexcept
{
    auto transformsView = inRegistry->view<EntityBaseInfo, Transform, RootEntityTag>();

    transformsView.each([&](ECS::entity_t entity,
                            EntityBaseInfo& entityBaseInfo,
                            Transform& transform, auto) {
        m_transformableEntitiesDesc.emplace(entity, &entityBaseInfo, &transform, nullptr);

        while(!m_transformableEntitiesDesc.empty())
        {
            auto [currentEntity, currentEntityBaseInfo, currentTransform, parentTransform] = m_transformableEntitiesDesc.top();
            m_transformableEntitiesDesc.pop();

            // ======================= updating transform

            if(currentTransform && currentTransform->isActive())
            {
                auto& childWorldTransform = currentTransform->m_worldTransform;
                const auto& childLocalTransform = currentTransform->m_localTransform;

                if(parentTransform) // calculating child position and rotation relating to parent
                {
                    childWorldTransform.m_position = TransformUtils::calculateWorldPosition(*parentTransform, childLocalTransform.m_position);
                    childWorldTransform.m_rotation = TransformUtils::calculateWorldRotation(*parentTransform, childLocalTransform.m_rotation);
                }
                else // else using local transform as world transform
                {
                    childWorldTransform.m_position = childLocalTransform.m_position;
                    childWorldTransform.m_rotation = childLocalTransform.m_rotation;
                }
                // else skipping child because it does not have parent
            }

            // =======================

            // iterating through all children
            for(auto childEntity : currentEntityBaseInfo->getChildren())
            {
                const auto& childBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
                auto* childTransform = inRegistry->tryGet<Transform>(childEntity);

                m_transformableEntitiesDesc.emplace(childEntity, &childBaseInfo, childTransform, currentTransform ? currentTransform : parentTransform);
            }
        }
    });
}

void SGCore::PhysicsWorld3D::calculatePostPhysicsEntitiesTransforms(const Ref<ECS::registry_t>& inRegistry) noexcept
{
    auto transformsView = inRegistry->view<EntityBaseInfo, Transform, RootEntityTag>();

    transformsView.each([&](ECS::entity_t entity,
                            EntityBaseInfo& entityBaseInfo,
                            Transform::reg_t& transform, auto) {
        m_transformableEntitiesDesc.emplace(entity, &entityBaseInfo, &transform, nullptr);

        while(!m_transformableEntitiesDesc.empty())
        {
            auto [currentEntity, currentEntityBaseInfo, currentTransform, parentTransform] = m_transformableEntitiesDesc.top();
            m_transformableEntitiesDesc.pop();

            // ======================= updating transform

            if(currentTransform && currentTransform->isActive())
            {
                auto* childRigidbody = inRegistry->tryGet<Rigidbody3D>(currentEntity);

                auto& childWorldTransform = currentTransform->m_worldTransform;

                if(parentTransform && !childRigidbody) // calculating child position and rotation relating to parent
                {
                    const auto& childLocalTransform = currentTransform->m_localTransform;

                    childWorldTransform.m_position = TransformUtils::calculateWorldPosition(*parentTransform, childLocalTransform.m_position);
                    childWorldTransform.m_rotation = TransformUtils::calculateWorldRotation(*parentTransform, childLocalTransform.m_rotation);
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

                    childWorldTransform.m_position = bodyPos;
                    childWorldTransform.m_rotation = bodyRot;
                }
                // else skipping child because it does not have parent and rigidbody. transform was not changed by parent
            }

            // =======================

            // iterating through all children
            for(auto childEntity : currentEntityBaseInfo->getChildren())
            {
                const auto& childBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
                auto* childTransform = inRegistry->tryGet<Transform>(childEntity);

                m_transformableEntitiesDesc.emplace(childEntity, &childBaseInfo, childTransform, currentTransform ? currentTransform : parentTransform);
            }
        }
    });
}