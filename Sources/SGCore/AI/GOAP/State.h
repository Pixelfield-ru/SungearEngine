//
// Created by stuka on 11.01.2026.
//

#pragma once

#include <any>
#include <unordered_map>

#include "SGCore/ECS/Component.h"
#include "SGCore/Utils/Macroses.h"

/**
 * Used to declare state.\n
 * Note: use only in header files.
 * @param stateName
 */
#define SG_DECLARE_GOAP_STATE(stateName)                                                    \
    struct SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL)) : SGCore::GOAP::State             \
    {                                                                                       \
        static const SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL))& instance() noexcept;   \
    };                                                                                      \
    static const SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL))& stateName = SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL))::instance();

/**
 * Used to implement state.\n
 * Note: use only in .cpp file.\n
 * Note: pass only name of state. If you have states have namespace, then call this macro in namespace in .cpp file.
 * @param stateName Name of state.
 */
#define SG_IMPLEMENT_GOAP_STATE(stateName) const SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL))& SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL))::instance() noexcept    \
    {                                                                                                                                                                   \
        static SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL)) instance;                                                                                                 \
        return instance;                                                                                                                                                \
    };

namespace SGCore::GOAP
{
    struct State {};

    /**
     * Builtin states.
     */
    namespace States
    {
        /// Entity has a weapon.
        SG_DECLARE_GOAP_STATE(HAS_WEAPON)
        /// Entity has ammo in weapon.
        SG_DECLARE_GOAP_STATE(HAS_AMMO)
        /// Entity is in a car.
        SG_DECLARE_GOAP_STATE(IN_CAR)
        /// Entity is in a boat.
        SG_DECLARE_GOAP_STATE(IN_BOAT)
        /// Entity is in an airplane.
        SG_DECLARE_GOAP_STATE(IN_AIRPLANE)
        /// Entity is alive.
        SG_DECLARE_GOAP_STATE(ALIVE)
        /// Entity has come to point.
        SG_DECLARE_GOAP_STATE(IN_POSITION)
        /// Entity has found point.
        SG_DECLARE_GOAP_STATE(POSITION_FOUND)
    }

    /**
     * Holds completion of state and user data.
     */
    struct StateData
    {
        bool m_complete = false;
        std::any m_data;
    };

    /**
     * All state of entity.
     */
    struct EntityState : ECS::Component<EntityState, const EntityState>
    {
        /**
         * @param state State.
         * @return State data.
         */
        StateData& getStateData(const State& state) noexcept;

        /**
         * @param state State.
         * @return Whether state is complete.
         */
        bool isStateComplete(const State& state) const noexcept;

    private:
        std::unordered_map<const State*, StateData> m_states;
    };
}