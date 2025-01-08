//
// Created by stuka on 07.01.2025.
//

#include "MotionPlannersResolver.h"
#include "SGCore/Scene/Scene.h"

void SGCore::AnimatorsSystem::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto registry = getScene()->getECSRegistry();

    auto motionPlannersView = registry->view<EntityBaseInfo, MotionPlanner>();

    motionPlannersView.each([dt](const EntityBaseInfo::reg_t& entityBaseInfo,
                                 MotionPlanner::reg_t& motionPlanner) {
        // motionPlanner.m_skeleton->m_allBones
        // motionPlanner.m_rootNode->m_skeletalAnimation->m_bonesAnimations[0].

        auto skeleton = motionPlanner.m_skeleton;

        if(!motionPlanner.m_rootNode) return;

        processMotionNode(dt, motionPlanner, motionPlanner.m_rootNode, motionPlanner.m_skeleton, entityBaseInfo);
        // motionPlanner.m_bonesTransformationsUniformBuffer->
        // for(const auto& )
    });
}

void SGCore::AnimatorsSystem::processMotionNode(const double& dt,
                                                MotionPlanner& motionPlanner,
                                                const Ref<MotionPlannerNode>& node,
                                                const AssetRef<Skeleton>& skeleton,
                                                const EntityBaseInfo& currentEntity)
{
    if(!node->m_isActive) return;

    node->m_currentAnimationTime += dt;

    // 1) взять два ключа между текущим временем текущей анимации и интерполировать значения трансформаций на основе текущего времени
    // 2) проделать то же самое с соседними анимациями (текущая анимация может быть связана с другими анимациями для плавного перехода между анимациямм. например: ходьба - бег)
    // 3) провести интерполяцию между первым трансформом (получен в пункте 1)
    // и остальными трансформами (получены в пункте 2) на основе весов каждой анимации
    // (у каждой анимации есть свой вес, который определяет как она будет влиять на интерполяцию между трансформами)
    // 4) применить изменения к текущей кости

    // т.е. нужно пробегаться по всем сущностям в дереве, начиная от той, которая содержит MotionPlanner,
    // и брать кость из скелета по имени сущности (использовать именно rawName у сущности, а не изменённое имя (изменённое имя гарантирует быть уникальным)).
    // если кость не существует, то нужно выкидывать ошибку и останавливать MotionPlanner.
    // после получения кости из скелета нужно получить все нужные ключи из текущей анимации и из соседних. ключи получаем
    // по имени полученной кости, а также по текущим временам анимаций.
    // после всего этого проделываем пункты выше.

    //if(node->m_currentAnimationTime >= node->m_skeletalAnimation->)
}
