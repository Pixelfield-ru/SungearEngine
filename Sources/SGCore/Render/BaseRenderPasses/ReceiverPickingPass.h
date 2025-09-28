//
// Created by stuka on 29.09.2025.
//

#pragma once

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct ReceiverPickingPass : IRenderPass
    {
        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
