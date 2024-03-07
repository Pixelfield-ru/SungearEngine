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
#include "glm/gtx/string_cast.hpp"

SGCore::TransformationsUpdater::TransformationsUpdater()
{
    Ref<TimerCallback> callback = MakeRef<TimerCallback>();
    
    m_updaterTimer.setTargetFrameRate(60);
    m_updaterTimer.m_cyclic = true;
    m_updaterTimer.addCallback(callback);

    callback->setUpdateFunction([this](const double& dt, const double& fixedDt) {
        updateTransformations(dt, fixedDt);
    });
    
    /*m_updaterThread = std::thread([this, callback]() {
        while(m_isAlive)
        {
            if(m_active)
            {
               m_updaterTimer.startFrame();
            }
        }
    });*/
    
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
    
    m_transformUpdateObserver.connect(scene->getECSRegistry(), entt::basic_collector<>::update<Ref<Transform>>());
    m_rigidbody3DUpdateObserver.connect(scene->getECSRegistry(), entt::basic_collector<>::update<Rigidbody3D>());
}

void SGCore::TransformationsUpdater::updateTransformations(const double& dt, const double& fixedDt) noexcept
{
    if(!m_sharedScene) return;
    
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
            const float rotDifX = ownTransform.m_rotation.x - ownTransform.m_lastRotation.x;
            const float rotDifY = ownTransform.m_rotation.y - ownTransform.m_lastRotation.y;
            const float rotDifZ = ownTransform.m_rotation.z - ownTransform.m_lastRotation.z;
            
            auto q = glm::identity<glm::quat>();
            
            q = glm::rotate(q, glm::radians(ownTransform.m_rotation.z), { 0, 0, 1 });
            q = glm::rotate(q, glm::radians(ownTransform.m_rotation.y), { 0, 1, 0 });
            q = glm::rotate(q, glm::radians(ownTransform.m_rotation.x), { 1, 0, 0 });
            
            ownTransform.m_rotationMatrix = glm::toMat4(q);
            
            /*ownTransform.m_rotationMatrix = glm::rotate(glm::mat4(1.0),
                                                        -glm::radians(ownTransform.m_rotation.x),
                                                        glm::vec3(1, 0, 0));

            ownTransform.m_rotationMatrix = glm::rotate(ownTransform.m_rotationMatrix,
                                                        -glm::radians(ownTransform.m_rotation.y),
                                                        glm::vec3(0, 1, 0));
            
            ownTransform.m_rotationMatrix = glm::rotate(ownTransform.m_rotationMatrix,
                                                        -glm::radians(ownTransform.m_rotation.z),
                                                        glm::vec3(0, 0, 1));*/
            
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
            // registry.patch<Ref<Transform>>(entity);
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
            
            Rigidbody3D* rigidbody3D = registry.try_get<Rigidbody3D>(entity);
            if(rigidbody3D)
            {
                btTransform initialTransform;
                initialTransform.setIdentity();
                initialTransform.setFromOpenGLMatrix(glm::value_ptr(finalTransform.m_modelMatrix));
                rigidbody3D->m_body->setWorldTransform(initialTransform);
                
                // rigidbody3D->m_body->getCollisionShape()->setLocalScaling({ scale.x, scale.y, scale.z });
            }
        }
        else
        {
            if(registry.any_of<Rigidbody3D>(entity))
            {
                // registry.patch<Rigidbody3D>(entity);
                Rigidbody3D* rigidbody3D = registry.try_get<Rigidbody3D>(entity);
                if(rigidbody3D)
                {
                    btTransform& rigidbody3DTransform = rigidbody3D->m_body->getWorldTransform();
                    
                    // ============================
                    
                    float rigidbody3DMatrix[16];
                    rigidbody3DTransform.getOpenGLMatrix(rigidbody3DMatrix);
                    
                    glm::mat4 glmRigidbody3DOwnModelMatrix = glm::make_mat4(rigidbody3DMatrix);
                    
                    glm::mat4 inversedParentTranslationMatrix = glm::mat4(1.0);
                    glm::mat4 inversedParentRotationMatrix = glm::mat4(1.0);
                    glm::mat4 inversedParentScaleMatrix = glm::mat4(1.0);
                    if(parentTransform)
                    {
                        inversedParentTranslationMatrix = glm::inverse(parentTransform->m_finalTransform.m_translationMatrix);
                        inversedParentRotationMatrix = glm::inverse(parentTransform->m_finalTransform.m_rotationMatrix);
                        inversedParentScaleMatrix = glm::inverse(parentTransform->m_finalTransform.m_scaleMatrix);
                    }
                    
                    glmRigidbody3DOwnModelMatrix = inversedParentScaleMatrix * inversedParentRotationMatrix * inversedParentTranslationMatrix * glmRigidbody3DOwnModelMatrix;
                    
                    glm::vec3 scale;
                    glm::quat rotation;
                    glm::vec3 translation;
                    glm::vec3 skew;
                    glm::vec4 perspective;
                    
                    glm::decompose(glmRigidbody3DOwnModelMatrix, scale, rotation, translation, skew, perspective);
                    
                    ownTransform.m_position = translation;
                    ownTransform.m_rotation = glm::degrees(glm::eulerAngles(rotation));
                    // std::cout << "pos : " << ownTransform.m_position.x << ", " << ownTransform.m_position.y << ", " << ownTransform.m_position.z << std::endl;
                    
                    translationChanged = false;
                    rotationChanged = false;
                    
                    if(ownTransform.m_position != ownTransform.m_lastPosition)
                    {
                        ownTransform.m_translationMatrix = glm::translate(glm::mat4(1.0), ownTransform.m_position);
                        
                        ownTransform.m_lastPosition = ownTransform.m_position;
                        
                        translationChanged = true;
                    }
                    
                    if(ownTransform.m_rotation != ownTransform.m_lastRotation)
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
                        
                        ownTransform.m_lastRotation = ownTransform.m_rotation;
                        
                        rotationChanged = true;
                    }
                    
                    transform->m_transformChanged =
                            transform->m_transformChanged || translationChanged || rotationChanged;
                    
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
                    }
                }
            }
        }
    });
    
    /*m_transformUpdateObserver.each([&registry, this](const entt::entity& entity) {
        Ref<Transform>* tmpEntityTransform = registry.try_get<Ref<Transform>>(entity);
        Ref<Transform> entityTransform = (tmpEntityTransform ? *tmpEntityTransform : nullptr);
        
        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        
        Ref<Transform> entityParentTransform;
        
        if(entityBaseInfo)
        {
            Ref<Transform>* tmpEntityParentTransform = registry.try_get<Ref<Transform>>(entityBaseInfo->m_parent);
            entityParentTransform = (tmpEntityParentTransform ? *tmpEntityParentTransform : nullptr);
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
                initialTransform.setFromOpenGLMatrix(glm::value_ptr(finalTransform.m_modelMatrix));
                rigidbody3D->m_body->setWorldTransform(initialTransform);
                
                // rigidbody3D->m_body->getCollisionShape()->setLocalScaling({ scale.x, scale.y, scale.z });
            }
        }
    });
    
    m_transformUpdateObserver.clear();*/
    
    /*m_rigidbody3DUpdateObserver.each([&registry, this](const entt::entity& entity) {
        Ref<Transform>* tmpEntityTransform = registry.try_get<Ref<Transform>>(entity);
        Ref<Transform> entityTransform = (tmpEntityTransform ? *tmpEntityTransform : nullptr);
        
        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        Rigidbody3D* rigidbody3D = registry.try_get<Rigidbody3D>(entity);
        
        Ref<Transform> entityParentTransform;
        
        if(entityBaseInfo)
        {
            Ref<Transform>* tmpEntityParentTransform = registry.try_get<Ref<Transform>>(entityBaseInfo->m_parent);
            entityParentTransform = (tmpEntityParentTransform ? *tmpEntityParentTransform : nullptr);
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
                glm::mat4 inversed = glm::inverse(entityParentTransform->m_finalTransform.m_rotationMatrix) *
                        glm::inverse(entityParentTransform->m_finalTransform.m_translationMatrix);
                
                glmRigidbody3DOwnModelMatrix *= inversed;
            }
            
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
                ownTransform.m_translationMatrix = glm::translate(glm::identity<glm::mat4>(), ownTransform.m_position);
                
                ownTransform.m_lastPosition = ownTransform.m_position;
                
                translationChanged = true;
            }
            
            if(ownTransform.m_rotation != ownTransform.m_lastRotation)
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
                            ownTransform.m_modelMatrix * entityParentTransform->m_finalTransform.m_modelMatrix;
                }
                else
                {
                    finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
                }
            }
        }
    });
    
    m_rigidbody3DUpdateObserver.clear();*/
}

void SGCore::TransformationsUpdater::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    updateTransformations(dt, fixedDt);
    // dispatch transformations
    
    /*if(m_changedModelMatrices.isLocked())
    {
        for(EntityComponentMember<glm::mat4>& matrix : m_changedModelMatrices.getObject())
        {
            if(m_sharedScene->getECSRegistry().valid(matrix.m_owner))
            {
                (*m_transformChangedEvent)(m_sharedScene->getECSRegistry(), matrix.m_owner);
            }
        }
        
        m_changedModelMatrices.getObject().clear();
        m_changedModelMatrices.unlock();
    }
    
    if(m_modelMatricesChangedByPhysics.isLocked())
    {
        for(EntityComponentMember<glm::mat4>& matrix : m_modelMatricesChangedByPhysics.getObject())
        {
            if(m_sharedScene->getECSRegistry().valid(matrix.m_owner))
            {
                (*m_transformChangedEvent)(m_sharedScene->getECSRegistry(), matrix.m_owner);
            }
        }
        
        m_modelMatricesChangedByPhysics.getObject().clear();
        m_modelMatricesChangedByPhysics.unlock();
    }*/
}
