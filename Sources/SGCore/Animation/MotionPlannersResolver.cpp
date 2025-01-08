//
// Created by stuka on 07.01.2025.
//

#include "MotionPlannersResolver.h"
#include "SGCore/Scene/Scene.h"
#include "MotionPlannerConnection.h"

void SGCore::MotionPlannersResolver::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto registry = getScene()->getECSRegistry();

    auto motionPlannersView = registry->view<EntityBaseInfo, MotionPlanner>();

    motionPlannersView.each([dt, &registry](const EntityBaseInfo::reg_t& entityBaseInfo,
                                            MotionPlanner::reg_t& motionPlanner) {
        auto skeleton = motionPlanner.m_skeleton;

        if(!motionPlanner.m_rootNode) return;

        processMotionNode(dt, registry, motionPlanner, motionPlanner.m_rootNode, motionPlanner.m_skeleton, entityBaseInfo);
    });
}

void SGCore::MotionPlannersResolver::processMotionNode(const double& dt,
                                                       Ref<ECS::registry_t>& inRegistry,
                                                       MotionPlanner& motionPlanner,
                                                       const Ref<MotionPlannerNode>& currentNode,
                                                       const AssetRef<Skeleton>& skeleton,
                                                       const EntityBaseInfo& currentEntity)
{
    if(!currentNode->m_isActive) return;

    // todo: what about the root node and the root entity? how to update them omg

    std::vector<Ref<MotionPlannerNode>> nodesToInterpolate;
    // nodesToInterpolate.push_back(currentNode);

    for(const auto& connection : currentNode->m_connections)
    {
        // calling activation function to understand if this connection of currentNode is must be active
        if(connection->activationFunction())
        {
            nodesToInterpolate.push_back(connection->m_nextNode);
        }
    }

    // iterating through all children entities because we need to apply transformations to all children with bones
    for(const auto& childEntity : currentEntity.getChildren())
    {
        EntityBaseInfo::reg_t& childEntityBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);
        // EntityBaseInfo::reg_t& childEntityBaseInfo = inRegistry->get<EntityBaseInfo>(childEntity);

        std::vector<SkeletalBoneAnimation*> currentBoneAnims;

        auto currentBone = skeleton->findBone(childEntityBaseInfo.getRawName());

        if(currentBone)
        {
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
                    LOG_W(SGCORE_TAG, "MotionPlannersResolver: Can not find bone '{}' in skeletal animation '{}'!",
                          currentBone->m_boneName, currentNode->m_skeletalAnimation->m_animationName);
                }
            }

            // GUARANTEED THAT currentBoneAnims.size() == nodesToInterpolate.size()
            // calculating all transformations for all animations using currentBone and keys from currentBoneAnims
            for(size_t i = 0; i < currentBoneAnims.size(); ++i)
            {
                auto& currentNodeToInterpolate = nodesToInterpolate[i];
                auto* currentBoneAnim = currentBoneAnims[i];

                currentNodeToInterpolate->m_currentAnimationTime +=
                        dt *
                        currentNodeToInterpolate->m_skeletalAnimation->m_ticksPerSecond *
                        currentNodeToInterpolate->m_animationSpeed;

                float curAnimTime = currentNodeToInterpolate->m_currentAnimationTime;
                // calculating interpolated position for bone
                {
                    const std::int64_t nextKeyIndex = currentBoneAnim->findPositionKeyByTime(curAnimTime);
                    if(nextKeyIndex != -1)
                    {
                        KeyPosition& nextKey = currentBoneAnim->m_positionKeys[nextKeyIndex];
                        KeyPosition prevKey;
                        if(nextKeyIndex - 1 >= 0)
                        {
                            prevKey = currentBoneAnim->m_positionKeys[nextKeyIndex - 1];
                        }

                        float normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);

                        auto interpolatedPosition = glm::lerp(prevKey.m_position, nextKey.m_position, normalizedTime);
                    }
                }

                // calculating interpolated rotation for bone
                {
                    const std::int64_t nextKeyIndex = currentBoneAnim->findRotationKeyByTime(curAnimTime);
                    if(nextKeyIndex != -1)
                    {
                        const KeyRotation& nextKey = currentBoneAnim->m_rotationKeys[nextKeyIndex];
                        KeyRotation prevKey;
                        if(nextKeyIndex - 1 >= 0)
                        {
                            prevKey = currentBoneAnim->m_rotationKeys[nextKeyIndex - 1];
                        }

                        float normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);

                        auto interpolatedRotation = glm::slerp(prevKey.m_rotation, nextKey.m_rotation, normalizedTime);
                    }
                }

                // calculating interpolated scale for bone
                {
                    const std::int64_t nextKeyIndex = currentBoneAnim->findScaleKeyByTime(curAnimTime);
                    if(nextKeyIndex != -1)
                    {
                        const KeyScale& nextKey = currentBoneAnim->m_scaleKeys[nextKeyIndex];
                        KeyScale prevKey;
                        if(nextKeyIndex - 1 >= 0)
                        {
                            prevKey = currentBoneAnim->m_scaleKeys[nextKeyIndex - 1];
                        }

                        float normalizedTime = (curAnimTime - prevKey.m_timeStamp) / (nextKey.m_timeStamp - prevKey.m_timeStamp);

                        auto interpolatedScale = glm::slerp(prevKey.m_scale, nextKey.m_scale, normalizedTime);
                    }
                }
            }
        }
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
