//
// Created by Ilya on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRFRPSKYBOXESPASS_H
#define SUNGEARENGINE_PBRFRPSKYBOXESPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"

namespace SGCore
{
    struct PBRFRPSkyboxesPass : public IRenderPass
    {
        void render(const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_PBRFRPSKYBOXESPASS_H
