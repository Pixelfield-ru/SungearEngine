//
// Created by stuka on 02.05.2023.
//

#include "Entity.h"

void Core::ECS::Entity::addComponent(const std::shared_ptr<IComponent>& component) noexcept
{
    m_components.push_back(component);
}
