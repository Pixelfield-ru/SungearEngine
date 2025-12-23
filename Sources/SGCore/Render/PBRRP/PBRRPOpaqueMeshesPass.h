//
// Created by stuka on 22.12.2025.
//

#pragma once

#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

namespace SGCore
{
    struct PBRRPOpaqueMeshesPass : IGeometryPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
