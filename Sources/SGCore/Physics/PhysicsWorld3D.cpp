//
// Created by ilya on 18.02.24.
//
#include "PhysicsWorld3D.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "PhysicsDebugDraw.h"
#include "Rigidbody3D.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Threading/WrappedObject.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/EntityBaseInfo.h"

SGCore::PhysicsWorld3D::PhysicsWorld3D()
{
    m_collisionConfig = MakeScope<btDefaultCollisionConfiguration>();
    m_collisionDispatcher = MakeScope<btCollisionDispatcher>(m_collisionConfig.get());
    m_overlappingPairCache = MakeScope<btDbvtBroadphase>();
    m_sequentialImpulseConstraintSolver = MakeScope<btSequentialImpulseConstraintSolver>();

    m_debugDraw = MakeScope<PhysicsDebugDraw>();

    m_dynamicsWorld = MakeScope<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(),
                                                         m_overlappingPairCache.get(),
                                                         m_sequentialImpulseConstraintSolver.get(),
                                                         m_collisionConfig.get());
    
    m_debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_dynamicsWorld->setDebugDrawer(m_debugDraw.get());
    
    m_dynamicsWorld->setGravity({ 0, -120.0, 0 });

    std::cout << "fdfdfd" << std::endl;

    m_thread->start();
}

void SGCore::PhysicsWorld3D::addBody(const SGCore::Ref<btRigidBody>& rigidBody) noexcept
{
    std::lock_guard guard(m_bodiesCountChangeMutex);
    m_dynamicsWorld->addRigidBody(rigidBody.get());
}

void SGCore::PhysicsWorld3D::removeBody(const Ref<btRigidBody>& rigidBody) noexcept
{
    std::lock_guard guard(m_bodiesCountChangeMutex);
    int num = rigidBody->getNumConstraintRefs();
    for(int j = 0; j < num; ++j)
    {
        m_dynamicsWorld->removeConstraint(rigidBody->getConstraintRef(0));
    }
    m_dynamicsWorld->removeRigidBody(rigidBody.get());
}

void SGCore::PhysicsWorld3D::parallelUpdate(const double& dt, const double& fixedDt) noexcept
{
    std::lock_guard guard(m_bodiesCountChangeMutex);
    
    auto lockedScene = m_scene.lock();

    if(lockedScene)
    {
        auto registry = lockedScene->getECSRegistry();
        
        Ref<TransformationsUpdater> transformationsUpdater = lockedScene->getSystem<TransformationsUpdater>();
        
        if(transformationsUpdater)
        {
            if(transformationsUpdater->m_changedModelMatrices.isLocked())
            {
                auto& transformations = transformationsUpdater->m_changedModelMatrices.getWrapped();
                for(const auto& val : transformations)
                {
                    Ref<Rigidbody3D>* tmpRigidbody3D = lockedScene->getECSRegistry()->try_get<Ref<Rigidbody3D>>(val.m_owner);
                    Ref<Rigidbody3D> rigidbody3D = (tmpRigidbody3D ? *tmpRigidbody3D : nullptr);
                    
                    if(rigidbody3D)
                    {
                        btTransform initialTransform;
                        initialTransform.setIdentity();
                        initialTransform.setFromOpenGLMatrix(glm::value_ptr(val.m_memberValue));
                        rigidbody3D->m_body->setWorldTransform(initialTransform);
                        
                        // rigidbody3D->m_body->getCollisionShape()->setLocalScaling({ scale.x, scale.y, scale.z });
                    }
                }
                
                transformations.clear();
                transformationsUpdater->m_changedModelMatrices.unlock();
            }
            
            if(transformationsUpdater->m_entitiesForPhysicsUpdateToCheck.isLocked())
            {
                auto& entities = transformationsUpdater->m_entitiesForPhysicsUpdateToCheck.getWrapped();
                for(const auto& entity : entities)
                {
                    if(registry->any_of<Ref<Rigidbody3D>>(entity))
                    {
                        Ref<Rigidbody3D> rigidbody3D = registry->get<Ref<Rigidbody3D>>(entity);
                        Ref<Transform> transform;
                        {
                            Ref<Transform>* tmpTransform = registry->try_get<Ref<Transform>>(entity);
                            transform = (tmpTransform ? *tmpTransform : nullptr);
                        }
                        
                        if(rigidbody3D && transform)
                        {
                            TransformBase& ownTransform = transform->m_ownTransform;
                            TransformBase& finalTransform = transform->m_finalTransform;
                            
                            EntityBaseInfo* entityBaseInfo = registry->try_get<EntityBaseInfo>(entity);
                            Ref<Transform> parentTransform;
                            
                            if(entityBaseInfo)
                            {
                                auto* tmp = registry->try_get<Ref<Transform>>(entityBaseInfo->m_parent);
                                parentTransform = (tmp ? *tmp : nullptr);
                            }
                            
                            btTransform& rigidbody3DTransform = rigidbody3D->m_body->getWorldTransform();
                            
                            // ============================
                            
                            float rigidbody3DMatrix[16];
                            rigidbody3DTransform.getOpenGLMatrix(rigidbody3DMatrix);
                            
                            glm::mat4 glmRigidbody3DOwnModelMatrix = glm::make_mat4(rigidbody3DMatrix);
                            
                            auto inversedParentTranslationMatrix = glm::mat4(1.0);
                            auto inversedParentRotationMatrix = glm::mat4(1.0);
                            auto inversedParentScaleMatrix = glm::mat4(1.0);
                            
                            if(parentTransform)
                            {
                                inversedParentTranslationMatrix = glm::inverse(
                                        parentTransform->m_finalTransform.m_translationMatrix);
                                inversedParentRotationMatrix = glm::inverse(
                                        parentTransform->m_finalTransform.m_rotationMatrix);
                                inversedParentScaleMatrix = glm::inverse(
                                        parentTransform->m_finalTransform.m_scaleMatrix);
                            }
                            
                            glmRigidbody3DOwnModelMatrix =
                                    inversedParentScaleMatrix * inversedParentRotationMatrix *
                                    inversedParentTranslationMatrix *
                                    glmRigidbody3DOwnModelMatrix;
                            
                            glm::vec3 scale;
                            glm::quat rotation;
                            glm::vec3 translation;
                            glm::vec3 skew;
                            glm::vec4 perspective;
                            
                            glm::decompose(glmRigidbody3DOwnModelMatrix, scale, rotation, translation, skew,
                                           perspective);
                            
                            ownTransform.m_position = translation;
                            ownTransform.m_rotation = glm::degrees(glm::eulerAngles(rotation));
                            // std::cout << "pos : " << ownTransform.m_position.x << ", " << ownTransform.m_position.y << ", " << ownTransform.m_position.z << std::endl;
                            
                            bool translationChanged = false;
                            bool rotationChanged = false;
                            
                            if(ownTransform.m_position != ownTransform.m_lastPosition)
                            {
                                ownTransform.m_translationMatrix = glm::translate(glm::mat4(1.0),
                                                                                  ownTransform.m_position);
                                
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
                                                                  glm::radians(-ownTransform.m_rotation.y),
                                                                  glm::vec3(0, 1, 0));
                                ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                                                  glm::radians(-ownTransform.m_rotation.z),
                                                                  glm::vec3(0, 0, 1));
                                
                                ownTransform.m_left *= -1.0f;
                                
                                ownTransform.m_forward = glm::rotate(MathUtils::forward3,
                                                                     glm::radians(-ownTransform.m_rotation.x),
                                                                     glm::vec3(1, 0, 0));
                                ownTransform.m_forward = glm::rotate(ownTransform.m_forward,
                                                                     glm::radians(-ownTransform.m_rotation.y),
                                                                     glm::vec3(0, 1, 0));
                                
                                ownTransform.m_forward *= -1.0f;
                                
                                ownTransform.m_up = glm::rotate(MathUtils::up3,
                                                                glm::radians(-ownTransform.m_rotation.x),
                                                                glm::vec3(1, 0, 0));
                                ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                                                glm::radians(-ownTransform.m_rotation.z),
                                                                glm::vec3(0, 0, 1));
                                
                                ownTransform.m_up *= -1.0f;
                                
                                ownTransform.m_lastRotation = ownTransform.m_rotation;
                                
                                rotationChanged = true;
                            }
                            
                            transform->m_transformChanged =
                                    transform->m_transformChanged || translationChanged || rotationChanged;
                            
                            if(transform->m_transformChanged)
                            {
                                ownTransform.m_modelMatrix =
                                        ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix *
                                        ownTransform.m_scaleMatrix;
                                
                                if(parentTransform)
                                {
                                    finalTransform.m_modelMatrix =
                                            parentTransform->m_finalTransform.m_modelMatrix *
                                            ownTransform.m_modelMatrix;
                                }
                                else
                                {
                                    finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
                                }
                                
                                transformationsUpdater->m_calculatedPhysicalEntities.getWrapped().push_back({ entity, transform });
                            }
                        }
                    }
                }
                
                if(transformationsUpdater->m_canCopyEntities)
                {
                    transformationsUpdater->m_canCopyEntities = false;
                    if(transformationsUpdater->m_calculatedPhysicalEntitiesCopy.empty())
                    {
                        transformationsUpdater->m_calculatedPhysicalEntitiesCopy = transformationsUpdater->m_calculatedPhysicalEntities.getWrapped();
                        transformationsUpdater->m_calculatedPhysicalEntities.getWrapped().clear();
                    }
                    transformationsUpdater->m_canCopyEntities = true;
                }
                
                transformationsUpdater->m_entitiesForPhysicsUpdateToCheck.getWrapped().clear();
                transformationsUpdater->m_entitiesForPhysicsUpdateToCheck.unlock();
            }
        }
    }
    
    m_dynamicsWorld->stepSimulation(dt, 12, dt);
}

void SGCore::PhysicsWorld3D::update(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = m_scene.lock();
    if(lockedScene && m_debugDraw->getDebugMode() != btIDebugDraw::DBG_NoDebug)
    {
        // if(m_bodiesToAdd.getObject().empty() && m_bodiesToRemove.getObject().empty())
        {
            m_dynamicsWorld->debugDrawWorld();
        }
        m_debugDraw->drawAll(lockedScene);
    }
}

void SGCore::PhysicsWorld3D::onAddToScene(const Ref<Scene>& scene)
{
    if(!scene) return;
    
    auto rigidbodies3DView = scene->getECSRegistry()->view<Ref<Rigidbody3D>>();
    
    rigidbodies3DView.each([this](Ref<Rigidbody3D> rigidbody3D) {
        this->addBody(rigidbody3D->m_body);
    });
}

void SGCore::PhysicsWorld3D::onRemoveFromScene(const Ref<Scene>& scene)
{

}
