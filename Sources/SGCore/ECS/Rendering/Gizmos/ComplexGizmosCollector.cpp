//
// Created by Ilya on 06.11.2023.
//

#include "ComplexGizmosCollector.h"
#include "IComplexGizmo.h"
#include "SGCore/ECS/Transformations/Transform.h"

void SGCore::ComplexGizmosCollector::cacheEntity(const Ref<Entity>& entity)
{
    cacheEntityComponents<IComplexGizmo, Transform>(entity);
}
