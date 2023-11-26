//
// Created by stuka on 26.11.2023.
//

#ifndef SUNGEARENGINE_PBRFRPCOMPLEXGIZMOSPASS_H
#define SUNGEARENGINE_PBRFRPCOMPLEXGIZMOSPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"

namespace SGCore
{
    struct PBRFRPComplexGizmosPass : public IRenderPass
    {
        void render(const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_PBRFRPCOMPLEXGIZMOSPASS_H
