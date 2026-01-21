//
// Created by stuka on 10.01.2026.
//

#pragma once

#include "ExecutionResult.h"
#include "IAction.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::GOAP
{
    /**
     * Sequence of actions to reach plan.
     */
    struct Plan
    {
        /**
         * Sequence of actions that must be executed to reach goal.
         */
        std::vector<Ref<IAction>> m_actions;

        /**
         * Executes plan. Also used to continue plan after it was paused.
         * @param registry Scene registry.
         * @param forEntity Entity that will follow plan.
         * @return Execution result.
         */
        Coro::Task<ExecutionResult> execute(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept;

        /**
         * Calculates cost of plan using a simple formula: action0.cost + action1.cost + ...
         * @param registry Scene ECS registry.
         * @param forEntity Entity for what cost is calculating.
         */
        void calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

        /**
         * @return Calculated cost.
         */
        float getCost() const noexcept;

        /**
         * Pauses plan. Note: isExecuting() returns 'false' after call to pause().
         */
        void pause() noexcept;

        bool isPaused() const noexcept;
        bool isExecuting() const noexcept;

    private:
        // used to continue plan from action where plan was stopped
        mutable std::vector<Ref<IAction>> m_currentExecutableActions;
        // used to collect all temporary states when executing plan
        // (it is member variable because plan can be stopped, so we need to store all temp states to revert them later)
        mutable std::vector<const State*> m_temporaryStates;

        mutable bool m_isExecuting = false;
        mutable bool m_isPaused = false;

        float m_cost = 0.0f;
    };
}
