//
// Created by stuka on 20.03.2026.
//

#include "Component.h"

SGCore::ECS::ComponentBase::ComponentBase(const ComponentBase& other) noexcept
{
    // do not copy m_entity field!!
    // ======================

    m_isActive = other.m_isActive;
}

SGCore::ECS::entity_t SGCore::ECS::ComponentBase::getThisEntity() const noexcept
{
    return m_thisEntity;
}

void SGCore::ECS::ComponentBase::setActive(bool active) noexcept
{
    m_isActive = active;
}

bool SGCore::ECS::ComponentBase::isActive() const noexcept
{
    return m_isActive;
}

SGCore::ECS::ComponentBase& SGCore::ECS::ComponentBase::operator=(const ComponentBase& other)
{
    // do not copy m_entity field!!
    // ======================

    m_isActive = other.m_isActive;

    return *this;
}
