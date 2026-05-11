//
// Created by stuka on 02.05.2023.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <execution>
#include <print>

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
    std::printf("creating TransformationsUpdater %llu\n", std::hash<size_t>()((size_t) (ISystem*) this));
}

void SGCore::TransformationsUpdater::update(double dt, double fixedDt) noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto registry = lockedScene->getECSRegistry();

    auto transformsView = registry->view<EntityBaseInfo, RootEntityTag>();

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for clearing notTransformUpdatedEntities and copying updatedByPhysicsEntities: " << updatedByPhysicsEntities.size() << std::endl;

    // =======================================================================
    for(auto&& [entity, rootBaseInfo, rootTag] : transformsView.each())
    {
        auto* rootTransform = registry->tryGet<Transform>(entity);

        m_entitiesDesc.emplace(entity, &rootBaseInfo, rootTransform, nullptr);

        // auto start = std::chrono::steady_clock::now();
        while(!m_entitiesDesc.empty())
        {
            auto [currentEntity, currentEntityBaseInfo, currentTransform, parentTransform] = m_entitiesDesc.top();
            m_entitiesDesc.pop();

            // ======================= updating transform

            if(currentTransform && currentTransform->isActive())
            {
                auto& worldTransform = currentTransform->m_worldTransform;
                auto& localTransform = currentTransform->m_localTransform;

                bool isTransformChanged = false;

                // if(!rigidbody3D)
                {
                    isTransformChanged |= TransformUtils::calculateTransform(*currentTransform, parentTransform);
                }

                /*if(isTransformChanged)
                {
                    onTransformChanged(registry, currentEntity, *currentTransform);
                }*/
            }

            // =======================

            for(auto childEntity : currentEntityBaseInfo->getChildren())
            {
                const auto& childBaseInfo = registry->get<EntityBaseInfo>(childEntity);
                auto* childTransform = registry->tryGet<Transform>(childEntity);

                m_entitiesDesc.emplace(childEntity, &childBaseInfo, childTransform, currentTransform ? currentTransform : parentTransform);
            }
        }
    }

    auto meshesView = registry->view<Transform, Mesh>();
    for(auto&& [entity, transform, mesh] : meshesView.each())
    {
        auto& worldTransform = transform.m_worldTransform;
        auto& localTransform = transform.m_localTransform;

        // calculating aabb
        if(mesh.m_base.getMeshData())
        {
            worldTransform.m_aabb.applyTransformations(worldTransform.m_position, worldTransform.m_rotation, worldTransform.m_scale, mesh.m_base.getMeshData()->m_aabb);
            localTransform.m_aabb.applyTransformations(localTransform.m_position, localTransform.m_rotation, localTransform.m_scale, mesh.m_base.getMeshData()->m_aabb);
        }
    }

    // ==========================================================================================

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for updating transforms of entities" << std::endl;
}

void SGCore::TransformationsUpdater::fixedUpdate(double dt, double fixedDt) noexcept
{

}
