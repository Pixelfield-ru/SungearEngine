//
// Created by stuka on 18.01.2026.
//

#pragma once

#include "Plan.h"

namespace SGCore::GOAP
{
    struct GoalSolution
    {
        Plan& getBestPlan() noexcept;
        const Plan& getBestPlan() const noexcept;

        void insertPlan(Plan plan) noexcept;
        void insertPlans(const std::vector<Plan>& plans) noexcept;

        const std::vector<Plan>& getPlans() const noexcept;

    private:
        std::vector<Plan> m_possiblePlans;
    };
}