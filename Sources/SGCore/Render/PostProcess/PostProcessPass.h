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

    struct PostProcessPass : public IRenderPass
    {
        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        Ref<IMeshData> m_postProcessQuad;

        PostProcessPass();

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        void depthPass(LayeredFrameReceiver& camera) const noexcept;
        void FXPass(LayeredFrameReceiver& camera) const noexcept;
        void layersCombiningPass(LayeredFrameReceiver& camera) const noexcept;
        void finalFrameFXPass(LayeredFrameReceiver& camera) const;
        
        void bindLayersIndices(LayeredFrameReceiver& camera, const Ref<ISubPassShader>& subPassShader) const noexcept;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXPASS_H
