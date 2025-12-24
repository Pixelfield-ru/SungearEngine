//
// Created by stuka on 25.12.2025.
//

#pragma once

#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

namespace SGCore
{
    struct PBRRPTerrainsPass : IGeometryPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
