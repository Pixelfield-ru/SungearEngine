//
// Created by stuka on 26.11.2023.
//

#ifndef SUNGEARENGINE_POSTPROCESSFXPASS_H
#define SUNGEARENGINE_POSTPROCESSFXPASS_H

#include "PostProcessFXSubPass.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    class IMeshData;
    class LayeredFrameReceiver;
    struct IRenderPipeline;
    class Scene;
    struct RenderingBase;
    struct Transform;

    struct PostProcessPass : public IRenderPass
    {
        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        Ref<IMeshData> m_postProcessQuad;

        PostProcessPass();

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        void depthPass(LayeredFrameReceiver& receiver, const Ref<RenderingBase>& renderingBase, const Ref<Transform>& transform) const noexcept;
        void FXPass(LayeredFrameReceiver& receiver, const Ref<RenderingBase>& renderingBase, const Ref<Transform>& transform) const noexcept;
        void layersCombiningPass(LayeredFrameReceiver& receiver, const Ref<RenderingBase>& renderingBase, const Ref<Transform>& transform) const noexcept;
        void finalFrameFXPass(LayeredFrameReceiver& receiver, const Ref<RenderingBase>& renderingBase, const Ref<Transform>& transform) const;
        
        void bindLayersIndices(LayeredFrameReceiver& receiver, const Ref<ISubPassShader>& subPassShader) const noexcept;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXPASS_H
