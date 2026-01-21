//
// Created by stuka on 10.01.2026.
//

#include "Plan.h"

#include "SGCore/ECS/Registry.h"

SGCore::Coro::Task<SGCore::GOAP::ExecutionResult> SGCore::GOAP::Plan::execute(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept
{
    m_isPaused = false;
    m_isExecuting = true;

    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState)
    {
        m_isExecuting = false;
        co_return ExecutionResult::EXEC_FAILED;
    }

    m_currentExecutableActions = m_actions;

    bool planFailed = false;

    auto it = m_currentExecutableActions.begin();
    while(it != m_currentExecutableActions.end())
    {
        // checking is plan paused before action execution (to prevent action execution)
        if(isPaused())
        {
            co_return ExecutionResult::EXEC_PAUSED;
        }

        const auto action = *it;

        const auto actionExecutionResult = co_await action->execute(registry, forEntity, *this);
        if(actionExecutionResult == ExecutionResult::EXEC_FAILED)
        {
            planFailed = true;
            break;
        }

        if(actionExecutionResult == ExecutionResult::EXEC_PAUSED)
        {
            // pausing plan because action can use custom logic to pause plan
            m_isPaused = true;
            m_isExecuting = false;
            co_return ExecutionResult::EXEC_PAUSED;
        }

        // collecting temporary states to reset
        for(const auto* effect : action->getTemporaryEffects())
        {
            m_temporaryStates.push_back(effect);
        }

        // deleting executed actions
        it = m_currentExecutableActions.erase(it);
    }

    // resetting temporary states to false
    for(const auto* state : m_temporaryStates)
    {
        goapState->getStateData(*state).m_complete = false;
    }

    m_temporaryStates.clear();
    m_currentExecutableActions.clear();

    // m_actions.clear();

    m_isExecuting = false;

    co_return planFailed ? ExecutionResult::EXEC_FAILED : ExecutionResult::EXEC_SUCCESS;
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

void SGCore::GOAP::Plan::pause() noexcept
{
    m_isPaused = true;
    m_isExecuting = false;
}

bool SGCore::GOAP::Plan::isPaused() const noexcept
{
    return m_isPaused;
}

bool SGCore::GOAP::Plan::isExecuting() const noexcept
{
    return m_isExecuting;
}
