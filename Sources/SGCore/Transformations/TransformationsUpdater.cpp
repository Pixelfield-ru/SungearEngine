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
    if(currentEntityTransform && currentEntityBaseInfo.m_isActive)
    {
        Ref<Rigidbody3D> rigidbody3D;

        {
            auto* tmpRigidbody3D = inRegistry->tryGet<Rigidbody3D>(currentEntity);
            rigidbody3D = tmpRigidbody3D ? *tmpRigidbody3D : nullptr;
        }

        TransformBase& finalTransform = currentEntityTransform->m_finalTransform;
        TransformBase& ownTransform = currentEntityTransform->m_ownTransform;

        bool isTransformChanged = false;

        // updating rigidbody3d =================================================

        /*if(rigidbody3D)
        {
            auto& bodyTransform = rigidbody3D->m_body->getWorldTransform();

            auto transformPosDif = finalTransform.m_position - rigidbody3D->m_lastPosition;
            auto transformRotDif = finalTransform.m_rotation * glm::inverse(rigidbody3D->m_lastRotation);

            auto localBodyPos = glm::vec3(bodyTransform.getOrigin().x(), bodyTransform.getOrigin().y(), bodyTransform.getOrigin().z());
            if(parentTransform)
            {
                localBodyPos = glm::inverse(parentTransform->m_finalTransform.m_rotation) * glm::vec4(localBodyPos, 1.0f);
            }

            auto localLastBodyPos = rigidbody3D->m_lastPosition;
            if(parentTransform)
            {
                localLastBodyPos = glm::inverse(parentTransform->m_finalTransform.m_rotation) * glm::vec4(localLastBodyPos, 1.0f);
            }

            // calculating position delta of rigidbody (physical transform delta)
            auto posDif = localBodyPos - localLastBodyPos;

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
            auto rotDif = localBodyRot * glm::inverse(localLastBodyRot);

            // applying transform component delta to rigidbody transform
            bodyTransform.setOrigin(bodyTransform.getOrigin() + btVector3(transformPosDif.x, transformPosDif.y, transformPosDif.z));
            bodyTransform.setRotation(btQuaternion(transformRotDif.x, transformRotDif.y, transformRotDif.z, transformRotDif.w) * bodyTransform.getRotation());

            // applying rigidbody transform delta to transform component (applying physics)
            ownTransform.m_position += posDif;
            ownTransform.m_rotation = rotDif * ownTransform.m_rotation;

            if(parentTransform)
            {
                const glm::vec3 finalGlobalPos = parentTransform->m_finalTransform.m_animatedModelMatrix * glm::vec4(ownTransform.m_position, 1.0f);

                // ищем локальную позицию относительно парента. используем глобальные позиции для этого, так как они уже включают все повороты
                const auto finalLocalPos = (finalGlobalPos - parentTransform->m_finalTransform.m_position);
                // теперь найденной локальной позиции вычитаем локальную позицию, но повёрнутую в противоположную сторону от физического поворота парента
                // т.е. возвращаем локальную позицию в исходное положение (получаем оффсет для этого)
                const auto rotatedTranslationCancellation = glm::inverse(parentTransform->m_finalTransform.m_rotation) * (finalLocalPos - glm::inverse(parentTransform->m_physicalDeltaRotation) * finalLocalPos) / parentTransform->m_finalTransform.m_scale;
                const glm::vec3 translationCancellation = (glm::inverse(parentTransform->m_finalTransform.m_rotation) * parentTransform->m_physicalDeltaTranslation) / parentTransform->m_finalTransform.m_scale;
                const auto rotationCancellation = glm::inverse(parentTransform->m_physicalDeltaRotation);

                std::println(std::cout, "name: {}, rotationCancellation: {}, finalLocalPos: {}, own pos: {}",
                             currentEntityBaseInfo.getName(), glm::to_string(rotationCancellation),
                             glm::to_string(finalLocalPos), glm::to_string(ownTransform.m_position));

                // вычитаем оффсет
                ownTransform.m_position -= rotatedTranslationCancellation;
                ownTransform.m_position -= translationCancellation;
                ownTransform.m_rotation = rotationCancellation * ownTransform.m_rotation;
            }

            currentEntityTransform->m_physicalDeltaTranslation = posDif;
            currentEntityTransform->m_physicalDeltaRotation = rotDif;

            if(posDif != glm::vec3(0.0f) || rotDif != glm::identity<glm::quat>())
            {
                isTransformChanged = true;
            }

            // saving new rigidbody transform as last transform
            rigidbody3D->m_lastPosition = glm::vec3 { bodyTransform.getOrigin().x(), bodyTransform.getOrigin().y(), bodyTransform.getOrigin().z() };
            rigidbody3D->m_lastRotation = glm::quat { bodyTransform.getRotation().w(), bodyTransform.getRotation().x(), bodyTransform.getRotation().y(), bodyTransform.getRotation().z() };
        }
        else if(parentTransform)
        {
            currentEntityTransform->m_physicalDeltaTranslation = parentTransform->m_physicalDeltaTranslation;
            currentEntityTransform->m_physicalDeltaRotation = parentTransform->m_physicalDeltaRotation;
        }*/

        // =====================================================================

        // if(!rigidbody3D)
        {
            isTransformChanged |= TransformUtils::calculateTransform(*currentEntityTransform, parentTransform.get(), rigidbody3D.get());
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
