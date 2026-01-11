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

    SG_DECLARE_GOAP_STATE(HAS_WEAPON)
    SG_DECLARE_GOAP_STATE(HAS_AMMO)
    SG_DECLARE_GOAP_STATE(IN_CAR)
    SG_DECLARE_GOAP_STATE(IN_BOAT)
    SG_DECLARE_GOAP_STATE(IN_AIRPLANE)
    SG_DECLARE_GOAP_STATE(ALIVE)
    SG_DECLARE_GOAP_STATE(IN_POSITION)

    struct EntityState : ECS::Component<EntityState, const EntityState>
    {
        void setState(const State& state, bool completion) noexcept;
        bool isStateComplete(const State& state) const noexcept;

    private:
        std::unordered_map<const State*, bool> m_states;
    };
}