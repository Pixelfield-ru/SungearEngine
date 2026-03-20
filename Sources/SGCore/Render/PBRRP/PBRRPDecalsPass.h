//
// Created by stuka on 01.05.2025.
//

#pragma once

#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

namespace SGCore
{
    struct SGCORE_EXPORT PBRRPDecalsPass : public IGeometryPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        AssetRef<IShader> m_instancingShader;
    };
}
