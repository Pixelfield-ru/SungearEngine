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

    // todo: fix!!!
    if(m_changedModelMatrices.isLocked() || m_entitiesForPhysicsUpdateToCheck.isLocked()) return;

    auto registry = lockedScene->getECSRegistry();

    auto transformsView = registry->view<Transform>();

    auto& matrices = m_changedModelMatrices.getWrapped();
    auto& notPhysicalEntities = m_calculatedNotPhysicalEntities.getWrapped();
    auto& physicEntitiesToCheck = m_entitiesForPhysicsUpdateToCheck.getWrapped();

    matrices.reserve(transformsView.size());
    notPhysicalEntities.reserve(transformsView.size());
    physicEntitiesToCheck.reserve(transformsView.size());

    transformsView.each([&registry, &matrices, &notPhysicalEntities, &physicEntitiesToCheck](const ECS::entity_t& entity, Transform::reg_t transform) {
        if(transform)
        {
            auto* entityBaseInfo = registry->tryGet<EntityBaseInfo>(entity);
            Ref<Transform> parentTransform;

            if(entityBaseInfo)
            {
                auto* tmp = registry->tryGet<Transform>(entityBaseInfo->getParent());
                parentTransform = (tmp ? *tmp : nullptr);
            }

            TransformBase& finalTransform = transform->m_finalTransform;

            const bool isTransformChanged = TransformUtils::calculateTransform(*transform, parentTransform.get());

            if(isTransformChanged)
            {
                matrices.push_back({ entity, finalTransform.m_modelMatrix });
                notPhysicalEntities.push_back({ entity, transform });
            }
            else
            {
                physicEntitiesToCheck.push_back(entity);
            }
        }
    });

    if(!m_calculatedNotPhysicalEntitiesCopy.isLocked())
    {
        std::lock_guard guard(m_calculatedNotPhysicalEntitiesCopy);
        auto& vec = m_calculatedNotPhysicalEntitiesCopy.getWrapped();
        if(vec.empty())
        {
            vec = std::move(notPhysicalEntities);
        }
    }

    // todo: fix!!!!
    /*m_changedModelMatrices.lock();
    m_entitiesForPhysicsUpdateToCheck.lock();*/
}

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    if(!m_calculatedNotPhysicalEntitiesCopy.isLocked())
    {
        std::lock_guard guard(m_calculatedNotPhysicalEntitiesCopy);
        
        size_t i = 0;
        
        auto& entitiesCopy = m_calculatedNotPhysicalEntitiesCopy.getWrapped();
        for(const auto& t : entitiesCopy)
        {
            auto* tmpNonConstTransform = lockedScene->getECSRegistry()->tryGet<Transform>(t.m_owner);
            Ref<Transform> nonConstTransform = (tmpNonConstTransform ? *tmpNonConstTransform : nullptr);
            Mesh* mesh = lockedScene->getECSRegistry()->tryGet<Mesh>(t.m_owner);
            if(nonConstTransform && mesh)
            {
                auto& finalTransform = nonConstTransform->m_finalTransform;
                auto& ownTransform = nonConstTransform->m_ownTransform;
                
                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;
                glm::vec3 skew;
                glm::vec4 perspective;
                
                glm::decompose(finalTransform.m_modelMatrix, scale, rotation, translation, skew,
                               perspective);

                if(mesh->m_base.getMeshData())
                {
                    finalTransform.m_aabb.applyTransformations(translation, rotation, scale,
                                                               mesh->m_base.getMeshData()->m_aabb);
                }
                
                finalTransform.m_position = translation;
                finalTransform.m_lastPosition = translation;
                
                finalTransform.m_rotation = rotation;
                finalTransform.m_lastRotation = rotation;
                
                finalTransform.m_scale = scale;
                finalTransform.m_lastScale = scale;
                
                ownTransform.m_aabb = finalTransform.m_aabb;
            }
            
            onTransformChanged(lockedScene->getECSRegistry(), t.m_owner, t.m_memberValue);
            
            ++i;
        }
        
        entitiesCopy.clear();
    }
    
    // std::cout << "calculatedNotPhysEntitiesLastSize - 1: " << (calculatedNotPhysEntitiesLastSize - 1) << std::endl;
    
    if(!m_calculatedPhysicalEntitiesCopy.isLocked())
    {
        std::lock_guard guard(m_calculatedPhysicalEntitiesCopy);
        auto& entitiesCopy = m_calculatedPhysicalEntitiesCopy.getWrapped();
        for(const auto& t : entitiesCopy)
        {
            auto* tmpNonConstTransform = lockedScene->getECSRegistry()->tryGet<Transform>(t.m_owner);
            Ref<Transform> nonConstTransform = (tmpNonConstTransform ? *tmpNonConstTransform : nullptr);
            Mesh* mesh = lockedScene->getECSRegistry()->tryGet<Mesh>(t.m_owner);
            if(nonConstTransform && mesh)
            {
                auto& finalTransform = nonConstTransform->m_finalTransform;
                auto& ownTransform = nonConstTransform->m_ownTransform;
                
                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;
                glm::vec3 skew;
                glm::vec4 perspective;
                
                glm::decompose(finalTransform.m_modelMatrix, scale, rotation, translation, skew,
                               perspective);
                
                finalTransform.m_aabb.applyTransformations(translation, rotation, scale,
                                                           mesh->m_base.getMeshData()->m_aabb);
                
                finalTransform.m_position = translation;
                finalTransform.m_lastPosition = translation;
                
                finalTransform.m_rotation = rotation;
                finalTransform.m_lastRotation = rotation;
                
                finalTransform.m_scale = scale;
                finalTransform.m_lastScale = scale;
                
                ownTransform.m_aabb = finalTransform.m_aabb;
            }
            
            onTransformChanged(lockedScene->getECSRegistry(), t.m_owner, t.m_memberValue);
        }
        
        entitiesCopy.clear();
    }
}
