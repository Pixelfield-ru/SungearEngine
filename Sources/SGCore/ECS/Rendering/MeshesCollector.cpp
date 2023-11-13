//
// Created by stuka on 22.10.2023.
//

#include "MeshesCollector.h"
#include "SGCore/ECS/ECSWorld.h"
#include "Mesh.h"
#include "Skybox.h"
#include "SGCore/ECS/Rendering/Gizmos/IGizmo.h"

void Core::ECS::MeshesCollector::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<Mesh, Transform>(entity, [&entity]() {
        if(entity->getComponent<Skybox>())
        {
            return false;
        }

        return true;
    }, [](const std::shared_ptr<IComponent>& component){
        // we wont cache mesh component if it is gizmo, because gizmo renderings in different way
        return !SG_INSTANCEOF(component.get(), IGizmo);
    });
}
