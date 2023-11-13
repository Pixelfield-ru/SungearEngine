//
// Created by Ilya on 06.11.2023.
//

#include "LinesGizmosCollector.h"
#include "LineGizmo.h"
#include "SGCore/ECS/Transformations/Transform.h"

void Core::ECS::LinesGizmosCollector::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<LineGizmo, Transform>(entity);
}
