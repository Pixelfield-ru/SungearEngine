//
// Created by stuka on 26.11.2023.
//

#ifndef SUNGEARENGINE_PBRRPDIRECTIONALLIGHTSPASS_H
#define SUNGEARENGINE_PBRRPDIRECTIONALLIGHTSPASS_H

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct PBRRPDirectionalLightsPass : public IRenderPass
    {
        // todo: сделать рендер пассы с настройкой времени выполнения. то же самое сделать с системами
        
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        Timer m_renderTimer { true };
        
        EventListener<void(const double&, const double&)> m_renderListener = [](const auto&, const auto&) { };
    };
}

#endif //SUNGEARENGINE_PBRRPDIRECTIONALLIGHTSPASS_H
