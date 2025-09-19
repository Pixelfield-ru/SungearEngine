//
// Created by stuka on 08.02.2025.
//

#ifndef POSTPROCESSBUFFERSCLEARPASS_H
#define POSTPROCESSBUFFERSCLEARPASS_H

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct PostProcessBuffersClearPass : public IRenderPass
    {
        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //POSTPROCESSBUFFERSCLEARPASS_H
