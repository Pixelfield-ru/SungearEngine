//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"
#include "Layer.h"

Core::ECS::Scene::Scene() noexcept
{
    auto defaultLayer = std::make_shared<Layer>();
    defaultLayer->m_name = "default";

    m_layers[defaultLayer->m_name] = std::move(defaultLayer);
}

void Core::ECS::Scene::setLayerName(const std::string& oldLayerName, std::string&& newLayerName) noexcept
{
    m_layers[oldLayerName]->m_name = std::move(newLayerName);
}

void Core::ECS::Scene::addLayer(std::string&& layerName) noexcept
{
    auto newLayer = std::make_shared<Layer>();
    newLayer->m_name = std::move(layerName);

    m_layers[newLayer->m_name] = std::move(newLayer);
}

void Core::ECS::Scene::addEntity(const std::shared_ptr<Entity>& entity) noexcept
{
    auto layer = m_layers["default"];
    entity->m_layer = layer;
    layer->m_entities.push_back(entity);
}

void Core::ECS::Scene::addEntity(const std::string& layerName, const std::shared_ptr<Entity>& entity) noexcept
{
    auto layer = m_layers[layerName];
    entity->m_layer = layer;
    layer->m_entities.push_back(entity);
}

void Core::ECS::Scene::addEntity(const std::shared_ptr<Core::ECS::Layer>& layer, const std::shared_ptr<Entity>& entity) noexcept
{
    entity->m_layer = layer;
    layer->m_entities.push_back(entity);
}

// ----------------

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

    for(const auto& layer : m_layers)
    {
        // define new shadow casters num for all entities in scene
        for(const auto& entity : layer.second->m_entities)
        {
            auto meshes = entity->getComponents<MeshComponent>();
            for(const auto& meshComponent: meshes)
            {
                for(const auto& shaderPair: meshComponent->m_mesh->m_material->getShaders())
                {
                    const auto& shader = shaderPair.second;

                    shader->setAssetModifiedChecking(false);

                    shader->removeShaderDefine(
                            SGShaderDefineType::SGG_OTHER_DEFINE,
                            SG_SHADERS_SHADOWS_CASTERS_NUM_NAME
                    );
                    shader->addShaderDefines(SGShaderDefineType::SGG_OTHER_DEFINE,
                                             {
                                                     Graphics::ShaderDefine(SG_SHADERS_SHADOWS_CASTERS_NUM_NAME,
                                                                            std::to_string(m_shadowsCastersNum))
                                             }
                    );

                    shader->setAssetModifiedChecking(true);
                }
            }
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

    for(const auto& layer : m_layers)
    {
        // define new shadow casters num for all entities in scene
        for(const auto& entity : layer.second->m_entities)
        {
            auto meshes = entity->getComponents<MeshComponent>();
            for(const auto& meshComponent: meshes)
            {
                for(const auto& shaderPair: meshComponent->m_mesh->m_material->getShaders())
                {
                    const auto& shader = shaderPair.second;

                    shader->setAssetModifiedChecking(false);

                    shader->removeShaderDefine(
                            SGShaderDefineType::SGG_OTHER_DEFINE,
                            SG_SHADERS_DIRECTIONAL_LIGHTS_NUM_NAME
                    );
                    shader->addShaderDefines(SGShaderDefineType::SGG_OTHER_DEFINE,
                                             {
                                                     Graphics::ShaderDefine(SG_SHADERS_DIRECTIONAL_LIGHTS_NUM_NAME,
                                                                            std::to_string(m_directionalLightsNum))
                                             }
                    );

                    shader->setAssetModifiedChecking(true);
                }
            }
        }
    }
}

int Core::ECS::Scene::getDirectionalLightsNum() const noexcept
{
    return m_directionalLightsNum;
}