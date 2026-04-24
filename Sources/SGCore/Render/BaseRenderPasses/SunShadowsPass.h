//
// Created by stuka on 08.07.2025.
//

#pragma once

#include "SGCore/Render/IRenderPass.h"

namespace SGCore
{
    struct RenderingBase;
    struct CSMTarget;

    struct SGCORE_EXPORT SunShadowsPass : IRenderPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        AssetRef<IShader> m_batchShader;
        RenderState m_renderState;
        Timer m_renderTimer;
        Slot<void(double, double)> m_renderSlot;

        void renderShadows(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline);

        static glm::mat4 getLightSpaceMatrix(const RenderingBase& cameraRenderingBase, const glm::vec3& sunDir, float nearPlane, float farPlane) noexcept;
        static std::array<glm::vec4, 8> getFrustumCornersWorldSpace(const glm::mat4& projView) noexcept;
        static std::array<glm::vec4, 8> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) noexcept;
        static std::vector<glm::mat4> getLightSpaceMatrices(const CSMTarget& csm, const RenderingBase& cameraRenderingBase, const glm::vec3& sunDir) noexcept;
    };
}
