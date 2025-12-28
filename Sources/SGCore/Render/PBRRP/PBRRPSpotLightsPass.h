//
// Created by stuka on 26.11.2023.
//

#pragma once

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct PBRRPSpotLightsPass : public IRenderPass
    {
        // todo: сделать рендер пассы с настройкой времени выполнения. то же самое сделать с системами
        
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        Timer m_renderTimer { true };
        
        Slot<void(double, double)> m_renderListener = [](auto, auto) { };

        Ref<IUniformBuffer> m_uniformBuffer;
    };
}
