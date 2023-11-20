//
// Created by Ilya on 20.10.2023.
//

#include "ComponentsCollection.h"

void SGCore::ComponentsCollection::clear() noexcept
{
    m_components.clear();
}

void SGCore::ComponentsCollection::addComponent(const std::shared_ptr<IComponent>& component) noexcept
{
    m_components.push_back(component);
}
