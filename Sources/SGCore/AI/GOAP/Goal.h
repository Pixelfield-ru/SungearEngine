//
// Created by stuka on 10.01.2026.
//

#pragma once

#include <unordered_set>

#include "State.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::GOAP
{
    struct Goal
    {
        void addFinalState(const State& state) noexcept;
        void removeFinalState(const State& state) noexcept;
        bool statesComplete(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept;
        bool statesComplete(const EntityState& entityState) const noexcept;

        const std::unordered_set<const State*>& getFinalStates() const noexcept;

    private:
        std::unordered_set<const State*> m_finalStates;
    };
}
