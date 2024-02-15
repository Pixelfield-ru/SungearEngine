//
// Created by ilya on 14.02.24.
//

#include <glm/gtx/rotate_vector.hpp>

#include "AtmosphereUpdater.h"

#include "SGCore/Scene/Scene.h"
#include "Atmosphere.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "glm/gtc/type_ptr.hpp"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"
#include "AtmosphereUtils.h"

SGCore::AtmosphereUpdater::AtmosphereUpdater() noexcept
{
    m_uniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    
    m_uniformBuffer->m_blockName = "AtmosphereBlock";
    m_uniformBuffer->putUniforms({
        IShaderUniform("atmosphere.sunPosition", SGGDataType::SGG_FLOAT3),
        IShaderUniform("atmosphere.p0", SGGDataType::SGG_FLOAT),

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
    
    auto renderPipeline = RenderPipelinesManager::getRenderPipeline();
    if(renderPipeline)
    {
        auto geomPass = renderPipeline->getRenderPass<IGeometryPass>();
        if(geomPass)
        {
            geomPass->m_uniformBuffersToUse.push_back(m_uniformBuffer);
        }
    }
}

void SGCore::AtmosphereUpdater::fixedUpdate(const SGCore::Ref<SGCore::Scene>& scene)
{
    auto atmosphereScatteringsView = scene->getECSRegistry().view<Atmosphere>();
    
    atmosphereScatteringsView.each([&scene, this](const entt::entity& entity, Atmosphere& atmosphere) {
        size_t hashedSunPos = MathUtils::hashVector(atmosphere.m_sunPosition);

        Mesh* atmosphereScatteringMesh = scene->getECSRegistry().try_get<Mesh>(entity);
        if(atmosphereScatteringMesh)
        {
            auto geomPassShader = atmosphereScatteringMesh->m_base.m_meshData->m_material->getShader()->getSubPassShader("GeometryPass");
            if(geomPassShader)
            {
                geomPassShader->bind();
                geomPassShader->useUniformBuffer(m_uniformBuffer);
            }
        }

        if(atmosphere.m_sunRotation != atmosphere.m_lastSunRotation)
        {
            atmosphere.m_sunPosition = glm::rotateX(Atmosphere::getSunOrigin(), glm::radians(atmosphere.m_sunRotation.x));
            atmosphere.m_sunPosition = glm::rotateY(atmosphere.m_sunPosition, glm::radians(atmosphere.m_sunRotation.y));
            atmosphere.m_sunPosition = glm::rotateZ(atmosphere.m_sunPosition, glm::radians(atmosphere.m_sunRotation.z));
            
            if(atmosphere.m_precalculatedSunColors.find(hashedSunPos) == atmosphere.m_precalculatedSunColors.end())
            {
                atmosphere.m_precalculatedSunColors[hashedSunPos] = AtmosphereUtils::calculateSunColor(atmosphere);
            }
            
            m_uniformBuffer->subData("atmosphere.sunPosition", glm::value_ptr(atmosphere.m_sunPosition), 3);
            m_uniformBuffer->subData("atmosphere.sunColor", glm::value_ptr(atmosphere.m_precalculatedSunColors[hashedSunPos]), 3);

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
