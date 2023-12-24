//
// Created by stuka on 02.05.2023.
//

#include "Entity.h"

#include "SGCore/ECS/Rendering/Lighting/DirectionalLight.h"
#include "Scene.h"

void SGCore::Entity::addComponent(const Ref<IComponent>& component) noexcept
{
    m_components.push_back(component);

    auto lockedScene = m_parentScene.lock();

    if(lockedScene)
    {
        lockedScene->recacheEntity(shared_from_this());
    }
}

SGCore::Ref<SGCore::Layer> SGCore::Entity::getLayer() const noexcept
{
    return m_layer.lock();
}

void SGCore::Entity::setParentScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_parentScene = scene;

    attachToUniqueNamesManager(scene->getUniqueNamesManager());

    for(const auto& child : m_children)
    {
        child->setParentScene(scene);
    }
}

void SGCore::Entity::addChild(const SGCore::Ref<SGCore::Entity>& child) noexcept
{
    m_children.insert(child);

    child->setParentScene(m_parentScene.lock());
}

void SGCore::Entity::removeChild(const SGCore::Ref<SGCore::Entity>& child) noexcept
{
    m_children.erase(child);
}

void SGCore::Entity::clearChildren() noexcept
{
    m_children.clear();
}
