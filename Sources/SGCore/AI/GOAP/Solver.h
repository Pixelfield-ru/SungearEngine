//
// Created by stuka on 11.01.2026.
//

#pragma once

#include <vector>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Macroses.h"
#include "IAction.h"

namespace SGCore::GOAP
{
    struct Plan;
    struct Goal;
    struct IAction;

    struct Solver
    {
        Plan resolveGoal(ECS::registry_t& registry, ECS::entity_t forEntity, const Goal& goal) const noexcept;

        template<typename ActionT, typename... CtorArgs>
        void registerActionType(CtorArgs&&... args) noexcept
        {
            for(const auto& action : s_availableActions)
            {
                if(typeid(*action).hash_code() == typeid(ActionT).hash_code())
                {
                    return;
                }
            }

            s_availableActions.push_back(MakeRef<ActionT>(std::forward<CtorArgs>(args)...));
        }

        template<typename ActionT>
        Ref<IAction> getAction() const noexcept
        {
            for(const auto& action : s_availableActions)
            {
                if(typeid(*action).hash_code() == typeid(ActionT).hash_code())
                {
                    return action;
                }
            }

            return nullptr;
        }

    private:
        std::vector<Ref<IAction>> s_availableActions;
    };
}
