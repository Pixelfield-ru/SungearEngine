//
// Created by stuka on 10.01.2026.
//

#pragma once
#include "IAction.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::GOAP
{
    struct Plan
    {
        std::vector<Ref<IAction>> m_actions;

        Coro::Task<> execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;
    };
}
