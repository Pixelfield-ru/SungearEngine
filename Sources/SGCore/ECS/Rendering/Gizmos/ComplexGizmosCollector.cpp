//
// Created by Ilya on 06.11.2023.
//

#include "ComplexGizmosCollector.h"
#include "IComplexGizmo.h"
#include "SGCore/ECS/Transformations/Transform.h"

void Core::ECS::ComplexGizmosCollector::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<IComplexGizmo, Transform>(entity);
}
