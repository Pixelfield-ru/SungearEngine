//
// Created by ilya on 25.02.24.
//

#pragma once

#include "IRenderPass.h"

namespace SGCore
{
    struct SGCORE_EXPORT TextRenderPass : public IRenderPass
    {
        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}
