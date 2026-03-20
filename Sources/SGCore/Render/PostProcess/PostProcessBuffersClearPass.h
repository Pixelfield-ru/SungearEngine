//
// Created by stuka on 08.02.2025.
//

#pragma once

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct SGCORE_EXPORT PostProcessBuffersClearPass : public IRenderPass
    {
        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
