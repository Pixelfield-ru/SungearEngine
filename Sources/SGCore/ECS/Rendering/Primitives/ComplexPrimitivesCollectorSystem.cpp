//
// Created by Ilya on 06.11.2023.
//

#include "ComplexPrimitivesCollectorSystem.h"
#include "ComplexPrimitiveComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"

void Core::ECS::ComplexPrimitivesCollectorSystem::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<ComplexPrimitiveComponent, TransformComponent>(entity);
}
