//
// Created by Ilya on 06.11.2023.
//

#include "LinesCollectorSystem.h"
#include "LineComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"

void Core::ECS::LinesCollectorSystem::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<LineComponent, TransformComponent>(entity);
}
