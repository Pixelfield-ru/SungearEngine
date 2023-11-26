//
// Created by stuka on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRFRPSHADOWSPASS_H
#define SUNGEARENGINE_PBRFRPSHADOWSPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"
#include "SGCore/Utils/Timer.h"

namespace SGCore
{
    struct PBRFRPShadowsPass : public IRenderPass
    {
        PBRFRPShadowsPass()
        {
            m_renderTimer.m_useFixedUpdateCatchUp = false;
            m_renderTimer.m_targetFrameRate = 24;

            m_renderTimer.addCallback(m_renderTimerCallback);
        }

        void render(const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        Ref<TimerCallback> m_renderTimerCallback = MakeRef<TimerCallback>();
        Timer m_renderTimer { true };
    };
}

#endif //SUNGEARENGINE_PBRFRPSHADOWSPASS_H
