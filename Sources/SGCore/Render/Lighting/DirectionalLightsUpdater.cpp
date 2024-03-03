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
                                                   IShaderUniform(currentDirLight + ".projectionMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".viewMatrix", SGGDataType::SGG_MAT4),
                                                   IShaderUniform(currentDirLight + ".spaceMatrix", SGGDataType::SGG_MAT4),
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

    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();

    if(renderPipeline)
    {
        auto geomPass = renderPipeline->getRenderPass<IGeometryPass>();
        if(geomPass)
        {
            geomPass->m_uniformBuffersToUse.push_back(m_uniformBuffer);
        }
    }
    
    Ref<TimerCallback> callback = MakeRef<TimerCallback>();
    callback->setUpdateFunction([this](const double& dt, const double& fixedDt) {
        updateLights();
    });
    
    m_lightsUpdateTimer.setTargetFrameRate(30);
    m_lightsUpdateTimer.addCallback(callback);
    m_lightsUpdateTimer.m_cyclic = true;
}

void SGCore::DirectionalLightsUpdater::update(const double& dt, const double& fixedDt)
{
    m_lightsUpdateTimer.startFrame();
}

void SGCore::DirectionalLightsUpdater::updateLights() noexcept
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    auto directionalLightsView = lockedScene->getECSRegistry().view<DirectionalLight, RenderingBase, Ref<Transform>>();
    
    int currenLightIdx = 0;
    
    m_uniformBuffer->bind();
    
    directionalLightsView.each([&currenLightIdx, this](DirectionalLight& directionalLight, RenderingBase& renderingBase, Ref<Transform>& transform) {
        if(currenLightIdx < m_maxLightsCount)
        {
            std::string currentDirLight = "directionalLights[" + std::to_string(currenLightIdx) + "]";
            
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
            
            // todo make observer
            m_uniformBuffer->subData(currentDirLight + ".position", glm::value_ptr(transform->m_ownTransform.m_position), 3);
            
            ++currenLightIdx;
        }
    });
    
    m_uniformBuffer->subData("directionalLightsCount", { currenLightIdx });
}
