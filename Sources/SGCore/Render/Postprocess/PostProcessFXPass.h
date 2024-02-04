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
    class Camera;
    struct IRenderPipeline;
    class Scene;

    struct PostProcessFXPass : public IRenderPass
    {
        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        Ref<IMeshData> m_postProcessQuad;

        PostProcessFXPass();

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        void depthPass(Camera& camera) const noexcept;
        void FXPass(Camera& camera) const noexcept;
        void layersCombiningPass(Camera& camera) const noexcept;
        void finalFrameFXPass(Camera& camera) const;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXPASS_H
