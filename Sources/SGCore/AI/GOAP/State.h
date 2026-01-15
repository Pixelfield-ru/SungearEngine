//
// Created by stuka on 11.01.2026.
//

#pragma once

#include <unordered_map>

#include "SGCore/ECS/Component.h"
#include "SGCore/Utils/Macroses.h"

#define SG_DECLARE_GOAP_STATE(stateName)    \
    struct SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL)) : SGCore::GOAP::State {};   \
    extern SG_CONCAT(STATE_, SG_CONCAT(stateName, _IMPL)) stateName;


namespace SGCore::GOAP
{
    struct State {};

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

    struct EntityState : ECS::Component<EntityState, const EntityState>
    {
        void setState(const State& state, bool completion) noexcept;
        bool isStateComplete(const State& state) const noexcept;

    private:
        std::unordered_map<const State*, bool> m_states;
    };
}