//
// Created by stuka on 02.05.2023.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <execution>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "TransformationsUpdater.h"

#include <stack>

#include "TransformBase.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Physics/Rigidbody3D.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "Transform.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/ECS/Registry.h"

#include "TransformUtils.h"
#include "SGCore/Scene/RootEntityTag.h"

SGCore::TransformationsUpdater::TransformationsUpdater()
{
    std::printf("creating TransformationsUpdater %llu\n", std::hash<size_t>()((size_t) (IParallelSystem<TransformationsUpdater>*) this));
    m_thread->setSleepTime(std::chrono::milliseconds(0));
    m_thread->start();
}

void SGCore::TransformationsUpdater::update(double dt, double fixedDt) noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto registry = lockedScene->getECSRegistry();

    auto transformsView = registry->view<EntityBaseInfo, Transform, RootEntityTag>();

    auto start = std::chrono::system_clock::now();

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for clearing notTransformUpdatedEntities and copying updatedByPhysicsEntities: " << updatedByPhysicsEntities.size() << std::endl;

    // =======================================================================
    transformsView.each([&](ECS::entity_t entity,
                                 EntityBaseInfo& entityBaseInfo,
                                 const Transform::reg_t& transform, auto) {
        m_entitiesDesc.emplace(entity, &entityBaseInfo, &transform, nullptr);

        while(!m_entitiesDesc.empty())
        {
            auto [currentEntity, currentEntityBaseInfo, currentTransform, parentTransform] = m_entitiesDesc.top();
            m_entitiesDesc.pop();

            // ======================= updating transform

            if(currentTransform && (*currentTransform)->isActive())
            {
                auto& finalTransform = (*currentTransform)->m_finalTransform;
                auto& ownTransform = (*currentTransform)->m_ownTransform;

                bool isTransformChanged = false;

                // if(!rigidbody3D)
                {
                    isTransformChanged |= TransformUtils::calculateTransform(*currentTransform->get(), parentTransform);
                }

                if(isTransformChanged)
                {
                    onTransformChanged(registry, currentEntity, *currentTransform);
                }

                // calculating aabb
                const auto* mesh = registry->tryGet<Mesh>(currentEntity);
                if(mesh && mesh->m_base.getMeshData())
                {
                    finalTransform.m_aabb.applyTransformations(finalTransform.m_position, finalTransform.m_rotation, finalTransform.m_scale, mesh->m_base.getMeshData()->m_aabb);
                    ownTransform.m_aabb.applyTransformations(ownTransform.m_position, ownTransform.m_rotation, ownTransform.m_scale, mesh->m_base.getMeshData()->m_aabb);
                }
            }

            // =======================

            for(auto childEntity : currentEntityBaseInfo->getChildren())
            {
                const auto& childBaseInfo = registry->get<EntityBaseInfo>(childEntity);
                const auto* childTransform = registry->tryGet<Transform>(childEntity);

                m_entitiesDesc.emplace(childEntity, &childBaseInfo, childTransform, currentTransform ? currentTransform->get() : parentTransform);
            }
        }
    });

    // ==========================================================================================

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for updating transforms of entities" << std::endl;
}

void SGCore::TransformationsUpdater::fixedUpdate(double dt, double fixedDt) noexcept
{

}
