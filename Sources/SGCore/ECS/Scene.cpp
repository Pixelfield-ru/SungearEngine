//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/Defines.h"
#include "Layer.h"
#include "ECSWorld.h"

Core::ECS::Scene::Scene() noexcept
{
    auto transparentLayer = std::make_shared<Layer>();
    transparentLayer->m_name = SG_LAYER_TRANSPARENT_NAME;
    transparentLayer->m_index = 0;
    transparentLayer->m_isOpaque  = false;

    auto opaqueLayer = std::make_shared<Layer>();
    opaqueLayer->m_name = SG_LAYER_OPAQUE_NAME;
    opaqueLayer->m_index = 1;

    m_layers[transparentLayer->m_name] = std::move(transparentLayer);
    m_layers[opaqueLayer->m_name] = std::move(opaqueLayer);
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
    auto layer = m_layers[SG_LAYER_OPAQUE_NAME];
    entity->m_layer = layer;
    layer->m_entities.push_back(entity);

    ECSWorld::recacheEntity(entity);
}

void Core::ECS::Scene::addEntity(const std::shared_ptr<Entity>& entity, const std::string& layerName) noexcept
{
    auto layer = m_layers[layerName];
    entity->m_layer = layer;
    layer->m_entities.push_back(entity);

    ECSWorld::recacheEntity(entity);
}

void Core::ECS::Scene::addEntity(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Core::ECS::Layer>& layer) noexcept
{
    entity->m_layer = layer;
    layer->m_entities.push_back(entity);

    ECSWorld::recacheEntity(entity);
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

void Core::ECS::Scene::setShadowsCastersNum(const size_t& num)
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

size_t Core::ECS::Scene::getShadowsCastersNum() const noexcept
{
    return m_shadowsCastersNum;
}

void Core::ECS::Scene::setDirectionalLightsNum(const size_t& num)
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

size_t Core::ECS::Scene::getDirectionalLightsNum() const noexcept
{
    return m_directionalLightsNum;
}