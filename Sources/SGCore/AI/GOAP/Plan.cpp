//
// Created by stuka on 10.01.2026.
//

#include "Plan.h"

#include "SGCore/ECS/Registry.h"

SGCore::Coro::Task<bool> SGCore::GOAP::Plan::execute(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept
{
    m_isExecuting = true;

    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState)
    {
        m_isExecuting = false;
        co_return false;
    }

    std::vector<const State*> tempStates;

    bool planComplete = true;

    for(const auto& action : m_actions)
    {
        if(!co_await action->execute(registry, forEntity))
        {
            planComplete = false;
            break;
        }

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

    // m_actions.clear();

    m_isExecuting = false;

    co_return planComplete;
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
