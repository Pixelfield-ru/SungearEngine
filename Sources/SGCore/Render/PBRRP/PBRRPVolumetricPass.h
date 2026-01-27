//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

namespace SGCore
{
    struct PBRRPVolumetricPass : public IGeometryPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
