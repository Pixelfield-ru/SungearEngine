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

    m_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_dynamicsWorld->setDebugDrawer(m_debugDraw.get());

    m_dynamicsWorld->setGravity({ 0, -98.0, 0 });

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
    updateWorld(dt, fixedDt);
}

void SGCore::PhysicsWorld3D::update(const double& dt, const double& fixedDt) noexcept
{
    // updateWorld(dt, fixedDt);

    auto lockedScene = getScene();

    if(!lockedScene) return;

    m_dynamicsWorld->debugDrawWorld();
}

void SGCore::PhysicsWorld3D::onAddToScene(const Ref<Scene>& scene)
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
    if(!m_simulate) return;

    std::lock_guard guard(m_bodiesCountChangeMutex);

    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto registry = lockedScene->getECSRegistry();

    Ref<TransformationsUpdater> transformationsUpdater = lockedScene->getSystem<TransformationsUpdater>();

    if(!transformationsUpdater) return;

    std::vector<ECS::entity_t> entitiesToUpdate;
    std::vector<ECS::entity_t> updatedEntities;

    auto start = std::chrono::system_clock::now();
    {
        std::lock_guard lock(transformationsUpdater->m_notTransformUpdatedEntitiesMutex);

        entitiesToUpdate = transformationsUpdater->m_notTransformUpdatedEntities;
    }

    glm::mat4 rigidbody3DMatrix;

    for(const auto e : entitiesToUpdate)
    {
        Ref<Rigidbody3D> rigidbody3D;
        Ref<Transform> parentTransform;
        auto transform = registry->get<Transform>(e);
        auto& entityBaseInfo = registry->get<EntityBaseInfo>(e);

        {
            auto tmpRigidbody3D = registry->tryGet<Rigidbody3D>(e);
            rigidbody3D = tmpRigidbody3D ? *tmpRigidbody3D : nullptr;
        }

        if(!rigidbody3D) continue;

        auto inversedParentTranslationMatrix = glm::mat4(1.0);
        auto inversedParentRotationMatrix = glm::mat4(1.0);
        auto inversedParentScaleMatrix = glm::mat4(1.0);

        if(entityBaseInfo.getParent() != entt::null)
        {
            auto tmpParentTransform = registry->tryGet<Transform>(entityBaseInfo.getParent());
            parentTransform = tmpParentTransform ? *tmpParentTransform : nullptr;
        }

        if(parentTransform)
        {
            inversedParentTranslationMatrix = glm::inverse(
                parentTransform->m_finalTransform.m_translationMatrix);
            inversedParentRotationMatrix = glm::inverse(
                parentTransform->m_finalTransform.m_rotationMatrix);
            inversedParentScaleMatrix = glm::inverse(
                parentTransform->m_finalTransform.m_scaleMatrix);
        }

        rigidbody3D->m_body->getWorldTransform().getOpenGLMatrix(&rigidbody3DMatrix[0][0]);

        // getting entity`s only own transform from rigidbody3d
        rigidbody3DMatrix =
                        inversedParentScaleMatrix * inversedParentRotationMatrix *
                        inversedParentTranslationMatrix *
                        rigidbody3DMatrix;

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;

        glm::decompose(rigidbody3DMatrix, scale, rotation, translation, skew,
                       perspective);

        if(rotation != transform->m_ownTransform.m_rotation ||
           translation != transform->m_ownTransform.m_position)
        {
            transform->m_ownTransform.m_position = translation;
            transform->m_ownTransform.m_rotation = rotation;

            TransformUtils::calculateTransform(*transform, parentTransform.get());

            transformationsUpdater->onTransformChanged(registry, e, transform);
        }
    }

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for updating transforms of entities (count: " << entitiesToUpdate.size() << ")" << std::endl;

    {
        std::lock_guard lock(transformationsUpdater->m_notTransformUpdatedEntitiesMutex);
        transformationsUpdater->m_notTransformUpdatedEntities.clear();
        transformationsUpdater->m_notTransformUpdatedEntitiesSet.clear();
    }

    m_dynamicsWorld->stepSimulation(dt, 12, dt);
}
