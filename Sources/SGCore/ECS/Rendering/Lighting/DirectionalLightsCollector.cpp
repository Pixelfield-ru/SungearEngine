//
// Created by stuka on 01.08.2023.
//

#include "DirectionalLightsCollector.h"
#include "DirectionalLight.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "GLFW/glfw3.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"

void SGCore::DirectionalLightsCollector::cacheEntity(const Ref<Entity>& entity)
{
    cacheEntityComponents<DirectionalLight, Transform>(entity);
}