//
// Created by stuka on 17.12.2024.
//

#pragma once

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    class IMeshData;

    struct SGCORE_EXPORT OutlinePass : public IRenderPass
    {
        float m_outlineThickness = 3.0;
        glm::vec4 m_outlineColor { 0.5, 0.5, 0.0, 1.0 };

        Ref<IMeshData> m_postProcessQuad;

        /// Pass 3 draws into colour attachment 7 alone, so it needs a variant of the outline program
        /// that declares only one output; see the SG_OUTLINE_COMBINE branch in outline.glsl.
        AssetRef<IShader> m_combineShader;

        void create(const Ref<IRenderPipeline>& parentRenderPipeline) noexcept final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) noexcept final;
    };
}
