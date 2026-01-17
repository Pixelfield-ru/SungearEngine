//
// Created by stuka on 11.01.2026.
//

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/scalar_multiplication.hpp>

#include "Goto.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Transformations/Transform.h"

SGCore::GOAP::Goto::Goto() noexcept
{
    addPrecondition(States::POSITION_FOUND);

    addEffect(States::IN_POSITION, true);
}

SGCore::Coro::Task<bool> SGCore::GOAP::Goto::executeImpl(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    auto* goapState = registry.tryGet<EntityState>(forEntity);
    if(!goapState) co_return false;

    auto* tmpTransform = registry.tryGet<Transform>(forEntity);
    if(!tmpTransform) co_return false;

    const auto& positionData = goapState->getStateData(States::POSITION_FOUND).m_data;

    static const auto& vec3TypeInfo = typeid(glm::vec3);

    if(positionData.type() != vec3TypeInfo)
    {
        co_return false;
    }

    const auto targetPosition = std::any_cast<glm::vec3>(positionData);

    auto& transform = *tmpTransform;

    auto currentThread = Threading::ThreadsManager::currentThread();
    while(glm::distance(transform->m_finalTransform.m_position, targetPosition) > m_distanceErrorRate)
    {
        const double dt = currentThread->getDeltaTime();
        transform->m_ownTransform.m_position += glm::normalize(targetPosition - transform->m_finalTransform.m_position) * m_speed * dt;

        co_await Coro::returnToCaller();
    }

    co_return true;
}

void SGCore::GOAP::Goto::calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept
{
    m_cost = 10.0f;
}

SGCore::GOAP::IAction* SGCore::GOAP::Goto::clone() const noexcept
{
    return new Goto(*this);
}
