//
// Created by stuka on 03.08.2023.
//

#include "SkyboxesCollector.h"

#include "Skybox.h"
#include "Mesh.h"
#include "SGCore/ECS/Transformations/Transform.h"

void SGCore::SkyboxesCollector::cacheEntity(const Ref<SGCore::Entity>& entity)
{
    cacheEntityComponents<Skybox, Mesh, Transform>(entity);
}
