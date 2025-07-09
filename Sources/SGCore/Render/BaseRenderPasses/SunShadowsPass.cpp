//
// Created by stuka on 08.07.2025.
//

#include "SunShadowsPass.h"

#include "SGCore/Render/Batching/Batch.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Render/ShadowMapping/ShadowCaster.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Render/IRenderPipeline.h"

void SGCore::SunShadowsPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
        *parentRenderPipeline->m_shadersPaths["ShadowsGen/BatchingShader"]);

    m_batchShader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());
}

void SGCore::SunShadowsPass::render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline)
{
    const auto registry = scene->getECSRegistry();

    const auto batchesView = registry->view<Batch, ShadowCaster>();
    const auto atmospheresView = registry->view<Atmosphere>();
    const auto camerasView = registry->view<CSMTarget, RenderingBase>();

    atmospheresView.each([&camerasView, &batchesView, &registry, this](const Atmosphere::reg_t& atmosphere) {
        camerasView.each([&atmosphere, &batchesView, &registry, this](const CSMTarget::reg_t& csm, const RenderingBase::reg_t& renderingBase) {
            const std::vector<glm::mat4> lightSpaceMatrices = getLightSpaceMatrices(csm, renderingBase, atmosphere.m_sunPosition);

            const auto& cascades = csm.getCascades();

            batchesView.each([&registry, &cascades, &lightSpaceMatrices, this](Batch::reg_t& batch, const auto&) {
                batch.update(*registry);
                batch.bind(m_batchShader.get());

                // rendering to levels
                for(size_t i = 0; i < cascades.size(); ++i)
                {
                    const auto& cascade = cascades[i];

                    if(!cascade.m_frameBuffer) continue;

                    const auto& cascadeMatrix = lightSpaceMatrices[i];

                    cascade.m_frameBuffer->bind();

                    m_batchShader->useMatrix("CSMLightSpaceMatrix", cascadeMatrix);

                    CoreMain::getRenderer()->renderArray(
                        batch.getVertexArray(),
                        batch.m_batchRenderState,
                        batch.getTrianglesCount(),
                        0
                    );

                    cascade.m_frameBuffer->unbind();
                }
            });
        });
    });
}

glm::mat4 SGCore::SunShadowsPass::getLightSpaceMatrix(const RenderingBase::reg_t& cameraRenderingBase, const glm::vec3& sunDir, float nearPlane, float farPlane) noexcept
{
    const auto proj = glm::perspective(
                glm::radians(cameraRenderingBase->m_fov), cameraRenderingBase->m_aspect, nearPlane,
                farPlane);
    const auto corners = getFrustumCornersWorldSpace(proj, cameraRenderingBase->m_viewMatrix);

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : corners)
    {
        center += glm::vec3(v);
    }
    center /= corners.size();

    const auto lightView = glm::lookAt(center + sunDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : corners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 10.0f;
    if(minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if(maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
    return lightProjection * lightView;
}

std::array<glm::vec4, 8> SGCore::SunShadowsPass::getFrustumCornersWorldSpace(const glm::mat4& projView) noexcept
{
    const auto inv = glm::inverse(projView);

    std::array<glm::vec4, 8> frustumCorners { };

    std::uint8_t i = 0;

    for(std::uint8_t x = 0; x < 2; ++x)
    {
        for(std::uint8_t y = 0; y < 2; ++y)
        {
            for(std::uint8_t z = 0; z < 2; ++z)
            {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                frustumCorners[i] = pt / pt.w;
            }
        }
    }

    return frustumCorners;
}

std::array<glm::vec4, 8> SGCore::SunShadowsPass::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) noexcept
{
    return getFrustumCornersWorldSpace(proj * view);
}

std::vector<glm::mat4> SGCore::SunShadowsPass::getLightSpaceMatrices(const CSMTarget::reg_t& csm, const RenderingBase::reg_t& cameraRenderingBase, const glm::vec3& sunDir) noexcept
{
    const auto& levels = csm.getCascades();

    std::vector<glm::mat4> ret;
    for(size_t i = 0; i < levels.size() + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(getLightSpaceMatrix(cameraRenderingBase, sunDir, cameraRenderingBase->m_zNear, levels[i].m_level));
        }
        else if (i < levels.size())
        {
            ret.push_back(getLightSpaceMatrix(cameraRenderingBase, sunDir, levels[i - 1].m_level, levels[i].m_level));
        }
        else
        {
            ret.push_back(getLightSpaceMatrix(cameraRenderingBase, sunDir, levels[i - 1].m_level, cameraRenderingBase->m_zFar));
        }
    }
    return ret;
}
