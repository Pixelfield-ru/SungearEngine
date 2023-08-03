//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Utils/Defines.h"
#include "SGCore/ECS/Rendering/ShadowsCasterComponent.h"

std::shared_ptr<Core::ECS::Scene> Core::ECS::Scene::getCurrentScene() noexcept
{
    return m_currentScene;
}

void Core::ECS::Scene::setCurrentScene(const std::shared_ptr<Scene>& newCurrentScene) noexcept
{
    m_currentScene = newCurrentScene;
}

void Core::ECS::Scene::setShadowsCastersNum(const int& num)
{
    m_shadowsCastersNum = num;

    // define new shadow casters num for all entities in scene
    for(const auto& entity : m_entities)
    {
        auto meshes = entity->getComponents<MeshComponent>();
        for(const auto& meshComponent : meshes)
        {
            meshComponent->m_mesh->m_material->m_shader->setAssetModifiedChecking(false);

            meshComponent->m_mesh->m_material->m_shader->removeShaderDefine(
                    SG_SHADERS_SHADOWS_CASTERS_NUM_NAME
                    );
            meshComponent->m_mesh->m_material->m_shader->addShaderDefines({
                Graphics::ShaderDefine(SG_SHADERS_SHADOWS_CASTERS_NUM_NAME,
                                       std::to_string(m_shadowsCastersNum))
            });

            meshComponent->m_mesh->m_material->m_shader->setAssetModifiedChecking(true);
        }
    }
}

int Core::ECS::Scene::getShadowsCastersNum() const noexcept
{
    return m_shadowsCastersNum;
}

void Core::ECS::Scene::setDirectionalLightsNum(const int& num)
{
    m_directionalLightsNum = num;

    // define new shadow casters num for all entities in scene
    for(const auto& entity : m_entities)
    {
        auto meshes = entity->getComponents<MeshComponent>();
        for(const auto& meshComponent : meshes)
        {
            meshComponent->m_mesh->m_material->m_shader->setAssetModifiedChecking(false);

            meshComponent->m_mesh->m_material->m_shader->removeShaderDefine(
                    SG_SHADERS_DIRECTIONAL_LIGHTS_NUM_NAME
                    );
            meshComponent->m_mesh->m_material->m_shader->addShaderDefines({
                Graphics::ShaderDefine(SG_SHADERS_DIRECTIONAL_LIGHTS_NUM_NAME,
                                       std::to_string(m_directionalLightsNum))
            });

            meshComponent->m_mesh->m_material->m_shader->setAssetModifiedChecking(true);
        }
    }
}

int Core::ECS::Scene::getDirectionalLightsNum() const noexcept
{
    return m_directionalLightsNum;
}
