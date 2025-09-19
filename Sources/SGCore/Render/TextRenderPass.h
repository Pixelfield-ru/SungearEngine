//
// Created by ilya on 25.02.24.
//

#ifndef SUNGEARENGINE_TEXTRENDERPASS_H
#define SUNGEARENGINE_TEXTRENDERPASS_H

#include "IRenderPass.h"

namespace SGCore
{
    struct TextRenderPass : public IRenderPass
    {
        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_TEXTRENDERPASS_H
