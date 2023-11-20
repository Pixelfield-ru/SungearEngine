//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCastersCollector.h"
#include "ShadowsCaster.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/ECSWorld.h"

void SGCore::ShadowsCastersCollector::cacheEntity(const Ref<Entity>& entity)
{
    cacheEntityComponents<ShadowsCaster, Transform>(entity);
}

