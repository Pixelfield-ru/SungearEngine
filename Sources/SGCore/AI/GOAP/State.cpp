//
// Created by stuka on 11.01.2026.
//

#include "State.h"

namespace SGCore::GOAP::States
{
    SG_IMPLEMENT_GOAP_STATE(HAS_WEAPON)
    SG_IMPLEMENT_GOAP_STATE(HAS_AMMO)
    SG_IMPLEMENT_GOAP_STATE(IN_CAR)
    SG_IMPLEMENT_GOAP_STATE(IN_BOAT)
    SG_IMPLEMENT_GOAP_STATE(IN_AIRPLANE)
    SG_IMPLEMENT_GOAP_STATE(ALIVE)
    SG_IMPLEMENT_GOAP_STATE(IN_POSITION)
    SG_IMPLEMENT_GOAP_STATE(POSITION_FOUND)
}

SGCore::GOAP::StateData& SGCore::GOAP::EntityState::getStateData(const State& state) noexcept
{
    return m_states[&state];
}

bool SGCore::GOAP::EntityState::isStateComplete(const State& state) const noexcept
{
    const auto stateIt = m_states.find(&state);
    if(stateIt == m_states.end()) return false;

    return stateIt->second.m_complete;
}
