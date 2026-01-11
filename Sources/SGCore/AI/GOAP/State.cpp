//
// Created by stuka on 11.01.2026.
//

#include "State.h"

void SGCore::GOAP::EntityState::setState(const State& state, bool completion) noexcept
{
    m_states[&state] = completion;
}

bool SGCore::GOAP::EntityState::isStateComplete(const State& state) const noexcept
{
    const auto it = m_states.find(&state);
    if(it == m_states.end()) return false;

    return it->second;
}
