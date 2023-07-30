//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCasterSystem.h"
#include "ShadowsCasterComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "MeshComponent.h"

void Core::ECS::ShadowsCasterSystem::update(const std::shared_ptr<Scene>& scene,
                                            const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::shared_ptr<ShadowsCasterComponent> shadowsCasterComponent = entity->getComponent<ShadowsCasterComponent>();

    if(!shadowsCasterComponent) return;

    for(auto& sceneEntity : scene->m_entities)
    {
        std::shared_ptr<TransformComponent> transformComponent = sceneEntity->getComponent<TransformComponent>();
        std::shared_ptr<MeshComponent> meshComponent = sceneEntity->getComponent<MeshComponent>();

        if(!transformComponent || !meshComponent) continue;

        // TODO: make rendering
        Core::Main::CoreMain::getRenderer().renderMesh(shadowsCasterComponent, transformComponent, meshComponent);
    }
}

void Core::ECS::ShadowsCasterSystem::deltaUpdate(const std::shared_ptr<Scene>& scene,
                                                 const std::shared_ptr<Core::ECS::Entity>& entity,
                                                 const double& deltaTime)
{

}
