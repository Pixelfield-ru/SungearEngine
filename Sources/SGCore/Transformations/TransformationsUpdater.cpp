//
// Created by stuka on 02.05.2023.
//

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

    auto transformsView = registry->view<EntityBaseInfo, Transform>();

    auto start = std::chrono::system_clock::now();

    // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) << " for clearing notTransformUpdatedEntities and copying updatedByPhysicsEntities: " << updatedByPhysicsEntities.size() << std::endl;

    // =======================================================================

    transformsView.each([&registry, this](
    ECS::entity_t entity,
    EntityBaseInfo& entityBaseInfo,
    const Transform::reg_t& transform) {
        // starting only on root entities
        if(entityBaseInfo.getParent() != entt::null) return;

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
    if(currentEntityTransform)
    {
        Ref<Rigidbody3D> rigidbody3D;

        {
            auto* tmpRigidbody3D = inRegistry->tryGet<Rigidbody3D>(currentEntity);
            rigidbody3D = tmpRigidbody3D ? *tmpRigidbody3D : nullptr;
        }

        TransformBase& finalTransform = currentEntityTransform->m_finalTransform;
        TransformBase& ownTransform = currentEntityTransform->m_ownTransform;

        bool isTransformChanged = false;
        // if(!rigidbody3D)
        {
            isTransformChanged = TransformUtils::calculateTransform(*currentEntityTransform, parentTransform.get());
        }

        // updating rigidbody3d =================================================

        if(rigidbody3D)
        {
            /*auto inversedParentTranslationMatrix = glm::mat4(1.0);
            auto inversedParentRotationMatrix = glm::mat4(1.0);
            auto inversedParentScaleMatrix = glm::mat4(1.0);

            // TransformUtils::calculateTransform(*transform, parentTransform.get());

            if(parentTransform)
            {
                inversedParentTranslationMatrix = glm::inverse(
                    parentTransform->m_finalTransform.m_translationMatrix);
                inversedParentRotationMatrix = glm::inverse(
                    parentTransform->m_finalTransform.m_rotationMatrix);
                inversedParentScaleMatrix = glm::inverse(
                    parentTransform->m_finalTransform.m_scaleMatrix);
            }

            glm::mat4 rigidbody3DMatrix = glm::mat4(1.0);

            rigidbody3D->m_body->getWorldTransform().getOpenGLMatrix(&rigidbody3DMatrix[0][0]);

            // getting entity`s only own transform from rigidbody3d
            rigidbody3DMatrix =
                            inversedParentScaleMatrix * inversedParentRotationMatrix *
                            inversedParentTranslationMatrix *
                            rigidbody3DMatrix;

            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;

            glm::decompose(rigidbody3DMatrix, scale, rotation, translation, skew,
                           perspective);*/

            auto& bodyTransform = rigidbody3D->m_body->getWorldTransform();

            const auto transformPosDif = finalTransform.m_position - rigidbody3D->m_lastPosition;
            auto transformRotDif = finalTransform.m_rotation * glm::inverse(glm::normalize(rigidbody3D->m_lastRotation));

            glm::vec3 localBodyPos = glm::vec3(bodyTransform.getOrigin().x(), bodyTransform.getOrigin().y(), bodyTransform.getOrigin().z());
            if(parentTransform)
            {
                // localBodyPos = glm::inverse(parentTransform->m_finalTransform.m_animatedModelMatrix) * glm::vec4(localBodyPos, 1.0f);
                localBodyPos = glm::inverse(parentTransform->m_finalTransform.m_rotation) * glm::vec4(localBodyPos, 1.0f);
            }

            glm::vec3 localLastBodyPos = rigidbody3D->m_lastPosition;
            if(parentTransform)
            {
                // localLastBodyPos = glm::inverse(parentTransform->m_finalTransform.m_animatedModelMatrix) * glm::vec4(localLastBodyPos, 1.0f);
                localLastBodyPos = glm::inverse(parentTransform->m_finalTransform.m_rotation) * glm::vec4(localLastBodyPos, 1.0f);
            }

            // calculating position delta of rigidbody (physical transform delta)
            const auto posDif = localBodyPos - localLastBodyPos;

            auto localBodyRot = glm::quat(bodyTransform.getRotation().w(), bodyTransform.getRotation().x(), bodyTransform.getRotation().y(), bodyTransform.getRotation().z());
            if(parentTransform)
            {
                localBodyRot = glm::inverse(parentTransform->m_finalTransform.m_rotation) * localBodyRot;
            }

            auto localLastBodyRot = rigidbody3D->m_lastRotation;
            if(parentTransform)
            {
                localLastBodyRot = glm::inverse(parentTransform->m_finalTransform.m_rotation) * localLastBodyRot;
            }

            // calculating rotation delta of rigidbody (physical transform delta)
            const auto rotDif = localBodyRot * glm::inverse(glm::normalize(localLastBodyRot));

            // applying transform component delta to rigidbody transform
            bodyTransform.setOrigin(bodyTransform.getOrigin() + btVector3(transformPosDif.x, transformPosDif.y, transformPosDif.z));
            bodyTransform.setRotation(btQuaternion(transformRotDif.x, transformRotDif.y, transformRotDif.z, transformRotDif.w) * bodyTransform.getRotation());

            // applying rigidbody transform delta to transform component (applying physics)
            ownTransform.m_position += posDif;
            ownTransform.m_rotation = rotDif * ownTransform.m_rotation;

            if(posDif != glm::vec3(0.0f) || rotDif != glm::identity<glm::quat>())
            {
                isTransformChanged = true;
            }

            // saving new rigidbody transform as last transform
            rigidbody3D->m_lastPosition = glm::vec3 { bodyTransform.getOrigin().x(), bodyTransform.getOrigin().y(), bodyTransform.getOrigin().z() };
            rigidbody3D->m_lastRotation = glm::quat { bodyTransform.getRotation().w(), bodyTransform.getRotation().x(), bodyTransform.getRotation().y(), bodyTransform.getRotation().z() };
        }

        // =====================================================================

        if(isTransformChanged)
        {
            onTransformChanged(inRegistry, currentEntity, currentEntityTransform);
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
