//
// Created by stuka on 10.01.2026.
//

#include "Plan.h"

#include "SGCore/ECS/Registry.h"

SGCore::Coro::Task<> SGCore::GOAP::Plan::execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    m_isExecuting = true;

    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState)
    {
        m_isExecuting = false;
        co_return;
    }

    std::vector<const State*> tempStates;

    for(const auto& action : m_actions)
    {
        co_await action->execute(registry, forEntity);

        // collecting temporary states to reset
        for(const auto* effect : action->getTemporaryEffects())
        {
            tempStates.push_back(effect);
        }
    }

    // resetting temporary states to false
    for(const auto* state : tempStates)
    {
        goapState->getStateData(*state).m_complete = false;
    }

    m_actions.clear();

    m_isExecuting = false;

    co_return;
}

void SGCore::GOAP::Plan::calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    m_cost = 0.0f;

    for(const auto& action : m_actions)
    {
        action->calculateCost(registry, forEntity);
        m_cost += action->getCost();
    }
}

float SGCore::GOAP::Plan::getCost() const noexcept
{
    return m_cost;
}

bool SGCore::GOAP::Plan::isExecuting() const noexcept
{
    return m_isExecuting;
}
