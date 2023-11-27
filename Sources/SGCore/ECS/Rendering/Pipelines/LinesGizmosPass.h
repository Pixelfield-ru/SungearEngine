//
// Created by stuka on 27.11.2023.
//

#ifndef SUNGEARENGINE_LINESGIZMOSPASS_H
#define SUNGEARENGINE_LINESGIZMOSPASS_H

#include "IRenderPass.h"

namespace SGCore
{
    struct LinesGizmosPass : public IRenderPass
    {
        void render(const Ref<IRenderPipeline>& renderPipeline) final;
    };
}

#endif //SUNGEARENGINE_LINESGIZMOSPASS_H
