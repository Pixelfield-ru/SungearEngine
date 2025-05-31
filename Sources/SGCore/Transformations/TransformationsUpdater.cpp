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

    auto transformsView = registry->view<Transform>();

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

    transformsView.each([&registry, &notTransformUpdatedEntities, &notTransformUpdatedEntitiesSet, this](ECS::entity_t entity, Transform::reg_t transform) {
        auto& entityBaseInfo = registry->get<EntityBaseInfo>(entity);
        Ref<Transform> parentTransform;
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

                rigidbody3DTransform.setIdentity();
                rigidbody3DTransform.setFromOpenGLMatrix(&finalTransform.m_animatedModelMatrix[0][0]);
            }

            // =====================================================================

            onTransformChanged(registry, entity, transform);
        }
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
