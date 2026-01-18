//
// Created by stuka on 18.01.2026.
//

#include "DecisionMaker.h"

#include "Goal.h"
#include "Solver.h"

SGCore::Coro::Task<> SGCore::GOAP::DecisionMaker::executeSolutions(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    bool isAnyPlanExecuted = false;

    while(!m_queue.empty())
    {
        for(auto& plan : m_queue[0].second.getPlans())
        {
            const bool executed = co_await plan.execute(registry, forEntity);
            // if one of any plans has been executed then leaving
            if(executed)
            {
                isAnyPlanExecuted = true;
                break;
            }
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
        const bool executed = co_await plan.execute(registry, forEntity);
        // if one of any plans has been executed then leaving
        if(executed)
        {
            co_return;
        }
    }
}

void SGCore::GOAP::DecisionMaker::addGoal(const Goal& goal, const Solver& goapSolver, ECS::registry_t& registry,
                                          ECS::entity_t forEntity) noexcept
{
    auto goalSolution = goapSolver.resolveGoal(registry, forEntity, goal);
    m_queue.push_back({ goal.m_priority, std::move(goalSolution) });

    std::ranges::sort(m_queue, [](const auto& left, const auto& right) {
        return left.first < right.first;
    });
}
