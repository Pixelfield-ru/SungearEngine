//
// Created by stuka on 18.01.2026.
//

#pragma once

#include <queue>

#include "GoalSolution.h"

namespace SGCore::GOAP
{
    struct Goal;
    struct Solver;

    /**
     * Wrapper that can be used as queue of goal solutions.
     */
    struct DecisionMaker
    {
        /// Used as fallback collection of plans if there is no more plans to execute.
        GoalSolution m_fallback;

        /**
         * Executes all solutions in queue by priority.
         * @param registry Scene ECS registry.
         * @param forEntity Entity for which solutions will be executed.
         * @return Async task.
         */
        Coro::Task<> executeSolutions(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

        /**
         * Resolves goal and adds solution for this goal in queue.
         * @param goal Custom goal that must be reached.
         * @param goapSolver Solver that will be used to solve goal.
         * @param registry Scene ECS registry.
         * @param forEntity Entity which must reach goal.
         */
        void addGoal(const Goal& goal, const Solver& goapSolver, ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

    private:
        // first - goal priority, second - solution for goal
        std::vector<std::pair<std::int32_t, GoalSolution>> m_queue;
    };
}
