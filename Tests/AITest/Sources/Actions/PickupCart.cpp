//
// Created by stuka on 16.01.2026.
//

#include "PickupCart.h"

#include "../States.h"
#include "SGCore/ECS/Registry.h"

PickupCart::PickupCart() noexcept
{
    addPrecondition(SGCore::GOAP::States::IN_POSITION);
    addPrecondition(CART_FOUND);

    addEffect(HAS_CART, true);
    addEffect(SGCore::GOAP::States::IN_POSITION, false);
    addEffect(CART_FOUND, false);
}

void PickupCart::calculateCost(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t forEntity) noexcept
{
    m_cost = 10.0f;
}

SGCore::GOAP::IAction* PickupCart::clone() const noexcept
{
    return new PickupCart(*this);
}

SGCore::Coro::Task<SGCore::GOAP::ExecutionResult> PickupCart::executeImpl(SGCore::ECS::registry_t& registry,
                                                                          SGCore::ECS::entity_t forEntity,
                                                                          const SGCore::GOAP::Plan& plan) noexcept
{
    auto* goapState = registry.tryGet<SGCore::GOAP::EntityState>(forEntity);
    if(!goapState) co_return SGCore::GOAP::ExecutionResult::EXEC_FAILED;

    const auto& cartEntityData = goapState->getStateData(CART_FOUND).m_data;

    static const auto& entityTypeInfo = typeid(SGCore::ECS::entity_t);

    if(cartEntityData.type() != entityTypeInfo) co_return SGCore::GOAP::ExecutionResult::EXEC_FAILED;

    const auto cartEntity = std::any_cast<SGCore::ECS::entity_t>(cartEntityData);

    std::cout << std::format("picked cart {}", std::to_underlying(cartEntity)) << std::endl;

    co_return SGCore::GOAP::ExecutionResult::EXEC_SUCCESS;
}
