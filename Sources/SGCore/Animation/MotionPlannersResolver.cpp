//
// Created by stuka on 07.01.2025.
//

#include "MotionPlannersResolver.h"
#include "SGCore/Scene/Scene.h"
#include "MotionPlannerConnection.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderPipelinesManager.h"

// omg

void SGCore::MotionPlannersResolver::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    if(m_debugDrawPassRef.expired())
    {
        m_debugDrawPassRef = RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<DebugDraw>();
    }

    auto registry = lockedScene->getECSRegistry();

    auto motionPlannersView = registry->view<EntityBaseInfo, Transform, MotionPlanner>();

    motionPlannersView.each([dt, &registry, this](const ECS::entity_t& entity,
                                                  const EntityBaseInfo::reg_t& entityBaseInfo,
                                                  const Transform::reg_t& transform,
                                                  MotionPlanner::reg_t& motionPlanner) {
        auto skeleton = motionPlanner.m_skeleton;

        std::vector<Ref<MotionPlannerNode>> nodesToInterpolate;

        // collecting all nodes to interpolate bones animations
        for(const auto& rootNode : motionPlanner.m_rootNodes)
        {
            if(rootNode->m_isActive /*&& rootNode->m_activationAction->execute()*/)
            {
                rootNode->m_anyState.m_isSomeNodeInTreeExceptRootActive = false;

                if(rootNode->m_skeletalAnimation)
                {
                    // rootNode->m_isPlaying = true;
                    // nodesToInterpolate.push_back(rootNode);
                }

                collectAndUpdateNodesToInterpolate(dt, motionPlanner, rootNode, rootNode, nodesToInterpolate);
            }
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
                    // node->m_currentAnimationTime -= node->m_skeletalAnimation->m_duration;
                    node->m_currentAnimationTime = 0.0f;
                    // node->m_currentAnimationTime = std::fmod(node->m_currentAnimationTime, node->m_skeletalAnimation->m_duration);
                }
                else
                {
                    node->m_currentAnimationTime = node->m_skeletalAnimation->m_duration - 1;
                    node->m_isPaused = true;
                }
            }

            node->m_isPlaying = !node->m_isPaused;
        }

        std::int32_t updatedBonesCount = 0;

        if(!nodesToInterpolate.empty())
        {
            // std::cout << "playing nodes: " << nodesToInterpolate.size() << std::endl;

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

        // updating count of bones in data vector
        motionPlanner.m_bonesMatricesData[0] = updatedBonesCount;

        std::cout << "updatedBonesCount: " << updatedBonesCount << std::endl;

        // updating data in texture buffer after updating bones matrices
        motionPlanner.m_bonesMatricesBuffer->bind(0);
        motionPlanner.m_bonesMatricesBuffer->subTextureBufferData(motionPlanner.m_bonesMatricesData.data(),
                                                                  motionPlanner.m_bonesMatricesData.size(), 0);
    });
}

void SGCore::MotionPlannersResolver::processMotionNodes(const double& dt,
                                                        Ref<ECS::registry_t>& inRegistry,
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
    }

    // collecting all animations for currentBone
    for(const auto& node : nodesToInterpolate)
    {
        // std::cout << "bones animations count: " << node->m_skeletalAnimation->getBonesAnimationsCount() << std::endl;
        SkeletalBoneAnimation* boneAnimation = node->m_skeletalAnimation->getBoneAnimation(currentBone ? currentBone->m_boneName : currentEntityBaseInfo.getRawName());
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

    bool isBoneAnimated = false;

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
                const KeyPosition& nextKey = currentBoneAnim->m_positionKeys[nextKeyIndex];
                KeyPosition prevKey;

                float normalizedTime = 0.0f;
                if(nextKeyIndex - 1 >= 0)
                {
                    prevKey = currentBoneAnim->m_positionKeys[nextKeyIndex - 1];
                    normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);
                }
                else
                {
                    prevKey = currentBoneAnim->m_positionKeys[currentBoneAnim->m_positionKeys.size() - 1];
                    normalizedTime = curAnimTime / nextKey.m_timeStamp;
                }

                if(normalizedTime > 1.0f) normalizedTime = 1.0f;
                interpolatedPosition = glm::lerp(prevKey.m_position, nextKey.m_position, normalizedTime);
            }

            isBoneAnimated = true;
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

                float normalizedTime = 0.0f;
                if(nextKeyIndex - 1 >= 0)
                {
                    prevKey = currentBoneAnim->m_rotationKeys[nextKeyIndex - 1];
                    normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);
                }
                else
                {
                    prevKey = currentBoneAnim->m_rotationKeys[currentBoneAnim->m_rotationKeys.size() - 1];
                    normalizedTime = curAnimTime / nextKey.m_timeStamp;
                }

                if(normalizedTime > 1.0f) normalizedTime = 1.0f;
                interpolatedRotation = glm::normalize(
                    glm::slerp(prevKey.m_rotation, nextKey.m_rotation, normalizedTime));
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

                float normalizedTime = 0.0f;
                if(nextKeyIndex - 1 >= 0)
                {
                    prevKey = currentBoneAnim->m_scaleKeys[nextKeyIndex - 1];
                    normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);
                }
                else
                {
                    prevKey = currentBoneAnim->m_scaleKeys[currentBoneAnim->m_scaleKeys.size() - 1];
                    normalizedTime = curAnimTime / nextKey.m_timeStamp;
                }

                if(normalizedTime > 1.0f) normalizedTime = 1.0f;
                interpolatedScale = glm::lerp(prevKey.m_scale, nextKey.m_scale, normalizedTime);
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
            const auto& node = nodesToInterpolate[i];

            interpolatedPosition = glm::lerp(interpolatedPosition, positionsToInterpolate[i],
                                             node->m_currentBlendFactor);
            interpolatedRotation = glm::slerp(interpolatedRotation, rotationsToInterpolate[i],
                                              node->m_currentBlendFactor);
            interpolatedScale = glm::lerp(interpolatedScale, scalesToInterpolate[i], node->m_currentBlendFactor);
        }
    }

    // constructing animated matrix from interpolated transform components of all animations nodes
    auto animatedMatrix = glm::identity<glm::mat4>();
    auto offsetMatrix = glm::identity<glm::mat4>();

    // if this bone is animated by some node
    // optimization
    if(isBoneAnimated)
    {
        animatedMatrix = glm::translate(animatedMatrix, interpolatedPosition);
        animatedMatrix *= glm::toMat4(interpolatedRotation);
        animatedMatrix = glm::scale(animatedMatrix, interpolatedScale);
    }
    else
    {
        // if(!currentBone)
        {
            animatedMatrix = currentEntityTransform->m_ownTransform.m_testMatrix;
        }
    }

    if(currentBone)
    {
        offsetMatrix = currentBone->m_offsetMatrix;
        // offsetMatrix = glm::inverse(currentEntityTransform->m_finalTransform.m_testMatrix);
    }
    else
    {
        offsetMatrix = glm::inverse(currentEntityTransform->getInitialModelMatrix());
    }

    // if current entity has parent with bone
    if(parentEntityTransform)
    {
        // optimization
        // if(isBoneAnimated)
        {
            currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix =
                    parentEntityTransform->m_finalTransform.m_boneAnimatedMatrix * animatedMatrix;
        }
        /*else
        {
            currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix =
                    parentEntityTransform->m_finalTransform.m_boneAnimatedMatrix;
        }*/
    }
    else
    {
        currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix = animatedMatrix;
    }

    currentEntityTransform->m_ownTransform.m_boneAnimatedMatrix = animatedMatrix;

    /*if(!currentBone)
    {
        currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix = currentEntityTransform->m_finalTransform.m_testMatrix;
    }*/

    // finally updating bone matrix in uniform buffer

    // glm::mat4 boneFinalMatrix = currentEntityTransform->m_finalTransform.m_testMatrix;
    // todo: !!!! FIX ANIMATED MATRIX currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix
    glm::mat4 boneFinalMatrix = currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix * offsetMatrix;
    // glm::mat4 boneFinalMatrix = glm::identity<glm::mat4>();

    // glm::mat4 boneFinalMatrix = currentEntityTransform->m_finalTransform.m_testMatrix * offsetMatrix * currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix;

    // if(isBoneAnimated)
    {
        // boneFinalMatrix = (currentEntityTransform->m_finalTransform.m_testMatrix) * currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix * offsetMatrix;
        // boneFinalMatrix *= offsetMatrix;
    }
    /*else
    {
        boneFinalMatrix *= currentEntityTransform->m_finalTransform.m_testMatrix * offsetMatrix;
    }*/

    // boneFinalMatrix = glm::identity<glm::mat4>();

    // if bone is not animated than we are set boneMatrix to identity to avoid incorrect position of node or mesh.
    // offset matrix will cancel the model matrix of node or mesh if bone is not animated
    if(!isBoneAnimated)
    {
        currentEntityTransform->m_boneMatrix = glm::identity<glm::mat4>();
    }
    else
    {
        if(!currentBone)
        {
            currentEntityTransform->m_boneMatrix = animatedMatrix * offsetMatrix;
        }
    }

    // currentEntityTransform->m_boneMatrix = glm::identity<glm::mat4>();

    currentEntityTransform->m_isAnimated = isBoneAnimated;

    if(currentBone)
    {
        // glm::vec3 translation = currentEntityTransform->m_finalTransform.m_modelMatrix[3];

        currentBone->m_currentPosition = (currentEntityTransform->m_finalTransform.m_testMatrix * offsetMatrix * currentEntityTransform->m_finalTransform.m_boneAnimatedMatrix)[3];

        // std::cout << "bone '" << currentBone->m_boneName << "' pos: " << currentBone->m_currentPosition << std::endl;

        auto parentBone = currentBone->m_parent.lock();
        if(parentBone)
        {
            glm::vec3 startPos = parentBone->m_currentPosition;
            m_debugDrawPassRef.lock()->drawLine(startPos, currentBone->m_currentPosition, { 1.0f, 1.0f, 0.0f, 1.0f });
        }
    }

    // updating current bone matrix data in m_bonesMatricesData
    // 16 is count of scalars in matrix
    // 4 is count of scalars in vector. this vector contains count of bones
    if(currentBone)
    {
        std::memcpy(motionPlanner.m_bonesMatricesData.data() + 4 + currentBone->m_id * 16 * 4,
                    glm::value_ptr(boneFinalMatrix), 16 * 4);
    }

    // ========================================================================================================

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

        // if(!childEntityTransform) continue;

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

void SGCore::MotionPlannersResolver::collectAndUpdateNodesToInterpolate(const double& dt,
                                                                        MotionPlanner& motionPlanner,
                                                                        const Ref<MotionPlannerNode>& fromRootNode,
                                                                        const Ref<MotionPlannerNode>& currentNode,
                                                                        std::vector<Ref<MotionPlannerNode>>& nodesToInterpolate) noexcept
{
    currentNode->m_currentBlendFactor = std::max(currentNode->m_currentBlendFactor, currentNode->m_staticBlendFactor);

    bool isNoConnectionsActivated = true;

    for(const auto& connection : currentNode->m_connections)
    {
        if(!connection->m_nextNode || !connection->m_nextNode->m_skeletalAnimation) continue;

        bool isConnectionActivated = connection->m_activationAction->execute();

        auto previousNode = connection->m_previousNode.lock();

        // calling activation function to understand if this connection of currentNode is must be active
        if(previousNode &&
           previousNode->m_isActive &&
           previousNode->m_isPlaying &&
           connection->m_nextNode->m_isActive &&
           isConnectionActivated)
        {
            connection->m_nextNode->m_isPlaying = true;

            if(connection->m_currentBlendTime < connection->m_blendTime)
            {
                connection->m_currentBlendTime += dt * connection->m_blendSpeed;
                connection->m_currentBlendTime = std::min(connection->m_currentBlendTime, connection->m_blendTime);

                const float blendFactor = connection->m_currentBlendTime / connection->m_blendTime;

                connection->m_nextNode->m_currentBlendFactor = blendFactor;
                previousNode->m_currentBlendFactor = 1.0 - blendFactor;
            }

            if(previousNode->m_currentBlendFactor > 0.0f)
            {
                nodesToInterpolate.push_back(previousNode);
            }

            fromRootNode->m_anyState.m_isSomeNodeInTreeExceptRootActive = true;

            isNoConnectionsActivated = false;

            collectAndUpdateNodesToInterpolate(dt, motionPlanner, fromRootNode, connection->m_nextNode, nodesToInterpolate);
        }
        else if(!isConnectionActivated)
        {
            // go backwards from nextNode to previousNode using interpolation
            if(connection->m_currentBlendTime > 0)
            {
                // next node is still playing but not its children
                connection->m_nextNode->m_isPlaying = true;

                connection->m_currentBlendTime -= dt * connection->m_blendSpeed;
                connection->m_currentBlendTime = std::max(connection->m_currentBlendTime, 0.0f);

                const float blendFactor = connection->m_currentBlendTime / connection->m_blendTime;

                connection->m_nextNode->m_currentBlendFactor = blendFactor;
                previousNode->m_currentBlendFactor = 1.0f - blendFactor;

                // we must add next node to nodesToInterpolate because the next node still
                // needs to be taken into account when interpolating.
                //
                // but we don't need to call collectAndUpdateNodesToInterpolate for nextNode because
                // we are performing reverse interpolation, i.e. it is assumed that nextNode is already inactive.
                nodesToInterpolate.push_back(connection->m_nextNode);
            }
        }
    }

    // interpolating only last activated node
    if(isNoConnectionsActivated)
    {
        nodesToInterpolate.push_back(currentNode);
    }
}
