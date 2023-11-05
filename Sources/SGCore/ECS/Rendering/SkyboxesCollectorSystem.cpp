//
// Created by stuka on 03.08.2023.
//

#include "SkyboxesCollectorSystem.h"

#include "SkyboxComponent.h"
#include "MeshComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"

void Core::ECS::SkyboxesCollectorSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<SkyboxComponent, MeshComponent, TransformComponent>(entity);
}
