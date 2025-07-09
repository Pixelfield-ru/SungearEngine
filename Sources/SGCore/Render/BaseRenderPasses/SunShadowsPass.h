//
// Created by stuka on 08.07.2025.
//

#ifndef SUNGEARENGINE_SUNSHADOWSPASS_H
#define SUNGEARENGINE_SUNSHADOWSPASS_H

#include "SGCore/Render/IRenderPass.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"

namespace SGCore
{
    struct SunShadowsPass : IRenderPass
    {
        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;

        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;

    private:
        AssetRef<IShader> m_batchShader;

        static glm::mat4 getLightSpaceMatrix(const RenderingBase::reg_t& cameraRenderingBase, const glm::vec3& sunDir, float nearPlane, float farPlane) noexcept;
        static std::array<glm::vec4, 8> getFrustumCornersWorldSpace(const glm::mat4& projView) noexcept;
        static std::array<glm::vec4, 8> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) noexcept;
        static std::vector<glm::mat4> getLightSpaceMatrices(const CSMTarget::reg_t& csm, const RenderingBase::reg_t& cameraRenderingBase, const glm::vec3& sunDir) noexcept;
    };
}

#endif // SUNGEARENGINE_SUNSHADOWSPASS_H
