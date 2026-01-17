//
// Created by stuka on 16.01.2026.
//

#pragma once

#include "SGCore/AI/GOAP/IAction.h"

struct PickupCart : SGCore::GOAP::IAction
{
    PickupCart() noexcept;

    void calculateCost(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t forEntity) noexcept final;

    IAction* clone() const noexcept final;

protected:
    SGCore::Coro::Task<bool> executeImpl(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t forEntity) noexcept final;
};
