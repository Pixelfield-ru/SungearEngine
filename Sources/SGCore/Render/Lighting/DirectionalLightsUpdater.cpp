//
// Created by ilya on 14.02.24.
//

#include <glm/gtc/type_ptr.hpp>

#include "DirectionalLightsUpdater.h"
#include "DirectionalLight.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"

SGCore::DirectionalLightsUpdater::DirectionalLightsUpdater() noexcept
{
    // todo: move uniform in other system
    m_uniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    m_uniformBuffer->m_blockName = "DirectionalLightsBlock";
    for(int i = 0; i < m_maxLightsCount; ++i)
    {
        std::string currentDirLight = "directionalLights[" + std::to_string(i) + "]";
        
        m_uniformBuffer->putUniforms({
                                                   IShaderUniform(currentDirLight + ".projectionSpaceMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".orthographicSpaceMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".orthographicMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".projectionMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".viewMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".position", SGGDataType::SGG_FLOAT3),
                                                   IShaderUniform(currentDirLight + ".p0", SGGDataType::SGG_FLOAT),
                                                   IShaderUniform(currentDirLight + ".rotation", SGGDataType::SGG_FLOAT3),
                                                   IShaderUniform(currentDirLight + ".p1", SGGDataType::SGG_FLOAT),
                                                   IShaderUniform(currentDirLight + ".scale", SGGDataType::SGG_FLOAT3),
                                                   IShaderUniform(currentDirLight + ".p2", SGGDataType::SGG_FLOAT),
                                                   IShaderUniform(currentDirLight + ".color", SGGDataType::SGG_FLOAT4),
                                                   IShaderUniform(currentDirLight + ".intensity", SGGDataType::SGG_FLOAT),
                                                   IShaderUniform(currentDirLight + ".shadowSamplesCount", SGGDataType::SGG_INT),
                                                   IShaderUniform(currentDirLight + ".p3", SGGDataType::SGG_FLOAT),
                                                   IShaderUniform(currentDirLight + ".p4", SGGDataType::SGG_FLOAT)
                                           });
    }
    
    m_uniformBuffer->putUniforms({
                                               IShaderUniform("directionalLightsCount", SGGDataType::SGG_INT)
                                       });
    
    m_uniformBuffer->setLayoutLocation(3);
    m_uniformBuffer->prepare();

    auto renderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();

    if(renderPipeline)
    {
        auto geomPass = renderPipeline->getRenderPass<IGeometryPass>();
        if(geomPass)
        {
            geomPass->m_uniformBuffersToUse.push_back(m_uniformBuffer);
        }
    }
    
    m_lightsUpdateTimer.setTargetFrameRate(30);
    m_lightsUpdateTimer.onUpdate += m_lightsUpdateEventListener;
    m_lightsUpdateTimer.m_cyclic = true;
}

void SGCore::DirectionalLightsUpdater::update(const double& dt, const double& fixedDt)
{
    m_lightsUpdateTimer.startFrame();
}

void SGCore::DirectionalLightsUpdater::updateLights() noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    auto directionalLightsView = lockedScene->getECSRegistry()->view<DirectionalLight, RenderingBase, Transform>();
    
    int currentLightIdx = 0;
    
    m_uniformBuffer->bind();
    
    directionalLightsView.each([&currentLightIdx, this](DirectionalLight::reg_t& directionalLight, RenderingBase::reg_t& renderingBase, Transform::reg_t& transform) {
        if(currentLightIdx < m_maxLightsCount)
        {
            std::string currentDirLight = "directionalLights[" + std::to_string(currentLightIdx) + "]";
            
            if(directionalLight.m_base.m_color != directionalLight.m_base.m_lastColor)
            {
                std::cout << "currentDirLight : " << currentDirLight << ", col : " << directionalLight.m_base.m_color.r << ", " << directionalLight.m_base.m_color.g << ", " << directionalLight.m_base.m_color.b << std::endl;
                
                m_uniformBuffer->subData(currentDirLight + ".color", glm::value_ptr(directionalLight.m_base.m_color), 4);
                
                // char* col = m_lightsUniformBuffer->getUniform(currentDirLight + ".color");
                
                // std::cout << "col : " << *(float*) col << ", " << *(float*) (col + 4) << ", " << *(float*) (col + 8) << ", " << *(float*) (col + 12) << std::endl;
                
                directionalLight.m_base.m_lastColor = directionalLight.m_base.m_color;
            }
            
            if(directionalLight.m_base.m_intensity != directionalLight.m_base.m_lastIntensity)
            {
                m_uniformBuffer->subData(currentDirLight + ".intensity", { directionalLight.m_base.m_intensity } );
                
                directionalLight.m_base.m_lastIntensity = directionalLight.m_base.m_intensity;
            }

            m_uniformBuffer->subData(currentDirLight + ".position", glm::value_ptr(transform->m_finalTransform.m_position), 3);
            
            ++currentLightIdx;
        }
    });

    m_uniformBuffer->subData("directionalLightsCount", { currentLightIdx });
}
