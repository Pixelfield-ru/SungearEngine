//
// Created by stuka on 02.05.2023.
//

#include "Entity.h"

#include "SGCore/ECS/Rendering/Lighting/DirectionalLight.h"
#include "Scene.h"

void SGCore::Entity::addComponent(const Ref<IComponent>& component) noexcept
{
    m_components.push_back(component);

    auto lockedScene = m_scene.lock();

    if(lockedScene)
    {
        lockedScene->recacheEntity(shared_from_this());
    }
}

SGCore::Ref<SGCore::Layer> SGCore::Entity::getLayer() const noexcept
{
    return m_layer.lock();
}

size_t SGCore::Entity::getCountOfEntities(const std::string& entitiesNames) const noexcept
{
    size_t count = 0;

    for(const auto& child : m_children)
    {
        if(child->m_name == entitiesNames)
        {
            ++count;
        }

        count += child->getCountOfEntities(entitiesNames);
    }

    return count;
}

size_t SGCore::Entity::getSceneSameNameIndex() const noexcept
{
    return m_sceneSameNameIndex;
}
