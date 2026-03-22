//
// Created by stuka on 26.11.2024.
//

#pragma once

#include "PostProcessFXSubPass.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct SGCORE_EXPORT PostProcessPass : public IRenderPass
    {
        Ref<IMeshData> m_postProcessQuad;

        PostProcessPass();

        void render(const Scene*, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        void layersFX(LayeredFrameReceiver& receiver) noexcept;
        size_t bindCommonUniforms(LayeredFrameReceiver& receiver, const AssetRef<IShader>& subPassShader) const noexcept;
    };
}
