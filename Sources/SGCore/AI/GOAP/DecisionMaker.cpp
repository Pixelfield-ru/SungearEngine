//
// Created by stuka on 18.01.2026.
//

#include "DecisionMaker.h"

#include "Goal.h"
#include "Solver.h"

SGCore::Coro::Task<> SGCore::GOAP::DecisionMaker::executeSolutions(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    // todo: finalize

    while(!m_queue.empty())
    {
        bool isAnyPlanExecuted = false;
        bool isAnyPlanStopped = false;

        const auto& plans = m_queue[0].second.getPlans();

        for(auto& plan : plans)
        {
            const auto executionResult = co_await plan.execute(registry, forEntity);

            // if one of any plans has been executed then leaving
            if(executionResult == ExecutionResult::EXEC_SUCCESS)
            {
                isAnyPlanExecuted = true;
                break;
            }

            if(executionResult == ExecutionResult::EXEC_PAUSED)
            {
                isAnyPlanStopped = true;
                break;
            }
        }

        if(isAnyPlanStopped)
        {
            // skipping current iteration to process new plan with higher priority or continue plan in next iteration
            continue;
        }

        if(isAnyPlanExecuted)
        {
            // erasing executed solution
            m_queue.erase(m_queue.begin());
            continue;
        }

        // postponing solution
        m_queue.push_back(*m_queue.begin());
        m_queue.erase(m_queue.begin());
    }

    // fallback to default solution when other solutions are over
    for(auto& plan : m_fallback.getPlans())
    {
        const auto executionResult = co_await plan.execute(registry, forEntity);
        // if one of any plans has been executed then leaving
        if(executionResult == ExecutionResult::EXEC_SUCCESS)
        {
            co_return;
        }
    }
}

void SGCore::GOAP::DecisionMaker::addGoal(const Goal& goal, const Solver& goapSolver, ECS::registry_t& registry,
                                          ECS::entity_t forEntity) noexcept
{
    const auto [lastPlanPriority, lastPlanSolution] = m_queue[0];

    auto goalSolution = goapSolver.resolveGoal(registry, forEntity, goal);
    m_queue.push_back({ goal.m_priority, std::move(goalSolution) });

    std::ranges::sort(m_queue, [](const auto& left, const auto& right) {
        return left.first < right.first;
    });

    if(goal.m_priority > lastPlanPriority)
    {
        // pausing all plans in last solution (to start new plan with higher priority)
        m_queue[1].second.pausePlans();
    }
}
