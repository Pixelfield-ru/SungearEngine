//
// Created by stuka on 24.12.2025.
//

#pragma once

#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

namespace SGCore
{
    struct PBRRPBatchingPass : IGeometryPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
