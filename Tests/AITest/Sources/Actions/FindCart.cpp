//
// Created by stuka on 16.01.2026.
//

#include "FindCart.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "../States.h"
#include "../Components/Cart.h"
#include "SGCore/ECS/Registry.h"
#include "SGCore/Transformations/Transform.h"

FindCart::FindCart() noexcept
{
    addEffect(SGCore::GOAP::States::POSITION_FOUND, true, true);
    addEffect(CART_FOUND, true, true);
}

void FindCart::calculateCost(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t forEntity) noexcept
{
    m_cost = 10.0f;
}

SGCore::GOAP::IAction* FindCart::clone() const noexcept
{
    return new FindCart(*this);
}

SGCore::Coro::Task<bool> FindCart::executeImpl(SGCore::ECS::registry_t& registry,
                                               SGCore::ECS::entity_t forEntity) noexcept
{
    auto* goapState = registry.tryGet<SGCore::GOAP::EntityState>(forEntity);
    if(!goapState) co_return false;

    auto* tmpTransform = registry.tryGet<SGCore::Transform>(forEntity);
    if(!tmpTransform) co_return false;

    const auto& entityTransform = *tmpTransform;
    const auto& entityPosition = entityTransform->m_finalTransform.m_position;

    std::pair<glm::vec3, SGCore::ECS::entity_t> nearestCart {
        std::numeric_limits<float>::max(),
        entt::null
    };

    auto cartsView = registry.view<Cart, SGCore::Transform>();
    cartsView.each([&](auto cartEntity, auto cart, const SGCore::Ref<SGCore::Transform>& transform) {
        const auto& cartPosition = transform->m_finalTransform.m_position;

        if(glm::distance2(cartPosition, entityPosition) > glm::distance2(nearestCart.first, entityPosition))
        {
            return;
        }

        nearestCart.first = transform->m_finalTransform.m_position;
        nearestCart.second = cartEntity;
    });

    goapState->getStateData(SGCore::GOAP::States::POSITION_FOUND).m_data = nearestCart.first;
    goapState->getStateData(CART_FOUND).m_data = nearestCart.second;

    co_return true;
}
