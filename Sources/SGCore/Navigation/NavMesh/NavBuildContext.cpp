//
// Created by stuka on 21.06.2026.
//

#include "NavBuildContext.h"

#include "SGCore/Logger/Logger.h"

#include <string>

void SGCore::NavBuildContext::doResetLog()
{
    // rcContext::doResetLog();
}

void SGCore::NavBuildContext::doLog(rcLogCategory category, const char* msg, int len)
{
    const auto strMsg = std::string(msg, len);
    switch(category)
    {
        case RC_LOG_PROGRESS:
            LOG_I_UNFORMATTED(SGCORE_TAG, strMsg);
            break;
        case RC_LOG_WARNING:
            LOG_W_UNFORMATTED(SGCORE_TAG, strMsg);
            break;
        case RC_LOG_ERROR:
            LOG_E_UNFORMATTED(SGCORE_TAG, strMsg);
            break;
    }
}

void SGCore::NavBuildContext::doResetTimers()
{
    for(size_t i = 0; i < RC_MAX_TIMERS; ++i)
    {
        m_accum[i] = -1;
    }
}

void SGCore::NavBuildContext::doStartTimer(rcTimerLabel label)
{
    const auto start = std::chrono::high_resolution_clock::now();
    m_timers[label] = start;
}

void SGCore::NavBuildContext::doStopTimer(rcTimerLabel label)
{
    const auto end = std::chrono::high_resolution_clock::now();
    const auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timers[label]).count();

    auto& accumValue = m_accum[label];
    if(accumValue == -1)
    {
        accumValue = dif;
    }
    else
    {
        accumValue += dif;
    }
}

int SGCore::NavBuildContext::doGetAccumulatedTime(rcTimerLabel label) const
{
    return m_accum[label];
}
