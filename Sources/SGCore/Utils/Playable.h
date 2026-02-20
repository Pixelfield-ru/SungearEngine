//
// Created by stuka on 20.02.2026.
//

#pragma once
#include "Signal.h"

namespace SGCore
{
    /**
     * State of playable class.
     */
    enum class PlayableState
    {
        /// Playable class is playing at the moment.
        SG_PLAYING,
        /// Playable class is paused.
        SG_PAUSED,
        /// Playable class is stopped. Timeline sets to 0.
        SG_STOPPED
    };

    struct Playable
    {
        virtual ~Playable() noexcept = default;

        void setState(PlayableState state) noexcept
        {
            m_state = state;

            if(m_state == PlayableState::SG_STOPPED)
            {
                resetTimelineTime();
            }

            doSetState(state);
        }

        virtual PlayableState getState() const noexcept
        {
            return m_state;
        }

        void checkIsStateChanged() noexcept
        {
            const auto currentState = getState();
            if(currentState != m_lastState)
            {
                onStateChanged(*this, m_lastState, currentState);
                m_lastState = currentState;
            }
        }

        /// Signal to subscribe to changes in state of playable class.
        Signal<void(Playable& self, PlayableState lastState, PlayableState newState)> onStateChanged;

    private:
        PlayableState m_state = PlayableState::SG_PLAYING;
        PlayableState m_lastState = PlayableState::SG_STOPPED;

        virtual void doSetState(PlayableState state) noexcept {}

        virtual void resetTimelineTime() noexcept = 0;
    };
}
