//
// Created by stuka on 10.01.2026.
//

#include "Plan.h"

#include "SGCore/ECS/Registry.h"

SGCore::Coro::Task<> SGCore::GOAP::Plan::execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) co_return;

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
        goapState->setState(*state, false);
    }

    m_actions.clear();

    co_return;
}
