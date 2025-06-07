//
// Created by stuka on 02.05.2023.
//

#include <glm/gtc/type_ptr.hpp>
#include <execution>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "TransformationsUpdater.h"
#include "TransformBase.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Physics/Rigidbody3D.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "Transform.h"
#include "SGCore/Render/Mesh.h"
#include <glm/gtx/string_cast.hpp>

#include "TransformUtils.h"

SGCore::TransformationsUpdater::TransformationsUpdater()
{
    std::printf("creating TransformationsUpdater %llu\n", std::hash<size_t>()((size_t) (IParallelSystem<TransformationsUpdater>*) this));
    m_thread->setSleepTime(std::chrono::milliseconds(0));
    m_thread->start();
}

void SGCore::TransformationsUpdater::update(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto registry = lockedScene->getECSRegistry();

    auto transformsView = registry->view<EntityBaseInfo, Transform>();

    std::vector<ECS::entity_t> notTransformUpdatedEntities;
    std::unordered_set<ECS::entity_t> notTransformUpdatedEntitiesSet;

    auto start = std::chrono::system_clock::now();
    {
        std::lock_guard lock(m_notTransformUpdatedEntitiesMutex);

        notTransformUpdatedEntities = m_notTransformUpdatedEntities;
        notTransformUpdatedEntitiesSet = m_notTransformUpdatedEntitiesSet;
        // m_notTransformUpdatedEntities.clear();
    }

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for clearing notTransformUpdatedEntities and copying updatedByPhysicsEntities: " << updatedByPhysicsEntities.size() << std::endl;

    // =======================================================================

    transformsView.each([&registry, &notTransformUpdatedEntities, &notTransformUpdatedEntitiesSet, this](
    ECS::entity_t entity,
    EntityBaseInfo& entityBaseInfo,
    const Transform::reg_t& transform) {
        // starting only on root entities
        if(entityBaseInfo.getParent() != entt::null) return;

        updateTransform(entityBaseInfo, entity, transform, nullptr, registry, notTransformUpdatedEntities, notTransformUpdatedEntitiesSet);

        /*Ref<Transform> parentTransform;
        Ref<Rigidbody3D> rigidbody3D;

        if(entityBaseInfo.getParent() != entt::null)
        {
            auto* tmp = registry->tryGet<Transform>(entityBaseInfo.getParent());
            parentTransform = tmp ? *tmp : nullptr;
        }

        {
            auto* tmpRigidbody3D = registry->tryGet<Rigidbody3D>(entity);
            rigidbody3D = tmpRigidbody3D ? *tmpRigidbody3D : nullptr;
        }

        TransformBase& finalTransform = transform->m_finalTransform;

        const bool isTransformChanged = TransformUtils::calculateTransform(*transform, parentTransform.get());

        if(!isTransformChanged)
        {
            if(!notTransformUpdatedEntitiesSet.contains(entity) && rigidbody3D)
            {
                notTransformUpdatedEntitiesSet.insert(entity);
                notTransformUpdatedEntities.push_back(entity);
            }
        }
        else
        {
            // updating rigidbody3d =================================================

            if(rigidbody3D)
            {
                auto& rigidbody3DTransform = rigidbody3D->m_body->getWorldTransform();

                const auto noScaleMatrix = glm::mat4(glm::mat3(finalTransform.m_animatedModelMatrix));

                rigidbody3DTransform.setIdentity();
                rigidbody3DTransform.setOrigin({ finalTransform.m_position.x, finalTransform.m_position.y, finalTransform.m_position.z });
                rigidbody3DTransform.setRotation({ finalTransform.m_rotation.x, finalTransform.m_rotation.y, finalTransform.m_rotation.z, finalTransform.m_rotation.w });
                // rigidbody3DTransform.setFromOpenGLMatrix(&noScaleMatrix[0][0]);
            }

            // =====================================================================

            onTransformChanged(registry, entity, transform);
        }*/
    });

    // ==========================================================================================

    {
        std::lock_guard lock(m_notTransformUpdatedEntitiesMutex);
        m_notTransformUpdatedEntities = std::move(notTransformUpdatedEntities);
        m_notTransformUpdatedEntitiesSet = std::move(notTransformUpdatedEntitiesSet);
    }

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for updating transforms of entities" << std::endl;
}

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{

}

void SGCore::TransformationsUpdater::updateTransform(const EntityBaseInfo::reg_t& currentEntityBaseInfo,
                                                     const ECS::entity_t& currentEntity,
                                                     const Transform::reg_t& currentEntityTransform,
                                                     const Transform::reg_t& parentTransform,
                                                     const Ref<ECS::registry_t>& inRegistry,
                                                     std::vector<ECS::entity_t>& notTransformUpdatedEntities,
                                                     std::unordered_set<ECS::entity_t>& notTransformUpdatedEntitiesSet)
{
    if(currentEntityTransform)
    {
        Ref<Rigidbody3D> rigidbody3D;

        {
            auto* tmpRigidbody3D = inRegistry->tryGet<Rigidbody3D>(currentEntity);
            rigidbody3D = tmpRigidbody3D ? *tmpRigidbody3D : nullptr;
        }

        TransformBase& finalTransform = currentEntityTransform->m_finalTransform;

        const bool isTransformChanged = TransformUtils::calculateTransform(*currentEntityTransform, parentTransform.get());

        if(!isTransformChanged)
        {
            if(!notTransformUpdatedEntitiesSet.contains(currentEntity) && rigidbody3D)
            {
                notTransformUpdatedEntitiesSet.insert(currentEntity);
                notTransformUpdatedEntities.push_back(currentEntity);
            }
        }
        else
        {
            // updating rigidbody3d =================================================

            if(rigidbody3D)
            {
                auto& rigidbody3DTransform = rigidbody3D->m_body->getWorldTransform();

                rigidbody3DTransform.setIdentity();
                rigidbody3DTransform.setOrigin({ finalTransform.m_position.x, finalTransform.m_position.y, finalTransform.m_position.z });
                rigidbody3DTransform.setRotation({ finalTransform.m_rotation.x, finalTransform.m_rotation.y, finalTransform.m_rotation.z, finalTransform.m_rotation.w });
                // rigidbody3DTransform.setFromOpenGLMatrix(&noScaleMatrix[0][0]);
            }

            // =====================================================================

            onTransformChanged(inRegistry, currentEntity, currentEntityTransform);
        }
    }

    for(const auto& childEntity : currentEntityBaseInfo.getChildren())
    {
        const auto& childBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
        const auto* childTransform = inRegistry->tryGet<Transform>(childEntity);
        updateTransform(childBaseInfo, childEntity, childTransform ? *childTransform : nullptr,
                        currentEntityTransform, inRegistry, notTransformUpdatedEntities,
                        notTransformUpdatedEntitiesSet);
    }
}
