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
    Ref<TimerCallback> callback = MakeRef<TimerCallback>();
    
    m_updaterTimer.setTargetFrameRate(80);
    m_updaterTimer.m_cyclic = true;
    m_updaterTimer.addCallback(callback);

    callback->setUpdateFunction([this](const double& dt, const double& fixedDt) {
        updateTransformations(dt, fixedDt);
    });
    
    m_updaterThread = std::thread([this, callback]() {
        while(m_isAlive)
        {
            if(m_active)
            {
               m_updaterTimer.startFrame();
            }
        }
    });
    
    // m_updaterThread.detach();
}

SGCore::TransformationsUpdater::~TransformationsUpdater()
{
    // m_scene;
    m_isAlive = false;
    m_updaterThread.join();
}

void SGCore::TransformationsUpdater::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;
    m_sharedScene = scene;
}

void SGCore::TransformationsUpdater::updateTransformations(const double& dt, const double& fixedDt) noexcept
{
    if(!m_sharedScene) return;

    if(m_changedModelMatrices.isLocked() || m_entitiesForPhysicsUpdateToCheck.isLocked()) return;

    entt::registry& registry = m_sharedScene->getECSRegistry();
    
    auto transformsView = registry.view<Ref<Transform>>();
    
    transformsView.each([&registry, this](const entt::entity& entity, Ref<Transform>& transform) {
        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        Ref<Transform> parentTransform;
        
        if(entityBaseInfo)
        {
            auto* tmp = registry.try_get<Ref<Transform>>(entityBaseInfo->m_parent);
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
            auto q = glm::identity<glm::quat>();
            
            q = glm::rotate(q, glm::radians(ownTransform.m_rotation.z), { 0, 0, 1 });
            q = glm::rotate(q, glm::radians(ownTransform.m_rotation.y), { 0, 1, 0 });
            q = glm::rotate(q, glm::radians(ownTransform.m_rotation.x), { 1, 0, 0 });
            
            ownTransform.m_rotationMatrix = glm::toMat4(q);
            
            // rotating directions vectors
            ownTransform.m_left = glm::rotate(MathUtils::left3,
                                              glm::radians(-ownTransform.m_rotation.y), glm::vec3(0, 1, 0));
            ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                              glm::radians(-ownTransform.m_rotation.z), glm::vec3(0, 0, 1));
            
            ownTransform.m_left *= -1.0f;
            
            ownTransform.m_forward = glm::rotate(MathUtils::forward3,
                                                 glm::radians(-ownTransform.m_rotation.x), glm::vec3(1, 0, 0));
            ownTransform.m_forward = glm::rotate(ownTransform.m_forward,
                                                 glm::radians(-ownTransform.m_rotation.y), glm::vec3(0, 1, 0));
            
            ownTransform.m_forward *= -1.0f;
            
            ownTransform.m_up = glm::rotate(MathUtils::up3,
                                            glm::radians(-ownTransform.m_rotation.x), glm::vec3(1, 0, 0));
            ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                            glm::radians(-ownTransform.m_rotation.z), glm::vec3(0, 0, 1));
            
            ownTransform.m_up *= -1.0f;
            
            //transformComponent->m_rotationMatrix = glm::toMat4(rotQuat);
            
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
            finalTransform.m_scaleMatrix = parentTransform->m_finalTransform.m_scaleMatrix * ownTransform.m_scaleMatrix;
        }
        else
        {
            finalTransform.m_scaleMatrix = ownTransform.m_scaleMatrix;
        }
        
        // model matrix =================================================
        
        bool modelMatrixChanged = translationChanged || rotationChanged || scaleChanged;
        
        // О ТАК ВЕРНО
        transform->m_transformChanged = modelMatrixChanged || (parentTransform && parentTransform->m_transformChanged);
        
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

            m_changedModelMatrices.getObject().push_back({ entity, finalTransform.m_modelMatrix });
            m_calculatedNotPhysicalEntities.getObject().m_vector.push_back({ entity, transform });
        }
        else
        {
            m_entitiesForPhysicsUpdateToCheck.getObject().push_back(entity);
        }
    });

    m_changedModelMatrices.lock();
    m_entitiesForPhysicsUpdateToCheck.lock();
    
    m_calculatedNotPhysicalEntities.getObject().m_vectorLastSize.store(m_calculatedNotPhysicalEntities.getObject().m_vector.size());
}

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    // updateTransformations(dt, fixedDt);
    // dispatch transformations
    
    if(!m_sharedScene) return;
    
    const size_t calculatedNotPhysEntitiesLastSize = m_calculatedNotPhysicalEntities.getObject().m_vectorLastSize.load();
    if(calculatedNotPhysEntitiesLastSize > 0)
    {
        auto& vec = m_calculatedNotPhysicalEntities.getObject().m_vector;
        for(size_t i = 0; i < calculatedNotPhysEntitiesLastSize; ++i)
        {
            auto* tmpNonConstTransform = m_sharedScene->getECSRegistry().try_get<Ref<Transform>>(vec[i].m_owner);
            Ref<Transform> nonConstTransform = (tmpNonConstTransform ? *tmpNonConstTransform : nullptr);
            Mesh* mesh = m_sharedScene->getECSRegistry().try_get<Mesh>(vec[i].m_owner);
            if(nonConstTransform && mesh)
            {
                auto& finalTransform = nonConstTransform->m_finalTransform;
                auto& ownTransform = nonConstTransform->m_ownTransform;
                
                ownTransform.m_aabbMin = mesh->m_base.m_meshData->m_aabbMin;
                ownTransform.m_aabbMax = mesh->m_base.m_meshData->m_aabbMax;
                
                glm::vec3& min = ownTransform.m_aabbMin;
                glm::vec3& max = ownTransform.m_aabbMax;
                
                glm::vec3 points[8] = {
                        { min },
                        { min.x, min.y, max.z },
                        { min.x, max.y, max.z },
                        { min.x, max.y, min.z },
                        
                        { max.x, min.y, min.z },
                        { max.x, max.y, min.z },
                        { max },
                        { max.x, min.y, max.z }
                };
                
                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;
                glm::vec3 skew;
                glm::vec4 perspective;
                
                glm::decompose(finalTransform.m_modelMatrix, scale, rotation, translation, skew,
                               perspective);
                
                glm::vec3 eulerRot = glm::eulerAngles(rotation);
                
                for(auto& point : points)
                {
                    point *= scale;
                    point = rotation * glm::vec4(point, 1.0);
                    point += translation;
                }
                
                min = points[0];
                max = points[0];
                
                for(const auto& point : points)
                {
                    if(point.x < min.x)
                    {
                        min.x = point.x;
                    }
                    if(point.y < min.y)
                    {
                        min.y = point.y;
                    }
                    if(point.z < min.z)
                    {
                        min.z = point.z;
                    }
                }
                
                for(const auto& point : points)
                {
                    if(point.x > max.x)
                    {
                        max.x = point.x;
                    }
                    if(point.y > max.y)
                    {
                        max.y = point.y;
                    }
                    if(point.z > max.z)
                    {
                        max.z = point.z;
                    }
                }
            }
            
            (*m_transformChangedEvent)(m_sharedScene->getECSRegistry(), vec[i].m_owner, vec[i].m_memberValue);
        }
        // std::cout << "calculatedNotPhysEntitiesLastSize - 1: " << (calculatedNotPhysEntitiesLastSize - 1) << std::endl;
        
        vec.erase(vec.begin(), vec.begin() + calculatedNotPhysEntitiesLastSize - 1);
        m_calculatedNotPhysicalEntities.getObject().m_vectorLastSize.store(0);
    }
    
    const size_t calculatedPhysEntitiesLastSize = m_calculatedPhysicalEntities.getObject().m_vectorLastSize.load();
    if(calculatedPhysEntitiesLastSize > 0)
    {
        auto& vec = m_calculatedPhysicalEntities.getObject().m_vector;
        for(size_t i = 0; i < calculatedPhysEntitiesLastSize; ++i)
        {
            auto* tmpNonConstTransform = m_sharedScene->getECSRegistry().try_get<Ref<Transform>>(vec[i].m_owner);
            Ref<Transform> nonConstTransform = (tmpNonConstTransform ? *tmpNonConstTransform : nullptr);
            Mesh* mesh = m_sharedScene->getECSRegistry().try_get<Mesh>(vec[i].m_owner);
            if(nonConstTransform && mesh)
            {
                auto& finalTransform = nonConstTransform->m_finalTransform;
                auto& ownTransform = nonConstTransform->m_ownTransform;
                
                ownTransform.m_aabbMin = mesh->m_base.m_meshData->m_aabbMin;
                ownTransform.m_aabbMax = mesh->m_base.m_meshData->m_aabbMax;
                
                glm::vec3& min = ownTransform.m_aabbMin;
                glm::vec3& max = ownTransform.m_aabbMax;
                
                glm::vec3 points[8] = {
                        { min },
                        { min.x, min.y, max.z },
                        { min.x, max.y, max.z },
                        { min.x, max.y, min.z },
                        
                        { max.x, min.y, min.z },
                        { max.x, max.y, min.z },
                        { max },
                        { max.x, min.y, max.z }
                };
                
                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;
                glm::vec3 skew;
                glm::vec4 perspective;
                
                glm::decompose(finalTransform.m_modelMatrix, scale, rotation, translation, skew,
                               perspective);
                
                glm::vec3 eulerRot = glm::eulerAngles(rotation);
                
                for(auto& point : points)
                {
                    point *= scale;
                    point = rotation * glm::vec4(point, 1.0);
                    point += translation;
                }
                
                min = points[0];
                max = points[0];
                
                for(const auto& point : points)
                {
                    if(point.x < min.x)
                    {
                        min.x = point.x;
                    }
                    if(point.y < min.y)
                    {
                        min.y = point.y;
                    }
                    if(point.z < min.z)
                    {
                        min.z = point.z;
                    }
                }
                
                for(const auto& point : points)
                {
                    if(point.x > max.x)
                    {
                        max.x = point.x;
                    }
                    if(point.y > max.y)
                    {
                        max.y = point.y;
                    }
                    if(point.z > max.z)
                    {
                        max.z = point.z;
                    }
                }
            }
            
            (*m_transformChangedEvent)(m_sharedScene->getECSRegistry(), vec[i].m_owner, vec[i].m_memberValue);
        }
        vec.erase(vec.begin(), vec.begin() + calculatedPhysEntitiesLastSize - 1);
        m_calculatedPhysicalEntities.getObject().m_vectorLastSize.store(0);
    }
}
