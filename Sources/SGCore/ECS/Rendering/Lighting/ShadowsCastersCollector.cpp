//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCastersCollector.h"
#include "ShadowsCaster.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/ECSWorld.h"

void Core::ECS::ShadowsCastersCollector::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<ShadowsCaster, Transform>(entity);
}

