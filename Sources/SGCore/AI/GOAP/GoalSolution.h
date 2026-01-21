//
// Created by stuka on 18.01.2026.
//

#pragma once

#include "Plan.h"

namespace SGCore::GOAP
{
    /**
     * A set of plans that are suitable for the goal.
     */
    struct GoalSolution
    {
        /**
         * @return Plan with the lowest cost
         */
        Plan& getBestPlan() noexcept;

        /**
         * @return Plan with the lowest cost
         */
        const Plan& getBestPlan() const noexcept;

        /**
         * Insert one suitable plan.
         * @param plan Suitable plan.
         */
        void insertPlan(Plan plan) noexcept;

        /**
         * Insert collection of suitable plans.
         * @param plans Suitable plans.
         */
        void insertPlans(const std::vector<Plan>& plans) noexcept;

        /**
         * Pause all plans in solution.
         */
        void pausePlans() noexcept;

        /**
         * @return All suitable plans
         */
        const std::vector<Plan>& getPlans() const noexcept;

    private:
        std::vector<Plan> m_possiblePlans;
    };
}