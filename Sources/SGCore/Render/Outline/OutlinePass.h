//
// Created by stuka on 17.12.2024.
//

#ifndef SUNGEARENGINE_OUTLINEPASS_H
#define SUNGEARENGINE_OUTLINEPASS_H

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct OutlinePass : public IRenderPass
    {
        float m_outlineThickness = 1.2;
        glm::vec4 m_outlineColor { 0.5, 0.5, 0.0, 1.0 };

        void create(const Ref<IRenderPipeline>& parentRenderPipeline) noexcept final;

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) noexcept final;
    };
}

#endif //SUNGEARENGINE_OUTLINEPASS_H
