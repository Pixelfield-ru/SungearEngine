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

    struct PostProcessFXPass : public IRenderPass
    {
        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        Ref<IMeshData> m_postProcessQuad;

        PostProcessFXPass();

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_POSTPROCESSFXPASS_H
