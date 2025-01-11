//
// Created by stuka on 07.01.2025.
//

#include "MotionPlannersResolver.h"
#include "SGCore/Scene/Scene.h"
#include "MotionPlannerConnection.h"
#include "SGCore/Render/Mesh.h"

// omg

void SGCore::MotionPlannersResolver::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto registry = getScene()->getECSRegistry();

    auto motionPlannersView = registry->view<EntityBaseInfo, Transform, MotionPlanner>();

    motionPlannersView.each([dt, &registry](const ECS::entity_t& entity,
                                            const EntityBaseInfo::reg_t& entityBaseInfo,
                                            const Transform::reg_t& transform,
                                            MotionPlanner::reg_t& motionPlanner) {
        auto skeleton = motionPlanner.m_skeleton;

        std::vector<Ref<MotionPlannerNode>> nodesToInterpolate;

        // collecting all nodes to interpolate bones animations
        for(const auto& rootNode : motionPlanner.m_rootNodes)
        {
            if(rootNode->m_isActive && rootNode->activationFunction())
            {
                rootNode->m_isPlaying = true;
                nodesToInterpolate.push_back(rootNode);

                collectNodesToInterpolate(rootNode, nodesToInterpolate);
            }
        }


        std::int32_t updatedBonesCount = 0;

        if(!nodesToInterpolate.empty())
        {

            // animating bones
            processMotionNodes(dt,
                               registry,
                               motionPlanner,
                               nodesToInterpolate,
                               motionPlanner.m_skeleton,
                               entity,
                               entityBaseInfo,
                               transform,
                               nullptr,
                               updatedBonesCount);
        }

        // then updating all collected nodes
        for(const auto& node : nodesToInterpolate)
        {
            if(node->m_isPlaying)
            {
                node->m_currentAnimationTime +=
                        dt *
                        node->m_skeletalAnimation->m_ticksPerSecond *
                        node->m_animationSpeed;
            }

            // if animation has ended
            if(node->m_currentAnimationTime >= node->m_skeletalAnimation->m_duration)
            {
                if(node->m_isRepeated)
                {
                    node->m_currentAnimationTime = 0.0f;
                }
                else
                {
                    node->m_currentAnimationTime = node->m_skeletalAnimation->m_duration - 1;
                    node->m_isPaused = true;
                }
            }

            node->m_isPlaying = !node->m_isPaused;
        }

        // updating count of bones in data vector
        motionPlanner.m_bonesMatricesData[0] = updatedBonesCount;

        // updating data in texture buffer after updating bones matrices
        motionPlanner.m_bonesMatricesBuffer->bind(0);
        motionPlanner.m_bonesMatricesBuffer->subTextureBufferData(motionPlanner.m_bonesMatricesData.data(),
                                                                  motionPlanner.m_bonesMatricesData.size(), 0);
    });
}

void SGCore::MotionPlannersResolver::processMotionNodes(const double& dt,
                                                        SGCore::Ref<SGCore::ECS::registry_t>& inRegistry,
                                                        MotionPlanner& motionPlanner,
                                                        const std::vector<Ref<MotionPlannerNode>>& nodesToInterpolate,
                                                        const AssetRef<Skeleton>& skeleton,
                                                        const ECS::entity_t& currentEntity,
                                                        const EntityBaseInfo& currentEntityBaseInfo,
                                                        const Transform::reg_t& currentEntityTransform,
                                                        const Transform::reg_t& parentEntityTransform,
                                                        std::int32_t& bonesCount) noexcept
{
    Mesh::reg_t* currentEntityMesh = inRegistry->tryGet<Mesh>(currentEntity);

    std::vector<SkeletalBoneAnimation*> currentBoneAnims;
    auto currentBone = skeleton->findBone(currentEntityBaseInfo.getRawName());

    if(currentBone)
    {
        ++bonesCount;

        // collecting all animations for currentBone
        for(const auto& node : nodesToInterpolate)
        {
            SkeletalBoneAnimation* boneAnimation = node->m_skeletalAnimation->getBoneAnimation(currentBone->m_boneName);
            if(boneAnimation)
            {
                currentBoneAnims.push_back(boneAnimation);
            }
            else
            {
                currentBoneAnims.push_back(nullptr);
                /*LOG_W(SGCORE_TAG, "MotionPlannersResolver: Can not find bone '{}' in skeletal animation '{}'!",
                      currentBone->m_boneName, node->m_skeletalAnimation->m_animationName);*/
            }
        }

        std::vector<glm::vec3> positionsToInterpolate;
        std::vector<glm::quat> rotationsToInterpolate;
        std::vector<glm::vec3> scalesToInterpolate;

        // GUARANTEED THAT currentBoneAnims.size() == nodesToInterpolate.size()
        // calculating all transformations for all animations using currentBone and keys from currentBoneAnims
        for(size_t i = 0; i < currentBoneAnims.size(); ++i)
        {
            auto& currentNodeToInterpolate = nodesToInterpolate[i];
            auto* currentBoneAnim = currentBoneAnims[i];

            float curAnimTime = currentNodeToInterpolate->m_currentAnimationTime;

            // calculating interpolated position for bone
            glm::vec3 interpolatedPosition = { 0.0, 0.0, 0.0 };

            if(currentBoneAnim)
            {
                const std::int64_t nextKeyIndex = currentBoneAnim->findPositionKeyByTime(curAnimTime);
                if(nextKeyIndex != -1)
                {
                    KeyPosition& nextKey = currentBoneAnim->m_positionKeys[nextKeyIndex];
                    KeyPosition prevKey;
                    if(nextKeyIndex - 1 >= 0)
                    {
                        prevKey = currentBoneAnim->m_positionKeys[nextKeyIndex - 1];

                        float normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);

                        interpolatedPosition = glm::lerp(prevKey.m_position, nextKey.m_position, normalizedTime);
                    }
                    else
                    {
                        interpolatedPosition = nextKey.m_position;
                    }
                }
            }

            positionsToInterpolate.push_back(interpolatedPosition);

            // calculating interpolated rotation for bone
            auto interpolatedRotation = glm::identity<glm::quat>();

            if(currentBoneAnim)
            {
                const std::int64_t nextKeyIndex = currentBoneAnim->findRotationKeyByTime(curAnimTime);
                if(nextKeyIndex != -1)
                {
                    const KeyRotation& nextKey = currentBoneAnim->m_rotationKeys[nextKeyIndex];
                    KeyRotation prevKey;
                    if(nextKeyIndex - 1 >= 0)
                    {
                        prevKey = currentBoneAnim->m_rotationKeys[nextKeyIndex - 1];

                        float normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);

                        interpolatedRotation = glm::normalize(glm::slerp(prevKey.m_rotation, nextKey.m_rotation, normalizedTime));
                    }
                    else
                    {
                        interpolatedRotation = nextKey.m_rotation;
                    }
                }
            }

            rotationsToInterpolate.push_back(interpolatedRotation);

            // calculating interpolated scale for bone
            glm::vec3 interpolatedScale = { 1.0, 1.0, 1.0 };

            if(currentBoneAnim)
            {
                const std::int64_t nextKeyIndex = currentBoneAnim->findScaleKeyByTime(curAnimTime);
                if(nextKeyIndex != -1)
                {
                    const KeyScale& nextKey = currentBoneAnim->m_scaleKeys[nextKeyIndex];
                    KeyScale prevKey;
                    if(nextKeyIndex - 1 >= 0)
                    {
                        prevKey = currentBoneAnim->m_scaleKeys[nextKeyIndex - 1];

                        float normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);

                        interpolatedScale = glm::lerp(prevKey.m_scale, nextKey.m_scale, normalizedTime);
                    }
                    else
                    {
                        interpolatedScale = nextKey.m_scale;
                    }
                }
            }

            scalesToInterpolate.push_back(interpolatedScale);
        }

        glm::vec3 interpolatedPosition = { 0, 0, 0 };
        auto interpolatedRotation = glm::identity<glm::quat>();
        glm::vec3 interpolatedScale = { 1, 1, 1 };

        // GUARANTEED THAT currentBoneAnims.size() ==
        // nodesToInterpolate.size() ==
        // positionsToInterpolate.size() ==
        // rotationsToInterpolate.size() == scalesToInterpolate.size()
        // interpolating all nodes transform components to find animated matrix
        if(!nodesToInterpolate.empty())
        {
            interpolatedPosition = positionsToInterpolate[0];
            interpolatedRotation = rotationsToInterpolate[0];
            interpolatedScale = scalesToInterpolate[0];

            for(size_t i = 1; i < nodesToInterpolate.size(); ++i)
            {
                interpolatedPosition = glm::lerp(interpolatedPosition, positionsToInterpolate[i], 0.5f);
                interpolatedRotation = glm::slerp(interpolatedRotation, rotationsToInterpolate[i], 0.5f);
                interpolatedScale = glm::lerp(interpolatedScale, scalesToInterpolate[i], 0.5f);
            }
        }

        // constructing animated matrix from interpolated transform components of all animations nodes
        auto animatedMatrix = glm::identity<glm::mat4>();

        animatedMatrix = glm::translate(animatedMatrix, interpolatedPosition);
        animatedMatrix *= glm::toMat4(interpolatedRotation);
        animatedMatrix = glm::scale(animatedMatrix, interpolatedScale);

        // if current entity has parent with bone
        if(parentEntityTransform)
        {
            currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix =
                    parentEntityTransform->m_finalTransform.m_boneAnimatedMatrix * animatedMatrix;
        }
        else
        {
            currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix = animatedMatrix;
        }

        // finally updating bone matrix in uniform buffer
        auto finalBoneMatrix =
                currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix * currentBone->m_offsetMatrix;

        // updating current bone matrix data in m_bonesMatricesData
        // 16 is count of scalars in matrix
        // 4 is count of scalars in vector. this vector contains count of bones
        std::memcpy(motionPlanner.m_bonesMatricesData.data() + 4 + currentBone->m_id * 16, glm::value_ptr(finalBoneMatrix), 16 * 4);
    }

    // if current entity has mesh then mesh of current entity
    // is using uniform buffer (m_bonesTransformationsUniformBuffer) from motion planner
    if(currentEntityMesh)
    {
        // currentEntityMesh->m_base.m_bonesUniformBuffer = motionPlanner.m_bonesTransformationsUniformBuffer;
        currentEntityMesh->m_base.m_bonesBuffer = motionPlanner.m_bonesMatricesBuffer;
    }

    // iterating through all children entities because we need to apply transformations to all children with bones
    for(const auto& childEntity : currentEntityBaseInfo.getChildren())
    {
        EntityBaseInfo::reg_t& childEntityBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
        Transform::reg_t* childEntityTransform = inRegistry->tryGet<Transform>(childEntity);

        if(!childEntityTransform) continue;

        processMotionNodes(dt,
                           inRegistry,
                           motionPlanner,
                           nodesToInterpolate,
                           motionPlanner.m_skeleton,
                           childEntity,
                           childEntityBaseInfo,
                           *childEntityTransform,
                           currentEntityTransform,
                           bonesCount
        );
    }

    // 1) взять два ключа между текущим временем текущей анимации и интерполировать значения трансформаций на основе текущего времени
    // 2) проделать то же самое с соседними анимациями (текущая анимация может быть связана с другими анимациями для плавного перехода между анимациями. например: ходьба - бег)
    // 3) провести интерполяцию между первым трансформом (получен в пункте 1)
    // и остальными трансформами (получены в пункте 2) на основе весов каждой анимации
    // (у каждой анимации есть свой вес, который определяет как она будет влиять на интерполяцию между трансформами)
    // 4) применить изменения к текущей кости

    // т.е. нужно пробегаться по всем сущностям в дереве, начиная от той, которая содержит MotionPlanner,
    // и брать кость из скелета по имени сущности (использовать именно rawName у сущности, а не изменённое имя (изменённое имя гарантирует быть уникальным)).
    // если кость не существует, то нужно выкидывать ошибку и останавливать MotionPlanner. WRONG!
    // после получения кости из скелета нужно получить все нужные ключи из текущей анимации и из соседних. ключи получаем
    // по имени полученной кости, а также по текущим временам анимаций.
    // после всего этого проделываем пункты выше.

    //if(node->m_currentAnimationTime >= node->m_skeletalAnimation->)
}

void SGCore::MotionPlannersResolver::collectNodesToInterpolate(const Ref<MotionPlannerNode>& currentNode,
                                                               std::vector<Ref<MotionPlannerNode>>& nodesToInterpolate) noexcept
{
    for(const auto& connection : currentNode->m_connections)
    {
        if(!connection->m_nextNode) continue;

        // calling activation function to understand if this connection of currentNode is must be active
        if(connection->m_previousNode->m_isActive &&
           connection->m_previousNode->m_isPlaying &&
           connection->m_nextNode->m_isActive &&
           connection->m_nextNode->activationFunction())
        {
            connection->m_nextNode->m_isPlaying = true;

            nodesToInterpolate.push_back(connection->m_nextNode);

            collectNodesToInterpolate(connection->m_nextNode, nodesToInterpolate);
        }
    }
}
