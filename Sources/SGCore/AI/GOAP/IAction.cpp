//
// Created by stuka on 10.01.2026.
//

#include "IAction.h"

#include "SGCore/ECS/Registry.h"

SGCore::Coro::Task<> SGCore::GOAP::IAction::execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) co_return;

    if(!preconditionsComplete(registry, forEntity))
    {
        co_return;
    }

    // const bool s = co_await Coro::returnToCaller();
    const bool complete = co_await executeImpl(registry, forEntity);

    if(!complete) co_return;

    for(const auto& effectState : m_effects)
    {
        goapState->setState(*effectState, true);
    }
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

    for(const auto& cond : m_preconditions)
    {
        if(!goapState->isStateComplete(*cond)) return false;
    }

    return true;
}

float SGCore::GOAP::IAction::getCost() const noexcept
{
    return m_cost;
}
