//
// Created by stuka on 02.05.2023.
//

#include <glm/gtc/type_ptr.hpp>
#include "TransformationsUpdater.h"
#include "TransformBase.h"
#include "glm/gtx/rotate_vector.hpp"
#include "SGCore/Scene/Scene.h"
#include "TransformationsUtils.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Physics/Rigidbody3D.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    entt::registry& registry = lockedScene->getECSRegistry();
    
    auto transformsView = lockedScene->getECSRegistry().view<Transform>();

    transformsView.each([&registry](const entt::entity& entity, Transform& transform) {
        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        Transform* parentTransform = nullptr;

        if(entityBaseInfo)
        {
            parentTransform = registry.try_get<Transform>(entityBaseInfo->m_parent);
        }

        bool translationChanged = false;
        bool rotationChanged = false;
        bool scaleChanged = false;

        TransformBase& ownTransform = transform.m_ownTransform;
        TransformBase& finalTransform = transform.m_finalTransform;
        
        // translation ==============================================

        if(ownTransform.m_lastPosition != ownTransform.m_position)
        {
            ownTransform.m_translationMatrix = glm::translate(ownTransform.m_translationMatrix,
                                                              ownTransform.m_position -
                                                              ownTransform.m_lastPosition
            );
            ownTransform.m_lastPosition = ownTransform.m_position;

            translationChanged = true;
        }

        if(parentTransform && transform.m_followParentTRS.x && parentTransform->m_transformChanged)
        {
            finalTransform.m_translationMatrix = parentTransform->m_finalTransform.m_translationMatrix * ownTransform.m_translationMatrix;
        }
        else
        {
            if(transform.m_transformChanged)
            {
                finalTransform.m_translationMatrix = ownTransform.m_translationMatrix;
            }
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

        if(parentTransform && transform.m_followParentTRS.y && parentTransform->m_transformChanged)
        {
            finalTransform.m_rotationMatrix = parentTransform->m_finalTransform.m_rotationMatrix * ownTransform.m_rotationMatrix;
        }
        else
        {
            if(transform.m_transformChanged)
            {
                finalTransform.m_rotationMatrix = ownTransform.m_rotationMatrix;
            }
        }

        // scale ========================================================

        if(ownTransform.m_lastScale != ownTransform.m_scale)
        {
            ownTransform.m_scaleMatrix = glm::scale(ownTransform.m_scaleMatrix,
                                                    ownTransform.m_scale -
                                                    ownTransform.m_lastScale
            );
            ownTransform.m_lastScale = ownTransform.m_scale;

            scaleChanged = true;
        }

        if(parentTransform && transform.m_followParentTRS.z && parentTransform->m_transformChanged)
        {
            finalTransform.m_scaleMatrix = parentTransform->m_finalTransform.m_scaleMatrix * ownTransform.m_scaleMatrix;
        }
        else
        {
            if(transform.m_transformChanged)
            {
                finalTransform.m_scaleMatrix = ownTransform.m_scaleMatrix;
            }
        }

        // model matrix =================================================
        
        bool modelMatrixChanged = translationChanged || rotationChanged || scaleChanged;
        
        // О ТАК ВЕРНО
        transform.m_transformChanged = modelMatrixChanged || (parentTransform && parentTransform->m_transformChanged);
        
        if(transform.m_transformChanged)
        {
            registry.patch<Transform>(entity);
        }
        else
        {
            if(registry.any_of<Rigidbody3D>(entity))
            {
                registry.patch<Rigidbody3D>(entity);
            }
        }
    });

    m_transformUpdateObserver.each([&registry, this](const entt::entity& entity) {
        Transform* entityTransform = registry.try_get<Transform>(entity);
        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        Transform* entityParentTransform = nullptr;
        
        if(entityBaseInfo)
        {
            entityParentTransform = registry.try_get<Transform>(entityBaseInfo->m_parent);
        }
        
        if(entityTransform)
        {
            TransformBase& ownTransform = entityTransform->m_ownTransform;
            TransformBase& finalTransform = entityTransform->m_finalTransform;
            
            ownTransform.m_modelMatrix =
                    ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix * ownTransform.m_scaleMatrix;
            
            if(entityParentTransform)
            {
                finalTransform.m_modelMatrix =
                        entityParentTransform->m_finalTransform.m_modelMatrix * ownTransform.m_modelMatrix;
            }
            else
            {
                finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
            }
            
            Rigidbody3D* rigidbody3D = registry.try_get<Rigidbody3D>(entity);
            if(rigidbody3D)
            {
                btTransform initialTransform;
                initialTransform.setIdentity();
                initialTransform.setFromOpenGLMatrix((float*) &finalTransform.m_modelMatrix[0]);
                rigidbody3D->m_body->setWorldTransform(initialTransform);
                
                // rigidbody3D->m_body->getCollisionShape()->setLocalScaling({ scale.x, scale.y, scale.z });
            }
        }
    });
    
    m_transformUpdateObserver.clear();
    
    m_rigidbody3DUpdateObserver.each([&registry, this](const entt::entity& entity) {
        Transform* entityTransform = registry.try_get<Transform>(entity);
        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        Rigidbody3D* rigidbody3D = registry.try_get<Rigidbody3D>(entity);
        Transform* entityParentTransform = nullptr;
        
        if(entityBaseInfo)
        {
            entityParentTransform = registry.try_get<Transform>(entityBaseInfo->m_parent);
        }
        
        if(rigidbody3D && entityTransform)
        {
            btTransform& rigidbody3DTransform = rigidbody3D->m_body->getWorldTransform();
            TransformBase& ownTransform = entityTransform->m_ownTransform;
            TransformBase& finalTransform = entityTransform->m_finalTransform;
            
            // ============================
            
            float rigidbody3DMatrix[16];
            rigidbody3DTransform.getOpenGLMatrix(rigidbody3DMatrix);
            
            glm::mat4 glmRigidbody3DOwnModelMatrix = glm::make_mat4(rigidbody3DMatrix);
            
            if(entityParentTransform)
            {
                glmRigidbody3DOwnModelMatrix /= entityParentTransform->m_finalTransform.m_translationMatrix * entityParentTransform->m_finalTransform.m_rotationMatrix;
            }
            /*else
            {
                glmRigidbody3DOwnModelMatrix /= ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix;
            }*/
            
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            
            glm::decompose(glmRigidbody3DOwnModelMatrix, scale, rotation, translation, skew, perspective);
            
            ownTransform.m_position = translation;
            ownTransform.m_rotation = glm::degrees(glm::eulerAngles(rotation));
            // std::cout << "pos : " << ownTransform.m_position.x << ", " << ownTransform.m_position.y << ", " << ownTransform.m_position.z << std::endl;
            
            bool translationChanged = false;
            bool rotationChanged = false;
            
            if(ownTransform.m_position != ownTransform.m_lastPosition)
            {
                ownTransform.m_translationMatrix = glm::translate(glm::mat4(1.0), ownTransform.m_position);
                
                ownTransform.m_lastPosition = ownTransform.m_position;
                
                translationChanged = true;
            }
            
            if(ownTransform.m_rotation != ownTransform.m_lastRotation)
            {
                const float rotDifX = ownTransform.m_rotation.x - ownTransform.m_lastRotation.x;
                const float rotDifY = ownTransform.m_rotation.y - ownTransform.m_lastRotation.y;
                const float rotDifZ = ownTransform.m_rotation.z - ownTransform.m_lastRotation.z;
                
                // ownTransform.m_rotationMatrix = glm::toMat4(rotation);
                
                auto q = glm::identity<glm::quat>();
                
                // q = glm::quat(glm::radians(ownTransform.m_rotation));
                
                q = glm::rotate(q, glm::radians(ownTransform.m_rotation.z), { 0, 0, 1 });
                q = glm::rotate(q, glm::radians(ownTransform.m_rotation.y), { 0, 1, 0 });
                q = glm::rotate(q, glm::radians(ownTransform.m_rotation.x), { 1, 0, 0 });
                
                ownTransform.m_rotationMatrix = glm::toMat4(q);
                
                // rotating directions vectors
                ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                                  -glm::radians(rotDifY), glm::vec3(0, 1, 0));
                ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                                  -glm::radians(rotDifZ), glm::vec3(0, 0, 1));
                
                ownTransform.m_forward = glm::rotate(ownTransform.m_forward,
                                                     -glm::radians(rotDifX), glm::vec3(1, 0, 0));
                ownTransform.m_forward = glm::rotate(ownTransform.m_forward,
                                                     -glm::radians(rotDifY), glm::vec3(0, 1, 0));
                
                ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                                -glm::radians(rotDifX), glm::vec3(1, 0, 0));
                ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                                -glm::radians(rotDifZ), glm::vec3(0, 0, 1));
                
                ownTransform.m_lastRotation = ownTransform.m_rotation;
                
                rotationChanged = true;
            }

            entityTransform->m_transformChanged =
                    entityTransform->m_transformChanged || translationChanged || rotationChanged;
            
            if(entityTransform->m_transformChanged)
            {
                ownTransform.m_modelMatrix =
                        ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix * ownTransform.m_scaleMatrix;
                
                if(entityParentTransform)
                {
                    finalTransform.m_modelMatrix =
                            entityParentTransform->m_finalTransform.m_modelMatrix * ownTransform.m_modelMatrix;
                }
                else
                {
                    finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
                }
            }
        }
    });
    
    m_rigidbody3DUpdateObserver.clear();
}

void SGCore::TransformationsUpdater::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;

    m_transformUpdateObserver.connect(scene->getECSRegistry(), entt::basic_collector<>::update<Transform>());
    m_rigidbody3DUpdateObserver.connect(scene->getECSRegistry(), entt::basic_collector<>::update<Rigidbody3D>());
}
