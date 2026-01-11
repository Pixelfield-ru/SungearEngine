//
// Created by stuka on 11.01.2026.
//

#include "Solver.h"

#include "Plan.h"
#include "Goal.h"
#include "IAction.h"

SGCore::GOAP::Plan SGCore::GOAP::Solver::resolveGoal(ECS::registry_t& registry,
                                                     ECS::entity_t forEntity,
                                                     const Goal& goal) const noexcept
{
    return {};
}
