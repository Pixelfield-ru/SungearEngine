//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/Defines.h"
#include "Layer.h"
#include "ECSWorld.h"

SGCore::Scene::Scene() noexcept
{
    auto transparentLayer = MakeRef<Layer>();
    transparentLayer->m_name = SG_LAYER_TRANSPARENT_NAME;
    transparentLayer->m_index = 0;
    transparentLayer->m_isOpaque  = false;

    m_layers[SG_LAYER_TRANSPARENT_NAME] = std::move(transparentLayer);

    auto opaqueLayer = MakeRef<Layer>();
    opaqueLayer->m_name = SG_LAYER_OPAQUE_NAME;
    opaqueLayer->m_index = 1;

    m_layers[SG_LAYER_OPAQUE_NAME] = std::move(opaqueLayer);
}

void SGCore::Scene::addLayer(std::string&& layerName) noexcept
{
    auto newLayer = MakeRef<Layer>();
    newLayer->m_name = std::move(layerName);

    m_layers[newLayer->m_name] = std::move(newLayer);
}

void SGCore::Scene::addEntity(const Ref<Entity>& entity) noexcept
{
    entity->m_layer = m_layers[SG_LAYER_OPAQUE_NAME];
    m_entities.push_back(entity);

    ECSWorld::recacheEntity(entity);
}

void SGCore::Scene::addEntity(const Ref<Entity>& entity, const std::string& layerName) noexcept
{
    entity->m_layer = m_layers[layerName];
    m_entities.push_back(entity);

    ECSWorld::recacheEntity(entity);
}

void SGCore::Scene::addEntity(const Ref<Entity>& entity, const Ref<Layer>& layer) noexcept
{
    entity->m_layer = layer;
    m_entities.push_back(entity);

    ECSWorld::recacheEntity(entity);
}

// ----------------

SGCore::Ref<SGCore::Scene> SGCore::Scene::getCurrentScene() noexcept
{
    return m_currentScene;
}

void SGCore::Scene::setCurrentScene(const Ref<Scene>& newCurrentScene) noexcept
{
    m_currentScene = newCurrentScene;
}

SGCore::Ref<SGCore::Layer> SGCore::Scene::getLayer(const size_t& layerIndex) noexcept
{
    Ref<Layer> foundLayer;
    for(const auto& layer : m_layers)
    {
        if(layer.second->m_index == layerIndex)
        {
            foundLayer = layer.second;
        }
    }

    return foundLayer;
    //return m_layers.
}
