//
// Created by ilya on 14.02.24.
//

// for glm/gtx/rotate_vector.hpp
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "AtmosphereUpdater.h"

#include "SGCore/Scene/Scene.h"
#include "Atmosphere.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "AtmosphereUtils.h"
#include "SGCore/Render/BaseRenderPasses/IDecalsPass.h"
#include "SGCore/Render/PBRRP/PBRRPDecalsPass.h"

SGCore::AtmosphereUpdater::AtmosphereUpdater() noexcept
{
    m_uniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    
    m_uniformBuffer->m_blockName = "AtmosphereBlock";
    m_uniformBuffer->putUniforms({
        IShaderUniform("atmosphere.sunAmbient", SGGDataType::SGG_FLOAT3),
        IShaderUniform("atmosphere.p0", SGGDataType::SGG_FLOAT),
        
        IShaderUniform("atmosphere.sunPosition", SGGDataType::SGG_FLOAT3),
        IShaderUniform("atmosphere.p1", SGGDataType::SGG_FLOAT),

        IShaderUniform("atmosphere.sunColor", SGGDataType::SGG_FLOAT3),
        IShaderUniform("atmosphere.rayleighScaleHeight", SGGDataType::SGG_FLOAT),

        IShaderUniform("atmosphere.rayleighScatteringCoeff", SGGDataType::SGG_FLOAT3),
        IShaderUniform("atmosphere.mieScatteringCoeff", SGGDataType::SGG_FLOAT),

        IShaderUniform("atmosphere.mieScaleHeight", SGGDataType::SGG_FLOAT),
        IShaderUniform("atmosphere.sunIntensity", SGGDataType::SGG_FLOAT),

        IShaderUniform("atmosphere.planetRadius", SGGDataType::SGG_FLOAT),
        IShaderUniform("atmosphere.atmosphereRadius", SGGDataType::SGG_FLOAT),

        IShaderUniform("atmosphere.rayOrigin", SGGDataType::SGG_FLOAT3),
        IShaderUniform("atmosphere.miePreferredScatteringDirection", SGGDataType::SGG_FLOAT)
    });

    m_uniformBuffer->setLayoutLocation(4);
    m_uniformBuffer->prepare();

    auto renderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();
    if(renderPipeline)
    {
        auto geomPass = renderPipeline->getRenderPass<IGeometryPass>();
        if(geomPass)
        {
            geomPass->m_uniformBuffersToUse.push_back(m_uniformBuffer);
        }

        auto decalsPass = renderPipeline->getRenderPass<IDecalsPass>();
        if(decalsPass)
        {
            decalsPass->m_uniformBuffersToUse.push_back(m_uniformBuffer);
        }
    }
    
    m_atmosphereUpdateTimer.setTargetFrameRate(60);
    m_atmosphereUpdateTimer.onUpdate += m_atmosphereUpdateEventListener;
    m_atmosphereUpdateTimer.m_cyclic = true;
}

void SGCore::AtmosphereUpdater::update(const double& dt, const double& fixedDt)
{
    m_atmosphereUpdateTimer.startFrame();
}

void SGCore::AtmosphereUpdater::updateAtmosphere() noexcept
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    auto atmosphereScatteringsView = lockedScene->getECSRegistry()->view<Atmosphere, Mesh>();
    
    atmosphereScatteringsView.each([&lockedScene, this](const ECS::entity_t& entity, Atmosphere::reg_t& atmosphere, Mesh::reg_t& mesh) {
        if(!mesh.m_base.getMaterial()) return;

        size_t hashedSunPos = MathUtils::hashVector(atmosphere.m_sunPosition);

        auto meshShader = mesh.m_base.getMaterial()->m_shaders["GeometryPass"];
        if(meshShader)
        {
            meshShader->bind();
            meshShader->useUniformBuffer(m_uniformBuffer);
        }
        
        if(atmosphere.m_sunRotation != atmosphere.m_lastSunRotation)
        {
            atmosphere.m_sunPosition = glm::rotateZ(Atmosphere::getSunOrigin(), glm::radians(atmosphere.m_sunRotation.z));
            atmosphere.m_sunPosition = glm::rotateY(atmosphere.m_sunPosition, glm::radians(atmosphere.m_sunRotation.y));
            atmosphere.m_sunPosition = glm::rotateX(atmosphere.m_sunPosition, glm::radians(atmosphere.m_sunRotation.x));
            
            if(atmosphere.m_precalculatedSunColors.find(hashedSunPos) == atmosphere.m_precalculatedSunColors.end())
            {
                atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            }
            
            auto& sunColor = atmosphere.m_precalculatedSunColors[hashedSunPos];
            
            atmosphere.m_sunAmbient = 0.025f * glm::vec3(sunColor.r, sunColor.r, sunColor.r);
            
            m_uniformBuffer->subData("atmosphere.sunPosition", glm::value_ptr(atmosphere.m_sunPosition), 3);
            m_uniformBuffer->subData("atmosphere.sunColor", glm::value_ptr(sunColor), 3);
            m_uniformBuffer->subData("atmosphere.sunAmbient", glm::value_ptr(atmosphere.m_sunAmbient), 3);
            
            atmosphere.m_lastSunRotation = atmosphere.m_sunRotation;
        }
        
        if(atmosphere.m_rayleighScatteringCoeff != atmosphere.m_lastRayleighScatteringCoeff)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.rayleighScatteringCoeff",
                                     glm::value_ptr(atmosphere.m_rayleighScatteringCoeff), 3);
            
            atmosphere.m_lastRayleighScatteringCoeff = atmosphere.m_rayleighScatteringCoeff;
        }
        
        if(atmosphere.m_mieScatteringCoeff != atmosphere.m_lastMieScatteringCoeff)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.mieScatteringCoeff", { atmosphere.m_mieScatteringCoeff });
            
            atmosphere.m_lastMieScatteringCoeff = atmosphere.m_mieScatteringCoeff;
        }
        
        if(atmosphere.m_rayleighScaleHeight != atmosphere.m_lastRayleighScaleHeight)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.rayleighScaleHeight", { atmosphere.m_rayleighScaleHeight });
            
            atmosphere.m_lastRayleighScaleHeight = atmosphere.m_rayleighScaleHeight;
        }
        
        if(atmosphere.m_mieScaleHeight != atmosphere.m_lastMieScaleHeight)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.mieScaleHeight", { atmosphere.m_mieScaleHeight });
            
            atmosphere.m_lastMieScaleHeight = atmosphere.m_mieScaleHeight;
        }
        
        if(atmosphere.m_sunIntensity != atmosphere.m_lastSunIntensity)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.sunIntensity", { atmosphere.m_sunIntensity });
            
            atmosphere.m_lastSunIntensity = atmosphere.m_sunIntensity;
        }
        
        if(atmosphere.m_planetRadius != atmosphere.m_lastPlanetRadius)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.planetRadius", { atmosphere.m_planetRadius });
            
            atmosphere.m_lastPlanetRadius = atmosphere.m_planetRadius;
        }
        
        if(atmosphere.m_atmosphereRadius != atmosphere.m_lastAtmosphereRadius)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.atmosphereRadius", { atmosphere.m_atmosphereRadius });
            
            atmosphere.m_lastAtmosphereRadius = atmosphere.m_atmosphereRadius;
        }
        
        if(atmosphere.m_miePreferredScatteringDirection != atmosphere.m_lastMiePreferredScatteringDirection)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.miePreferredScatteringDirection", { atmosphere.m_miePreferredScatteringDirection });
            
            atmosphere.m_lastMiePreferredScatteringDirection = atmosphere.m_miePreferredScatteringDirection;
        }
        
        if(atmosphere.m_rayOrigin != atmosphere.m_lastRayOrigin)
        {
            atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            
            m_uniformBuffer->subData("atmosphere.rayOrigin", glm::value_ptr(atmosphere.m_rayOrigin), 3);
            
            atmosphere.m_lastRayOrigin = atmosphere.m_rayOrigin;
        }
    });
}
