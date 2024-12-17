//
// Created by stuka on 17.12.2024.
//

#ifndef SUNGEARENGINE_PICKINGPASS_H
#define SUNGEARENGINE_PICKINGPASS_H

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct PickingPass : public IRenderPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) noexcept final;

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) noexcept final;
    };
}

#endif //SUNGEARENGINE_PICKINGPASS_H
