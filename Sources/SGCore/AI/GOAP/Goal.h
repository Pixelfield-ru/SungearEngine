//
// Created by stuka on 10.01.2026.
//

#pragma once

#include <unordered_set>

#include "State.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::GOAP
{
    /**
     * Goal that must be reached by entity.
     */
    struct Goal
    {
        /// Priority of plan in queue.
        std::int32_t m_priority {};

        /**
         * Adds state that must be complete after plan execution.
         * @param state State.
         */
        void addFinalState(const State& state) noexcept;

        /**
         * Removes state that must be complete after plan execution.
         * @param state State.
         */
        void removeFinalState(const State& state) noexcept;

        /**
         * @param registry Scene ECS registry.
         * @param forEntity Entity which is used to get entity state.
         * @return Does all final states of goal are complete.
         */
        bool statesComplete(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept;

        /**
         * @param entityState State of the entity.
         * @return Does all final states of goal are complete.
         */
        bool statesComplete(const EntityState& entityState) const noexcept;

        /**
         * @return All final states that must be complete in the end of plan.
         */
        const std::unordered_set<const State*>& getFinalStates() const noexcept;

    private:
        std::unordered_set<const State*> m_finalStates;
    };
}
