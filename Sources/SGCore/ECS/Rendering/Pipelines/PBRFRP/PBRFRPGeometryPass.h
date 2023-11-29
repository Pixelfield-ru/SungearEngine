//
// Created by stuka on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRFRPGEOMETRYPASS_H
#define SUNGEARENGINE_PBRFRPGEOMETRYPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"

namespace SGCore
{
    struct PBRFRPGeometryPass : public IRenderPass
    {
        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_PBRFRPGEOMETRYPASS_H
