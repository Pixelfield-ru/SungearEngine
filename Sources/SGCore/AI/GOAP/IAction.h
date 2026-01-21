//
// Created by stuka on 10.01.2026.
//

#pragma once

#include "ExecutionResult.h"
#include "State.h"
#include "SGCore/Coro/Task.h"

namespace SGCore::GOAP
{
    struct Plan;

    /**
     * GOAP atomic action. Used in plan.
     */
    struct IAction
    {
        virtual ~IAction() = default;

        /**
         * Executes action.
         * @param registry Scene ECS registry.
         * @param forEntity Entity that will follow this action.
         * @param plan Parent plan.
         * @return Action execution result.
         */
        Coro::Task<ExecutionResult> execute(ECS::registry_t& registry, ECS::entity_t forEntity, const Plan& plan) noexcept;

        /**
         * Calculates cost of action. You can implement your custom logic to account entity stats etc.
         * @param registry Scene ECS registry.
         * @param forEntity Entity for what cost is calculating.
         */
        virtual void calculateCost(ECS::registry_t& registry, ECS::entity_t forEntity) noexcept = 0;

        /**
         * Adds precondition state without which the action cannot be executed.
         * @param preconditionState Precondition state.
         */
        void addPrecondition(const State& preconditionState) noexcept;

        /**
         * @param preconditionState
         * @return Whether the action has a precondition.
         */
        bool hasPrecondition(const State& preconditionState) const noexcept;

        /**
         * Removes precondition state without which the action cannot be executed.
         * @param preconditionState Precondition state.
         */
        void removePrecondition(const State& preconditionState) noexcept;

        /**
         * @param registry Scene ECS registry.
         * @param forEntity Entity to take state from.
         * @return Whether the preconditions for the execution of the action are complete.
         */
        bool preconditionsComplete(ECS::registry_t& registry, ECS::entity_t forEntity) const noexcept;

        /**
         * @param entityState Entity state in world.
         * @return Whether the preconditions for the execution of the action are complete.
         */
        bool preconditionsComplete(const EntityState& entityState) const noexcept;

        /**
         * @return All the preconditions that are necessary to execute the action.
         */
        const std::unordered_set<const State*>& getPreconditions() const noexcept;

        /**
         * Adds effect (state) that will be applied to EntityState after action execution.
         * @param effectState State.
         * @param effectValue State value.
         * @param isTemporary Is state must be reset (to 'false') after plan execution.
         */
        void addEffect(const State& effectState, bool effectValue, bool isTemporary = false) noexcept;

        /**
         * @param effectState State.
         * @return Does action has effect state.
         */
        bool hasEffect(const State& effectState) const noexcept;

        /**
         * Removes effect (state) that will be applied to EntityState after action execution.
         * @param effectState State.
         */
        void removeEffect(const State& effectState) noexcept;

        /**
         * @return All the effects that will be applied to EntityState after action execution.
         */
        const std::unordered_map<const State*, bool>& getEffects() const noexcept;

        /**
         * @return Only temporary effects that will be applied to EntityState after action execution.
         */
        const std::vector<const State*>& getTemporaryEffects() const noexcept;

        /**
         * @return Calculated cost.
         */
        float getCost() const noexcept;

        /**
         * @return Instance of this action.
         */
        virtual IAction* clone() const noexcept = 0;

    protected:
        /**
         * User-defined implementation of action.
         * @param registry Scene ECS registry.
         * @param forEntity Entity that will follow this action.
         * @param plan Parent plan.
         * @return Action execution result.
         */
        virtual Coro::Task<ExecutionResult> executeImpl(ECS::registry_t& registry, ECS::entity_t forEntity, const Plan& plan) noexcept = 0;

        /**
         * Cost of action.
         */
        float m_cost = 0.0f;

    private:
        std::unordered_set<const State*> m_preconditions;
        std::unordered_map<const State*, bool> m_effects;
        std::vector<const State*> m_temporaryEffects;
    };
}
