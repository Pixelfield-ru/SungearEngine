//
// Created by stuka on 11.01.2026.
//

#include "Solver.h"

#include "Plan.h"
#include "Goal.h"
#include "IAction.h"
#include "SGCore/ECS/Registry.h"

std::optional<SGCore::GOAP::Plan> SGCore::GOAP::Solver::resolveGoal(ECS::registry_t& registry,
                                                                    ECS::entity_t forEntity,
                                                                    const Goal& goal) const noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) return std::nullopt;

    std::vector<Ref<IAction>> finalActions;

    // collecting possible last actions
    for(const auto& action : m_availableActions)
    {
        for(const auto& effect : action->getEffects())
        {
            if(goal.getFinalStates().contains(effect))
            {
                finalActions.push_back(action);
            }
        }
    }

    // goal is unreachable
    if(finalActions.empty()) return std::nullopt;

    // getting all actions that have those effects witch satisfy current action`s preconditions
    const auto getBestActionsForAction = [this](const Ref<IAction>& action) {
        std::vector<Ref<IAction>> bestActions;

        for(const auto& availableAction : m_availableActions)
        {
            bool hasAllEffects = true;

            for(const auto& effect : availableAction->getEffects())
            {
                if(!action->hasPrecondition(*effect))
                {
                    hasAllEffects = false;
                    break;
                }
            }

            if(hasAllEffects)
            {
                bestActions.push_back(availableAction);
            }
        }

        return bestActions;
    };

    // recursively get all plans
    const auto getAllPlans = [&](this auto&& self, const Ref<IAction>& action, Plan& currentPlan, std::vector<Plan>& outPlans) {
        const std::vector<Ref<IAction>> bestActions = getBestActionsForAction(action);

        if(bestActions.empty())
        {
            // if first action cannot be executed because of incompleted preconditions in EntityState
            // then dont pushing new plan in vector. taking rbegin() because actions are in reversed order in current moment
            if(!currentPlan.m_actions.empty() &&
               !(*currentPlan.m_actions.rbegin())->preconditionsComplete(registry, forEntity))
            {
                return;
            }

            outPlans.push_back(currentPlan);
            return;
        }

        // if we have one branch in tree then using currentPlan to avoid duplicating same plans
        currentPlan.m_actions.push_back(bestActions[0]);
        self({ bestActions[0] }, currentPlan, outPlans);

        if(bestActions.size() == 1) return;

        // if more then one branch then copying currentPlan to construct new plan
        for(size_t i = 1; i < bestActions.size(); i++)
        {
            auto newPlan = currentPlan;
            newPlan.m_actions.push_back(bestActions[i]);
            self(bestActions[i], newPlan, outPlans);
        }
    };

    // all available plans
    std::vector<Plan> plans;

    // collecting all plans based on last action
    for(const auto& finalAction : finalActions)
    {
        Plan plan{};
        plan.m_actions.push_back(finalAction);
        getAllPlans(finalAction, plan, plans);
    }

    if(plans.empty()) return std::nullopt;

    // reversing actions order in plan and calculating plan cost
    for(auto& plan : plans)
    {
        std::ranges::reverse(plan.m_actions);
        plan.calculateCost(registry, forEntity);
    }

    const auto cheapestPlanIt =
            std::ranges::min_element(plans,
                                     std::less<> { },
                                     &Plan::getCost);

    if(cheapestPlanIt == plans.end()) return std::nullopt;

    return *cheapestPlanIt;
}
