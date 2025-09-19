//
// Created by stuka on 26.11.2024.
//

#ifndef SUNGEARENGINE_POSTPROCESSPASS_H
#define SUNGEARENGINE_POSTPROCESSPASS_H

#include "PostProcessFXSubPass.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Render/IRenderPass.h"

class IMeshData;
class LayeredFrameReceiver;
struct IRenderPipeline;
class Scene;
struct RenderingBase;
struct Transform;

namespace SGCore
{
    struct PostProcessPass : public IRenderPass
    {
        Ref<IMeshData> m_postProcessQuad;

        PostProcessPass();

        void render(const Scene*, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        void layersFX(LayeredFrameReceiver& receiver) noexcept;
        void bindCommonUniforms(LayeredFrameReceiver& receiver, const AssetRef<IShader>& subPassShader) const noexcept;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSPASS_H
