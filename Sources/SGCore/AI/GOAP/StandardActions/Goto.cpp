//
// Created by stuka on 11.01.2026.
//

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/scalar_multiplication.hpp>

#include "Goto.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Transformations/Transform.h"

SGCore::Coro::Task<bool> SGCore::GOAP::Goto::executeImpl(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    auto* tmpTransform = registry.tryGet<Transform>(forEntity);
    if(!tmpTransform) co_return false;

    auto& transform = *tmpTransform;

    auto currentThread = Threading::ThreadsManager::currentThread();
    while(glm::distance(transform->m_finalTransform.m_position, m_targetPosition) > m_distanceErrorRate)
    {
        transform->m_ownTransform.m_position = glm::normalize(transform->m_finalTransform.m_position - m_targetPosition) * m_speed * currentThread->getDeltaTime();

        co_await Coro::returnToCaller();
    }

    auto g = executeImpl(registry, forEntity).run();

    co_return true;
}

void SGCore::GOAP::Goto::calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    m_cost = 10.0f;
}

SGCore::GOAP::IAction* SGCore::GOAP::Goto::clone() const noexcept
{
    return new Goto(*this);
}
