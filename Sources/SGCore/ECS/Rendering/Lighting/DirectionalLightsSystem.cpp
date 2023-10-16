//
// Created by stuka on 01.08.2023.
//

#include "DirectionalLightsSystem.h"
#include "DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/ECSWorld.h"
#include "GLFW/glfw3.h"

void Core::ECS::DirectionalLightsSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene,
                                                                const std::shared_ptr<Core::ECS::Entity>& entity)
{
    double t0 = glfwGetTime();

    ECSWorld::cacheComponents<DirectionalLightsSystem, DirectionalLightComponent, TransformComponent>(entity);

    double t1 = glfwGetTime();

    // 0.002500 ms average

    //std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}
