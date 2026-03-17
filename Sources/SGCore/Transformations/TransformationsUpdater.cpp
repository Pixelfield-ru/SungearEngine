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

void SGCore::TransformationsUpdater::update(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto registry = lockedScene->getECSRegistry();

    auto transformsView = registry->view<EntityBaseInfo, Transform, RootEntityTag>();

    auto start = std::chrono::system_clock::now();

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for clearing notTransformUpdatedEntities and copying updatedByPhysicsEntities: " << updatedByPhysicsEntities.size() << std::endl;

    // =======================================================================

    transformsView.each([&registry, this](ECS::entity_t entity,
                                          EntityBaseInfo& entityBaseInfo,
                                          const Transform::reg_t& transform, auto) {
        updateTransform(entityBaseInfo, entity, transform, nullptr, registry);
    });

    // ==========================================================================================

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for updating transforms of entities" << std::endl;
}

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{

}

void SGCore::TransformationsUpdater::updateTransform(const EntityBaseInfo::reg_t& currentEntityBaseInfo,
                                                     const ECS::entity_t& currentEntity,
                                                     const Transform::reg_t& currentEntityTransform,
                                                     const Transform::reg_t& parentTransform,
                                                     const Ref<ECS::registry_t>& inRegistry)
{
    if(currentEntityTransform && currentEntityBaseInfo.m_isActive)
    {
        TransformBase& finalTransform = currentEntityTransform->m_finalTransform;
        TransformBase& ownTransform = currentEntityTransform->m_ownTransform;

        bool isTransformChanged = false;

        // if(!rigidbody3D)
        {
            isTransformChanged |= TransformUtils::calculateTransform(*currentEntityTransform, parentTransform.get());
        }

        if(isTransformChanged)
        {
            onTransformChanged(inRegistry, currentEntity, currentEntityTransform);
        }

        // calculating aabb
        const auto* mesh = inRegistry->tryGet<Mesh>(currentEntity);
        if(mesh && mesh->m_base.getMeshData())
        {
            finalTransform.m_aabb.applyTransformations(finalTransform.m_position, finalTransform.m_rotation, finalTransform.m_scale, mesh->m_base.getMeshData()->m_aabb);
            ownTransform.m_aabb.applyTransformations(ownTransform.m_position, ownTransform.m_rotation, ownTransform.m_scale, mesh->m_base.getMeshData()->m_aabb);
        }
    }

    for(const auto& childEntity : currentEntityBaseInfo.getChildren())
    {
        const auto& childBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
        const auto* childTransform = inRegistry->tryGet<Transform>(childEntity);
        updateTransform(childBaseInfo, childEntity, childTransform ? *childTransform : nullptr,
                        currentEntityTransform, inRegistry);
    }
}
