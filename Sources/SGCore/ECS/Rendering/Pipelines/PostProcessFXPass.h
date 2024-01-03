//
// Created by stuka on 26.11.2023.
//

#ifndef SUNGEARENGINE_POSTPROCESSFXPASS_H
#define SUNGEARENGINE_POSTPROCESSFXPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"
#include "PostProcessFXSubPass.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

namespace SGCore
{
    class IMeshData;
    class ICamera;

    struct PostProcessFXPass : public IRenderPass
    {
        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        Ref<IMeshData> m_postProcessQuad;

        PostProcessFXPass();

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        void depthPass(const Ref<ICamera>& camera) const noexcept;
        void FXPass(const Ref<ICamera>&) const noexcept;
        void layersCombiningPass(const Ref<ICamera>& camera) const noexcept;
        void finalFrameFXPass(const Ref<ICamera>& camera) const;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXPASS_H
