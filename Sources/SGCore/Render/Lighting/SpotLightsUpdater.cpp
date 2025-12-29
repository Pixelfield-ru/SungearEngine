//
// Created by ilya on 14.02.24.
//

#include <glm/gtc/type_ptr.hpp>

#include "SpotLightsUpdater.h"
#include "SpotLight.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

SGCore::SpotLightsUpdater::SpotLightsUpdater() noexcept
{
    // todo: move uniform in other system
    m_uniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    m_uniformBuffer->m_blockName = "SpotLightsBlock";
    for(int i = 0; i < m_maxLightsCount; ++i)
    {
        std::string currentDirLight = "sg_spotLights[" + std::to_string(i) + "]";

        m_uniformBuffer->putUniforms({
            IShaderUniform(currentDirLight + ".projectionSpaceMatrix", SGGDataType::SGG_MAT4),
            IShaderUniform(currentDirLight + ".modelMatrix", SGGDataType::SGG_MAT4),
            IShaderUniform(currentDirLight + ".position", SGGDataType::SGG_FLOAT3),
            IShaderUniform(currentDirLight + ".p0", SGGDataType::SGG_FLOAT),
            IShaderUniform(currentDirLight + ".direction", SGGDataType::SGG_FLOAT3),
            IShaderUniform(currentDirLight + ".intensity", SGGDataType::SGG_FLOAT),
            IShaderUniform(currentDirLight + ".color", SGGDataType::SGG_FLOAT4),
            IShaderUniform(currentDirLight + ".shadowSamplesCount", SGGDataType::SGG_INT),
            IShaderUniform(currentDirLight + ".cutoffSamplerIndex", SGGDataType::SGG_INT),
            IShaderUniform(currentDirLight + ".innerCutoff", SGGDataType::SGG_FLOAT),
            IShaderUniform(currentDirLight + ".outerCutoff", SGGDataType::SGG_FLOAT)
        });
    }

    m_uniformBuffer->putUniforms({
        IShaderUniform("sg_spotLightsCount", SGGDataType::SGG_INT)
    });
    
    m_uniformBuffer->setLayoutLocation(3);
    m_uniformBuffer->prepare();

    auto renderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();

    if(renderPipeline)
    {
        auto geomPasses = renderPipeline->getRenderPasses<IGeometryPass>();
        for(const auto& geomPass : geomPasses)
        {
            geomPass->m_uniformBuffersToUse.push_back(m_uniformBuffer);
        }
    }
    
    m_lightsUpdateTimer.setTargetFrameRate(60);
    m_lightsUpdateTimer.onUpdate += m_lightsUpdateEventListener;
    m_lightsUpdateTimer.m_cyclic = true;
}

void SGCore::SpotLightsUpdater::update(const double& dt, const double& fixedDt)
{
    // m_lightsUpdateTimer.startFrame();
    updateLights();
}

void SGCore::SpotLightsUpdater::updateLights() noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto spotLightsView = lockedScene->getECSRegistry()->view<SpotLight, RenderingBase, Transform>();
    
    int currentLightIdx = 0;
    
    m_uniformBuffer->bind();
    
    spotLightsView.each([&currentLightIdx, this](SpotLight::reg_t& spotLight, RenderingBase::reg_t& renderingBase, Transform::reg_t& transform) {
        if(currentLightIdx < m_maxLightsCount)
        {
            const std::string currentSpotLight = "sg_spotLights[" + std::to_string(currentLightIdx) + "]";
            
            if(spotLight.m_base.m_color != spotLight.m_base.m_lastColor)
            {
                std::cout << "currentDirLight : " << currentSpotLight << ", col : " << spotLight.m_base.m_color.r << ", " << spotLight.m_base.m_color.g << ", " << spotLight.m_base.m_color.b << std::endl;
                
                m_uniformBuffer->subData(currentSpotLight + ".color", glm::value_ptr(spotLight.m_base.m_color), 4);
                
                // char* col = m_lightsUniformBuffer->getUniform(currentDirLight + ".color");
                
                // std::cout << "col : " << *(float*) col << ", " << *(float*) (col + 4) << ", " << *(float*) (col + 8) << ", " << *(float*) (col + 12) << std::endl;
                
                spotLight.m_base.m_lastColor = spotLight.m_base.m_color;
            }
            
            if(spotLight.m_base.m_intensity != spotLight.m_base.m_lastIntensity)
            {
                m_uniformBuffer->subData(currentSpotLight + ".intensity", { spotLight.m_base.m_intensity } );
                
                spotLight.m_base.m_lastIntensity = spotLight.m_base.m_intensity;
            }

            const glm::vec3 lightDirection = transform->m_finalTransform.m_rotation * spotLight.m_basicDirection;

            std::cout << "lightDirection: " << lightDirection.x << ", " << lightDirection.y << ", " << lightDirection.z << std::endl;

            m_uniformBuffer->subData(currentSpotLight + ".projectionSpaceMatrix", glm::value_ptr(renderingBase->m_projectionSpaceMatrix), 16);
            m_uniformBuffer->subData(currentSpotLight + ".modelMatrix", glm::value_ptr(transform->m_finalTransform.m_animatedModelMatrix), 16);
            m_uniformBuffer->subData(currentSpotLight + ".position", glm::value_ptr(transform->m_finalTransform.m_position), 3);
            m_uniformBuffer->subData(currentSpotLight + ".direction", glm::value_ptr(lightDirection), 3);
            m_uniformBuffer->subData(currentSpotLight + ".innerCutoff", { spotLight.getInnerCutoff() });
            m_uniformBuffer->subData(currentSpotLight + ".outerCutoff", { spotLight.getOuterCutoff() });

            ++currentLightIdx;
        }
    });

    m_uniformBuffer->subData("sg_spotLightsCount", { currentLightIdx });
}
