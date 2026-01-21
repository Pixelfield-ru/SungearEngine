//
// Created by stuka on 11.01.2026.
//

#pragma once

#include <vector>

#include "GoalSolution.h"
#include "SGCore/Main/CoreGlobals.h"
#include "IAction.h"

namespace SGCore::GOAP
{
    struct Plan;
    struct Goal;
    struct IAction;

    struct Solver
    {
        /**
         * Builds collection of plans (solution) that can be used to reach goal.
         * @param registry Scene ECS registry
         * @param forEntity Entity that will be used to execute solution. Also, entity is used to take its state.
         * @param goal Custom goal.
         * @return Solution to reach goal.
         */
        GoalSolution resolveGoal(ECS::registry_t& registry, ECS::entity_t forEntity, const Goal& goal) const noexcept;

        /**
         * Registers new action that can be used to solve goals.
         * @tparam ActionT Type of user-defined action.
         * @tparam CtorArgs Types of constructor argument to construct action.
         * @param args Action constructor arguments.
         */
        template<typename ActionT, typename... CtorArgs>
        void registerActionType(CtorArgs&&... args) noexcept
        {
            static auto& actionTypeInfo = typeid(ActionT);

            for(const auto& action : m_availableActions)
            {
                if(typeid(*action).hash_code() == actionTypeInfo.hash_code())
                {
                    return;
                }
            }

            m_availableActions.push_back(MakeRef<ActionT>(std::forward<CtorArgs>(args)...));
        }

        /**
         * @tparam ActionT Type of user-defined action.
         * @return Action with type ActionT or nullptr if not found.
         */
        template<typename ActionT>
        Ref<IAction> getAction() const noexcept
        {
            static auto& actionTypeInfo = typeid(ActionT);

            for(const auto& action : m_availableActions)
            {
                if(typeid(*action).hash_code() == actionTypeInfo.hash_code())
                {
                    return action;
                }
            }

            return nullptr;
        }

    private:
        /**
         * All registered actions.
         */
        std::vector<Ref<IAction>> m_availableActions;
    };
}
