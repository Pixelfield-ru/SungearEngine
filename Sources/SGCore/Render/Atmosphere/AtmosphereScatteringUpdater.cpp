//
// Created by ilya on 14.02.24.
//

#include <glm/gtx/rotate_vector.hpp>

#include "AtmosphereScatteringUpdater.h"

#include "SGCore/Scene/Scene.h"
#include "AtmosphereScattering.h"
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

SGCore::AtmosphereScatteringUpdater::AtmosphereScatteringUpdater() noexcept
{
    m_uniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    
    m_uniformBuffer->m_blockName = "SunBlock";
    m_uniformBuffer->putUniforms({
        IShaderUniform("sunColor", SGGDataType::SGG_FLOAT4),
        IShaderUniform("sunPosition", SGGDataType::SGG_FLOAT3),
        IShaderUniform("p0", SGGDataType::SGG_FLOAT)
    });
    
    for(size_t i = 0; i < 128; ++i)
    {
        m_uniformBuffer->putUniforms({
            IShaderUniform("sunColors[" + std::to_string(i) + "]",SGGDataType::SGG_FLOAT)
        });
    }
    
    m_uniformBuffer->setLayoutLocation(4);
    m_uniformBuffer->prepare();
    
    m_uniformBuffer->subData("sunColor", { 1.0f, 1.0f, 1.0f, 1.0f });
    
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

void SGCore::AtmosphereScatteringUpdater::fixedUpdate(const SGCore::Ref<SGCore::Scene>& scene)
{
    auto atmosphereScatteringsView = scene->getECSRegistry().view<AtmosphereScattering>();
    
    atmosphereScatteringsView.each([&scene, this](const entt::entity& entity, AtmosphereScattering& atmosphereScattering) {
        if(atmosphereScattering.m_sunRotation != atmosphereScattering.m_lastSunRotation)
        {
            atmosphereScattering.m_sunPosition = glm::rotateX(AtmosphereScattering::getSunOrigin(), glm::radians(atmosphereScattering.m_sunRotation.x));
            atmosphereScattering.m_sunPosition = glm::rotateY(atmosphereScattering.m_sunPosition, glm::radians(atmosphereScattering.m_sunRotation.y));
            atmosphereScattering.m_sunPosition = glm::rotateZ(atmosphereScattering.m_sunPosition, glm::radians(atmosphereScattering.m_sunRotation.z));
            
            size_t hashedVec = MathUtils::hashVector(atmosphereScattering.m_sunPosition);
            
            if(atmosphereScattering.m_precalculatedSunColors.find(hashedVec) == atmosphereScattering.m_precalculatedSunColors.end())
            {
                atmosphereScattering.m_precalculatedSunColors[hashedVec] = AtmosphereUtils::rayleighMieAtmosphere(
                        atmosphereScattering.m_sunPosition,                                 // normalized ray direction
                        { 0, 6372e3, 0 },                                         // ray origin
                        atmosphereScattering.m_sunPosition,                             // position of the sun
                        30.0,                                                           // intensity of the sun
                        6371e3,                                                         // radius of the planet in meters
                        6471e3,                                                         // radius of the atmosphere in meters
                        { 14.5e-6, 15.0e-6, 25.4e-6 },                          // Rayleigh scattering coefficient
                        5e-6,                                                           // Mie scattering coefficient
                        8e3,                                                            // Rayleigh scale height
                        1.0e3,                                                          // Mie scale height
                        0.958,                                                              // Mie preferred scattering direction,
                        16,                                                             // i steps
                        8                                                               // j steps
                );
            }
            
            Mesh* atmosphereScatteringMesh = scene->getECSRegistry().try_get<Mesh>(entity);
            if(atmosphereScatteringMesh)
            {
                auto geomPassShader = atmosphereScatteringMesh->m_base.m_meshData->m_material->getShader()->getSubPassShader("GeometryPass");
                if(geomPassShader)
                {
                    geomPassShader->bind();
                    geomPassShader->useUniformBuffer(m_uniformBuffer);
                    geomPassShader->useVectorf("sunPosition", atmosphereScattering.m_sunPosition);
                }
            }
            
            m_uniformBuffer->subData("sunPosition", glm::value_ptr(atmosphereScattering.m_sunPosition), 3);
            
            atmosphereScattering.m_lastSunRotation = atmosphereScattering.m_sunRotation;
        }
    });
}
