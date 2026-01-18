//
// Created by stuka on 11.01.2026.
//

#include "Solver.h"

#include "Plan.h"
#include "Goal.h"
#include "IAction.h"
#include "SGCore/ECS/Registry.h"

std::vector<SGCore::GOAP::Plan> SGCore::GOAP::Solver::resolveGoal(ECS::registry_t& registry,
                                                                  ECS::entity_t forEntity,
                                                                  const Goal& goal) const noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) return {};

    std::vector<Plan> plans;

    const auto findPlans = [&](this auto&& self, Plan& currentPlan,
                               EntityState& currentState) {
        if(goal.statesComplete(currentState))
        {
            plans.push_back(currentPlan);
            return;
        }

        std::vector<Ref<IAction>> nextPossibleActions;

        for(const auto& availableAction : m_availableActions)
        {
            // checking if all preconditions in available action is complete
            if(!availableAction->preconditionsComplete(currentState))
                continue; // action is unavailable

            // checking if any of available actions applies any new effect to entity state
            const bool appliesAnyNewEffect = std::ranges::any_of(
                availableAction->getEffects(),
                [&currentState](const State* state) {
               return !currentState.isStateComplete(*state);
            });

            if(!appliesAnyNewEffect) continue;

            nextPossibleActions.push_back(availableAction);
        }

        if(nextPossibleActions.empty()) return;

        // APPLYING NEW EFFECTS ONLY AFTER SCANNING POSSIBLE NEXT ACTIONS
        // BECAUSE STATE MUST BE IMMUTABLE WHEN SCANNING NEXT ACTIONS!
        for(const auto& nextAction : nextPossibleActions)
        {
            for(const auto& effect : nextAction->getEffects())
            {
                currentState.getStateData(*effect).m_complete = true;
            }
        }

        // if more then one branch then copying currentPlan and currentState to construct new plan and state
        for(size_t i = 1; i < nextPossibleActions.size(); ++i)
        {
            auto newPlan = currentPlan;
            auto newState = currentState;

            newPlan.m_actions.push_back(nextPossibleActions[i]);
            self(newPlan, newState);
        }

        // if we have one branch in tree then using currentPlan and currentState to avoid duplicating same plans and states
        currentPlan.m_actions.push_back(nextPossibleActions[0]);
        self(currentPlan, currentState);
    };

    EntityState currentEntityState = *goapState;
    Plan currentPlan;
    findPlans(currentPlan, currentEntityState);

    // calculating costs of plans
    for(auto& plan : plans)
    {
        plan.calculateCost(registry, forEntity);
    }

    return plans;
}

std::optional<SGCore::GOAP::Plan> SGCore::GOAP::Solver::findBestPlan(const std::vector<Plan>& availablePlans) const noexcept
{
    const auto cheapestPlanIt =
            std::ranges::min_element(availablePlans,
                                     std::less<> { },
                                     &Plan::getCost);

    if(cheapestPlanIt == availablePlans.end()) return std::nullopt;

    return *cheapestPlanIt;
}
