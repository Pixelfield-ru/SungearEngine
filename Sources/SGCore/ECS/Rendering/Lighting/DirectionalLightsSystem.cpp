//
// Created by stuka on 01.08.2023.
//

#include "DirectionalLightsSystem.h"
#include "DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/ECSWorld.h"

void Core::ECS::DirectionalLightsSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene,
                                                                const std::shared_ptr<Core::ECS::Entity>& entity)
{
    ECSWorld::cacheComponents<DirectionalLightsSystem, DirectionalLightComponent, TransformComponent>(entity);
}
