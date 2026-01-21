//
// Created by stuka on 18.01.2026.
//

#include "GoalSolution.h"

const SGCore::GOAP::Plan& SGCore::GOAP::GoalSolution::getBestPlan() const noexcept
{
    return m_possiblePlans[0];
}

void SGCore::GOAP::GoalSolution::insertPlan(Plan plan) noexcept
{
    m_possiblePlans.push_back(std::move(plan));

    std::ranges::sort(m_possiblePlans, std::less<>{}, &Plan::getCost);
}

void SGCore::GOAP::GoalSolution::insertPlans(const std::vector<Plan>& plans) noexcept
{
    for(const auto& plan : plans)
    {
        m_possiblePlans.push_back(plan);
    }

    std::ranges::sort(m_possiblePlans, std::less<>{}, &Plan::getCost);
}

void SGCore::GOAP::GoalSolution::pausePlans() noexcept
{
    for(auto& plan : m_possiblePlans)
    {
        plan.pause();
    }
}

const std::vector<SGCore::GOAP::Plan>& SGCore::GOAP::GoalSolution::getPlans() const noexcept
{
    return m_possiblePlans;
}

SGCore::GOAP::Plan& SGCore::GOAP::GoalSolution::getBestPlan() noexcept
{
    return m_possiblePlans[0];
}
