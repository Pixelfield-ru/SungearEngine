//
// Created by stuka on 10.01.2026.
//

#include "Goal.h"

#include "SGCore/ECS/Registry.h"

void SGCore::GOAP::Goal::addFinalState(const State& state) noexcept
{
    m_finalStates.insert(&state);
}

void SGCore::GOAP::Goal::removeFinalState(const State& state) noexcept
{
    m_finalStates.erase(&state);
}

bool SGCore::GOAP::Goal::statesComplete(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept
{
    const auto* goapState = registry.tryGet<EntityState>(forEntity);

    return std::ranges::all_of(m_finalStates, [&](const State* state) {
        return goapState->isStateComplete(*state);
    });
}

bool SGCore::GOAP::Goal::statesComplete(const EntityState& entityState) const noexcept
{
    return std::ranges::all_of(m_finalStates, [&](const State* state) {
        return entityState.isStateComplete(*state);
    });
}

const std::unordered_set<const SGCore::GOAP::State*>& SGCore::GOAP::Goal::getFinalStates() const noexcept
{
    return m_finalStates;
}
