//
// Created by stuka on 22.10.2023.
//

#include "MeshedEntitiesCollectorSystem.h"
#include "SGCore/ECS/ECSWorld.h"
#include "MeshComponent.h"
#include "SGCore/ECS/Rendering/Primitives/IPrimitiveComponent.h"
#include "SkyboxComponent.h"

void Core::ECS::MeshedEntitiesCollectorSystem::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<MeshComponent, TransformComponent>(entity, [&entity]() {
        if(entity->getComponent<SkyboxComponent>())
        {
            return false;
        }

        return true;
    });
}
