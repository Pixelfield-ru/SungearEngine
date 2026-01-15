//
// Created by stuka on 10.01.2026.
//

#pragma once

#include "State.h"
#include "SGCore/Coro/Task.h"

namespace SGCore::GOAP
{
    struct IAction
    {
        virtual ~IAction() = default;

        Coro::Task<> execute(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept;

        virtual void calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept = 0;

        void addPrecondition(const State& preconditionState) noexcept;
        bool hasPrecondition(const State& preconditionState) const noexcept;
        void removePrecondition(const State& preconditionState) noexcept;
        bool preconditionsComplete(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept;

        const std::unordered_set<const State*>& getPreconditions() const noexcept;

        void addEffect(const State& effectState, bool isTemporary = false) noexcept;
        bool hasEffect(const State& effectState) const noexcept;
        void removeEffect(const State& effectState) noexcept;

        const std::unordered_set<const State*>& getEffects() const noexcept;
        const std::unordered_set<const State*>& getTemporaryEffects() const noexcept;

        float getCost() const noexcept;

        virtual IAction* clone() const noexcept = 0;

    protected:
        virtual Coro::Task<bool> executeImpl(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept = 0;

        float m_cost = 0.0f;

    private:
        std::unordered_set<const State*> m_preconditions;
        std::unordered_set<const State*> m_effects;
        std::unordered_set<const State*> m_temporaryEffects;
    };
}
