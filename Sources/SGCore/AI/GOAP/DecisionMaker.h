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

    struct DecisionMaker
    {
        GoalSolution m_fallback;

        Coro::Task<> executeSolutions(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

        void addGoal(const Goal& goal, const Solver& goapSolver, ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

    private:
        // first - goal priority, second - solution for goal
        std::vector<std::pair<std::int32_t, GoalSolution>> m_queue;
    };
}
