//
// Created by stuka on 10.01.2026.
//

#include "IAction.h"

#include "SGCore/ECS/Registry.h"

SGCore::Coro::Task<bool> SGCore::GOAP::IAction::execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) co_return false;

    if(!preconditionsComplete(registry, forEntity))
    {
        co_return false;
    }

    // const bool s = co_await Coro::returnToCaller();
    const bool complete = co_await executeImpl(registry, forEntity);

    if(!complete) co_return false;

    for(const auto& [effectState, effectValue] : m_effects)
    {
        goapState->getStateData(*effectState).m_complete = effectValue;
    }

    co_return true;
}

void SGCore::GOAP::IAction::addPrecondition(const State& preconditionState) noexcept
{
    m_preconditions.insert(&preconditionState);
}

bool SGCore::GOAP::IAction::hasPrecondition(const State& preconditionState) const noexcept
{
    return m_preconditions.contains(&preconditionState);
}

void SGCore::GOAP::IAction::removePrecondition(const State& preconditionState) noexcept
{
    m_preconditions.erase(&preconditionState);
}

bool SGCore::GOAP::IAction::preconditionsComplete(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) return false;

    return std::ranges::all_of(m_preconditions, [&](const State* state) {
       return goapState->isStateComplete(*state);
    });
}

bool SGCore::GOAP::IAction::preconditionsComplete(const EntityState& entityState) const noexcept
{
    return std::ranges::all_of(m_preconditions, [&](const State* state) {
       return entityState.isStateComplete(*state);
    });
}

const std::unordered_set<const SGCore::GOAP::State*>& SGCore::GOAP::IAction::getPreconditions() const noexcept
{
    return m_preconditions;
}

void SGCore::GOAP::IAction::addEffect(const State& effectState, bool effectValue, bool isTemporary) noexcept
{
    const bool hasSameEffect = hasEffect(effectState);
    m_effects[&effectState] = effectValue;

    if(isTemporary && hasSameEffect)
    {
        m_temporaryEffects.push_back(&effectState);
    }
}

bool SGCore::GOAP::IAction::hasEffect(const State& effectState) const noexcept
{
    return m_effects.contains(&effectState);
}

void SGCore::GOAP::IAction::removeEffect(const State& effectState) noexcept
{
    m_effects.erase(&effectState);
}

const std::unordered_map<const SGCore::GOAP::State*, bool>& SGCore::GOAP::IAction::getEffects() const noexcept
{
    return m_effects;
}

const std::vector<const SGCore::GOAP::State*>& SGCore::GOAP::IAction::getTemporaryEffects() const noexcept
{
    return m_temporaryEffects;
}

float SGCore::GOAP::IAction::getCost() const noexcept
{
    return m_cost;
}
