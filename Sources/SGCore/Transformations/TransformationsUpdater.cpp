//
// Created by stuka on 02.05.2023.
//

#include "TransformationsUpdater.h"
#include "TransformBase.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "SGCore/Scene/Scene.h"
#include "GLFW/glfw3.h"
#include "TransformationsUtils.h"
#include "SGCore/Scene/EntityBaseInfo.h"

/*double curT = 0.0;
double accum = 0.0;
size_t fps = 0;*/

void SGCore::TransformationsUpdater::fixedUpdate
(const Ref<Scene>& scene)
{
    auto transformsView = scene->getECSRegistry().view<Transform>();

    transformsView.each([&scene](const entt::entity& entity, Transform& transform) {
        EntityBaseInfo* entityBaseInfo = scene->getECSRegistry().try_get<EntityBaseInfo>(entity);
        Transform* parentTransform = nullptr;

        if(entityBaseInfo)
        {
            parentTransform = scene->getECSRegistry().try_get<Transform>(entityBaseInfo->m_parent);
        }

        TransformationsUtils::updateTransform(transform, parentTransform);
    });
}