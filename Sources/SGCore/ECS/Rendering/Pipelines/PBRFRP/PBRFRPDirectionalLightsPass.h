//
// Created by stuka on 26.11.2023.
//

#ifndef SUNGEARENGINE_PBRFRPDIRECTIONALLIGHTSPASS_H
#define SUNGEARENGINE_PBRFRPDIRECTIONALLIGHTSPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"

namespace SGCore
{
    struct PBRFRPDirectionalLightsPass : public IRenderPass
    {
        // todo: сделать рендер пассы с настройкой времени выполнения. то же самое сделать с системами
        PBRFRPDirectionalLightsPass()
        {
            m_renderTimer.m_useFixedUpdateCatchUp = false;
            m_renderTimer.m_targetFrameRate = 24;

            m_renderTimer.addCallback(m_renderTimerCallback);
        }

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        Ref<TimerCallback> m_renderTimerCallback = MakeRef<TimerCallback>();
        Timer m_renderTimer { true };
    };
}

#endif //SUNGEARENGINE_PBRFRPDIRECTIONALLIGHTSPASS_H
