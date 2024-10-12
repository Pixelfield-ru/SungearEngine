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

SGCore::TransformationsUpdater::TransformationsUpdater()
{
    m_thread->setSleepTime(std::chrono::milliseconds(0));
    m_thread->start();
}

void SGCore::TransformationsUpdater::onAddToScene(const Ref<Scene>& scene)
{
    m_sharedScene = scene;
}

void SGCore::TransformationsUpdater::parallelUpdate(const double& dt, const double& fixedDt) noexcept
{
    if(!m_sharedScene) return;

    if(m_changedModelMatrices.isLocked() || m_entitiesForPhysicsUpdateToCheck.isLocked()) return;

    auto registry = m_sharedScene->getECSRegistry();
    
    auto transformsView = registry->view<Ref<Transform>>();
    
    auto& matrices = m_changedModelMatrices.getWrapped();
    auto& notPhysicalEntities = m_calculatedNotPhysicalEntities.getWrapped();
    auto& physicEntitiesToCheck = m_entitiesForPhysicsUpdateToCheck.getWrapped();

    matrices.reserve(transformsView.size());
    notPhysicalEntities.reserve(transformsView.size());
    physicEntitiesToCheck.reserve(transformsView.size());

    transformsView.each([&registry, &matrices, &notPhysicalEntities, &physicEntitiesToCheck, this](const entity_t& entity, Ref<Transform> transform) {
        if(transform)
        {
            EntityBaseInfo* entityBaseInfo = registry->try_get<EntityBaseInfo>(entity);
            Ref<Transform> parentTransform;
            
            if(entityBaseInfo)
            {
                auto* tmp = registry->try_get<Ref<Transform>>(entityBaseInfo->m_parent);
                parentTransform = (tmp ? *tmp : nullptr);
            }
            
            bool translationChanged = false;
            bool rotationChanged = false;
            bool scaleChanged = false;
            
            TransformBase& ownTransform = transform->m_ownTransform;
            TransformBase& finalTransform = transform->m_finalTransform;
            
            // translation ==============================================
            
            if(ownTransform.m_lastPosition != ownTransform.m_position)
            {
                ownTransform.m_translationMatrix = glm::translate(glm::mat4(1.0),
                                                                  ownTransform.m_position
                );
                
                // std::cout << "pos : " << ownTransform.m_position.x << ", " << ownTransform.m_position.y << ", " << ownTransform.m_position.z << std::endl;
                
                ownTransform.m_lastPosition = ownTransform.m_position;
                
                translationChanged = true;
            }
            
            if(parentTransform && transform->m_followParentTRS.x)
            {
                // std::cout << "dfdff" << std::endl;
                finalTransform.m_translationMatrix =
                        parentTransform->m_finalTransform.m_translationMatrix * ownTransform.m_translationMatrix;
            }
            else
            {
                finalTransform.m_translationMatrix = ownTransform.m_translationMatrix;
            }
            
            // rotation ================================================
            
            if(ownTransform.m_lastRotation != ownTransform.m_rotation)
            {                
                // rotating directions vectors
                ownTransform.m_left = glm::rotate(MathUtils::left3,
                                                  glm::radians(-ownTransform.m_rotation.y), glm::vec3(0, 1, 0));
                ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                                  glm::radians(-ownTransform.m_rotation.z), glm::vec3(0, 0, 1));
                
                ownTransform.m_left *= -1.0f;
                
                ownTransform.m_forward.x = cos(glm::radians(ownTransform.m_rotation.x)) * cos(glm::radians(ownTransform.m_rotation.y));
                ownTransform.m_forward.y = sin(glm::radians(ownTransform.m_rotation.y));
                ownTransform.m_forward.z = sin(glm::radians(ownTransform.m_rotation.x)) * cos(glm::radians(ownTransform.m_rotation.y));
                
                ownTransform.m_forward *= -1.0f;
                
                ownTransform.m_up = glm::rotate(MathUtils::up3,
                                                glm::radians(-ownTransform.m_rotation.x), glm::vec3(1, 0, 0));
                ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                                glm::radians(-ownTransform.m_rotation.z), glm::vec3(0, 0, 1));
                
                ownTransform.m_up *= -1.0f;

                ownTransform.m_lastRotation = ownTransform.m_rotation;
                
                rotationChanged = true;
            }
            
            if(parentTransform && transform->m_followParentTRS.y)
            {
                finalTransform.m_rotationMatrix =
                        parentTransform->m_finalTransform.m_rotationMatrix * ownTransform.m_rotationMatrix;
            }
            else
            {
                finalTransform.m_rotationMatrix = ownTransform.m_rotationMatrix;
            }
            
            // scale ========================================================
            
            if(ownTransform.m_lastScale != ownTransform.m_scale)
            {
                ownTransform.m_scaleMatrix = glm::scale(glm::mat4(1.0),
                                                        ownTransform.m_scale
                );
                ownTransform.m_lastScale = ownTransform.m_scale;
                
                scaleChanged = true;
            }
            
            if(parentTransform && transform->m_followParentTRS.z)
            {
                finalTransform.m_scaleMatrix =
                        parentTransform->m_finalTransform.m_scaleMatrix * ownTransform.m_scaleMatrix;
            }
            else
            {
                finalTransform.m_scaleMatrix = ownTransform.m_scaleMatrix;
            }
            
            // model matrix =================================================
            
            bool modelMatrixChanged = translationChanged || rotationChanged || scaleChanged;
            
            // О ТАК ВЕРНО
            transform->m_transformChanged =
                    modelMatrixChanged || (parentTransform && parentTransform->m_transformChanged);
            
            if(transform->m_transformChanged)
            {
                ownTransform.m_modelMatrix =
                        ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix * ownTransform.m_scaleMatrix;
                
                if(parentTransform)
                {
                    finalTransform.m_modelMatrix =
                            parentTransform->m_finalTransform.m_modelMatrix * ownTransform.m_modelMatrix;
                }
                else
                {
                    finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
                }
                
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
    
    m_changedModelMatrices.lock();
    m_entitiesForPhysicsUpdateToCheck.lock();
}

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    if(!m_sharedScene) return;
    
    if(!m_calculatedNotPhysicalEntitiesCopy.isLocked())
    {
        std::lock_guard guard(m_calculatedNotPhysicalEntitiesCopy);
        
        size_t i = 0;
        
        auto& entitiesCopy = m_calculatedNotPhysicalEntitiesCopy.getWrapped();
        for(const auto& t : entitiesCopy)
        {
            auto* tmpNonConstTransform = m_sharedScene->getECSRegistry()->try_get<Ref<Transform>>(t.m_owner);
            Ref<Transform> nonConstTransform = (tmpNonConstTransform ? *tmpNonConstTransform : nullptr);
            Mesh* mesh = m_sharedScene->getECSRegistry()->try_get<Mesh>(t.m_owner);
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
                
                finalTransform.m_aabb.calculateAABBFromTRS(translation, rotation, scale,
                                                           mesh->m_base.getMeshData()->m_aabb);
                
                finalTransform.m_position = translation;
                finalTransform.m_lastPosition = translation;
                
                finalTransform.m_rotation = rotation;
                finalTransform.m_lastRotation = rotation;
                
                finalTransform.m_scale = scale;
                finalTransform.m_lastScale = scale;
                
                ownTransform.m_aabb = finalTransform.m_aabb;
            }
            
            onTransformChanged(m_sharedScene->getECSRegistry(), t.m_owner, t.m_memberValue);
            
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
            auto* tmpNonConstTransform = m_sharedScene->getECSRegistry()->try_get<Ref<Transform>>(t.m_owner);
            Ref<Transform> nonConstTransform = (tmpNonConstTransform ? *tmpNonConstTransform : nullptr);
            Mesh* mesh = m_sharedScene->getECSRegistry()->try_get<Mesh>(t.m_owner);
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
                
                finalTransform.m_aabb.calculateAABBFromTRS(translation, rotation, scale,
                                                           mesh->m_base.getMeshData()->m_aabb);
                
                finalTransform.m_position = translation;
                finalTransform.m_lastPosition = translation;
                
                finalTransform.m_rotation = rotation;
                finalTransform.m_lastRotation = rotation;
                
                finalTransform.m_scale = scale;
                finalTransform.m_lastScale = scale;
                
                ownTransform.m_aabb = finalTransform.m_aabb;
            }
            
            onTransformChanged(m_sharedScene->getECSRegistry(), t.m_owner, t.m_memberValue);
        }
        
        entitiesCopy.clear();
    }
}
