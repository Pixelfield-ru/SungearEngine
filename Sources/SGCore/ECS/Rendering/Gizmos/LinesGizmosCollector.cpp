//
// Created by Ilya on 06.11.2023.
//

#include "LinesGizmosCollector.h"
#include "LineGizmo.h"
#include "SGCore/ECS/Transformations/Transform.h"

void SGCore::LinesGizmosCollector::cacheEntity(const Ref<Entity>& entity)
{
    cacheEntityComponents<LineGizmo, Transform>(entity);
}
