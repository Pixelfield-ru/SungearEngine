//
// Created by stuka on 03.08.2023.
//

#include "SkyboxesCollector.h"

#include "Skybox.h"
#include "Mesh.h"
#include "SGCore/ECS/Transformations/Transform.h"

void Core::ECS::SkyboxesCollector::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<Skybox, Mesh, Transform>(entity);
}
