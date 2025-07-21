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

    m_renderState.m_depthFunc = SGDepthStencilFunc::SGG_LESS;
    m_renderState.m_globalBlendingState.m_useBlending = false;

    m_batchShader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    m_renderTimer.m_cyclic = true;
    m_renderTimer.setTargetFrameRate(30.0);

    if(!m_renderTimer.onUpdate.contains(m_renderSlot))
    {
        m_renderSlot = [this](const double& dt, const double& fixedDt) {
            renderShadows(Scene::getCurrentScene());
        };

        m_renderTimer.onUpdate += m_renderSlot;
    }
}

void SGCore::SunShadowsPass::render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline)
{
    // m_renderTimer.startFrame();
    renderShadows(scene);
}

void SGCore::SunShadowsPass::renderShadows(const Ref<Scene>& scene)
{
    const auto registry = scene->getECSRegistry();

    const auto batchesView = registry->view<Batch, ShadowCaster>();
    const auto atmospheresView = registry->view<Atmosphere>();
    const auto camerasView = registry->view<CSMTarget, RenderingBase>();

    m_renderState.use(true);

    // glGetError();

    atmospheresView.each([&camerasView, &batchesView, &registry, this](const Atmosphere::reg_t& atmosphere) {
        camerasView.each([&atmosphere, &batchesView, &registry, this](CSMTarget::reg_t& csm, const RenderingBase::reg_t& renderingBase) {
            const std::vector<glm::mat4> lightSpaceMatrices = getLightSpaceMatrices(csm, renderingBase, atmosphere.m_sunPosition);

            // rendering to levels
            for(size_t i = 0; i < csm.getCascades().size(); ++i)
            {
                auto& cascade = csm.getCascade(i);

                if(!cascade.m_frameBuffer) continue;

                const auto& cascadeMatrix = lightSpaceMatrices[i];

                cascade.m_projectionSpaceMatrix = cascadeMatrix;

                cascade.m_frameBuffer->bind();
                cascade.m_frameBuffer->clear();

                batchesView.each([&registry, &csm, &lightSpaceMatrices, &cascadeMatrix, this](Batch::reg_t& batch, const auto&) {
                    batch.bind(m_batchShader.get());

                    m_batchShader->useMatrix("CSMLightSpaceMatrix", cascadeMatrix);

                    CoreMain::getRenderer()->renderArray(
                        batch.getVertexArray(),
                        batch.m_batchRenderState,
                        batch.getTrianglesCount(),
                        0
                    );
                });

                cascade.m_frameBuffer->unbind();
            }
        });
    });

    // CoreMain::getRenderer()->checkForErrors();
}

glm::mat4 SGCore::SunShadowsPass::getLightSpaceMatrix(const RenderingBase::reg_t& cameraRenderingBase, const glm::vec3& sunDir, float nearPlane, float farPlane) noexcept
{
    const auto proj = glm::perspective(
                glm::radians(cameraRenderingBase->m_fov), cameraRenderingBase->m_aspect, nearPlane,
                farPlane);
    const auto corners = getFrustumCornersWorldSpace(proj, cameraRenderingBase->m_viewMatrix);

    // std::cout << proj << std::endl;

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : corners)
    {
        center += glm::vec3(v);
    }

    center /= corners.size();

    // std::cout << center << std::endl;
    // std::cout << sunDir << std::endl;

    const auto lightView = glm::lookAt(center + sunDir, center, glm::vec3(0.0f, 1.0f, 0.0f));
    // const auto lightView = glm::lookAt(glm::normalize(center) + sunDir, glm::normalize(center), glm::vec3(0.0f, 1.0f, 0.0f));

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
    // std::cout << lightProjection << std::endl;
    // std::cout << lightView << std::endl;
    const auto mat = lightProjection * lightView;

    // std::cout << mat << std::endl;

    return mat;
}

std::array<glm::vec4, 8> SGCore::SunShadowsPass::getFrustumCornersWorldSpace(const glm::mat4& projView) noexcept
{
    const auto inv = glm::inverse(projView);

    std::array<glm::vec4, 8> frustumCorners {
        glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
        glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),
        glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
        // Far plane
        glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
        glm::vec4(-1.0f, -1.0f, 1.0, 1.0f)
    };

    for(std::uint8_t i = 0; i < 8; ++i)
    {
        const glm::vec4 pt = inv * frustumCorners[i];
        frustumCorners[i] = pt / pt.w;
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
    for(size_t i = 0; i < levels.size(); ++i)
    {
        if (i == 0)
        {
            ret.push_back(getLightSpaceMatrix(cameraRenderingBase, sunDir, cameraRenderingBase->m_zNear, cameraRenderingBase->m_zFar / levels[i].m_level));
        }
        else if (i < levels.size())
        {
            ret.push_back(getLightSpaceMatrix(cameraRenderingBase, sunDir, cameraRenderingBase->m_zFar / levels[i - 1].m_level, cameraRenderingBase->m_zFar / levels[i].m_level));
        }
    }
    return ret;
}
