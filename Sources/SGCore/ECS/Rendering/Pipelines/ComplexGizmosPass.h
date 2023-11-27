//
// Created by stuka on 26.11.2023.
//

#ifndef SUNGEARENGINE_COMPLEXGIZMOSPASS_H
#define SUNGEARENGINE_COMPLEXGIZMOSPASS_H

#include "SGCore/ECS/Rendering/Pipelines/IRenderPass.h"

namespace SGCore
{
    struct ComplexGizmosPass : public IRenderPass
    {
        void render(const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_COMPLEXGIZMOSPASS_H
