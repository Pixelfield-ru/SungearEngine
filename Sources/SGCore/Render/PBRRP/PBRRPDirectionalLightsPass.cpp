//
// Created by stuka on 26.11.2023.
//

#include <glm/gtc/type_ptr.hpp>

#include "PBRRPDirectionalLightsPass.h"
#include "PBRRPGeometryPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Render/Mesh.h"

SGCore::PBRRPDirectionalLightsPass::PBRRPDirectionalLightsPass() noexcept
{
    m_maxLightsCount = 5;
    
    m_lightsUniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    m_lightsUniformBuffer->m_blockName = "DirectionalLightsBlock";
    for(int i = 0; i < m_maxLightsCount; ++i)
    {
        std::string currentDirLight = "directionalLights[" + std::to_string(i) + "]";

        m_lightsUniformBuffer->putUniforms({
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
    
    m_lightsUniformBuffer->putUniforms({
        IShaderUniform("directionalLightsCount", SGGDataType::SGG_INT)
    });
    
    m_lightsUniformBuffer->setLayoutLocation(3);
    m_lightsUniformBuffer->prepare();

    m_renderTimer.m_useFixedUpdateCatchUp = false;
    m_renderTimer.m_targetFrameRate = 24;

    m_renderTimer.addCallback(m_renderTimerCallback);
}

void SGCore::PBRRPDirectionalLightsPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    m_renderTimerCallback->setUpdateFunction([&]() {
        m_lightsUniformBuffer->bind();

        auto directionalLightsView = scene->getECSRegistry().view<DirectionalLight, RenderingBase, Transform>();

        // std::cout << "upd : " << std::endl;
        
        int currenLightIdx = 0;
        
        directionalLightsView.each([&currenLightIdx, this](DirectionalLight& directionalLight, RenderingBase& renderingBase, Transform& transform) {
            if(currenLightIdx < m_maxLightsCount)
            {
                std::string currentDirLight = "directionalLights[" + std::to_string(currenLightIdx) + "]";

                if(directionalLight.m_base.m_color != directionalLight.m_base.m_lastColor)
                {
                    std::cout << "currentDirLight : " << currentDirLight << ", col : " << directionalLight.m_base.m_color.r << ", " << directionalLight.m_base.m_color.g << ", " << directionalLight.m_base.m_color.b << std::endl;
                    
                    m_lightsUniformBuffer->subData(currentDirLight + ".color", glm::value_ptr(directionalLight.m_base.m_color), 4);
                    
                    // char* col = m_lightsUniformBuffer->getUniform(currentDirLight + ".color");
                    
                    // std::cout << "col : " << *(float*) col << ", " << *(float*) (col + 4) << ", " << *(float*) (col + 8) << ", " << *(float*) (col + 12) << std::endl;
                    
                    directionalLight.m_base.m_lastColor = directionalLight.m_base.m_color;
                }

                if(directionalLight.m_base.m_intensity != directionalLight.m_base.m_lastIntensity)
                {
                    m_lightsUniformBuffer->subData(currentDirLight + ".intensity", { directionalLight.m_base.m_intensity } );

                    directionalLight.m_base.m_lastIntensity = directionalLight.m_base.m_intensity;
                }

                // todo make observer
                m_lightsUniformBuffer->subData(currentDirLight + ".position", glm::value_ptr(transform.m_ownTransform.m_position), 3);
                
                ++currenLightIdx;
            }
            
            m_lightsUniformBuffer->subData("directionalLightsCount", { currenLightIdx });
        });

        // m_lightsUniformBuffer->subData("directionalLightsCount", { currentLightIdx });
        
        // std::cout << "endupd : " << std::endl;
    });

    /*m_renderTimerCallback->setUpdateFunction([&, renderPipeline]()
                                             {
                                                 auto geometryPass = renderPipeline->getRenderPass<PBRRPGeometryPass>();

                                                 if (geometryPass || !geometryPass->m_active)
                                                 {
                                                     // ------ updating all lights in geom shader -------------

                                                     size_t directionalLightsCount = 0;

                                                     SG_BEGIN_ITERATE_CACHED_ENTITIES(
                                                             *m_componentsToRenderIn,
                                                             dirLightsLayer, directionalLightEntity)

                                                             auto directionalLightComponents =
                                                                     directionalLightEntity.getComponents<PBRRPDirectionalLight>();

                                                             auto directionalLightTransform =
                                                                     directionalLightEntity.getComponent<TransformBase>();

                                                             if (!directionalLightTransform) continue;

                                                             // TODO: OPTIMIZE
                                                             for (const auto& directionalLightComponent: directionalLightComponents)
                                                             {
                                                                 // TODO: MOVE
                                                                 *//*geometryPass->m_shader->bind();

                                                                 std::string directionalLightString =
                                                                         "directionalLights[" +
                                                                         std::to_string(directionalLightsCount) + "]";
                                                                 std::string lightPartString =
                                                                         directionalLightString + ".lightPart";
                                                                 std::string renderingPartString =
                                                                         lightPartString + ".renderingPart";

                                                                 geometryPass->m_shader->useVectorf(
                                                                         lightPartString + ".color",
                                                                         directionalLightComponent->m_color
                                                                 );

                                                                 geometryPass->m_shader->useFloat(
                                                                         lightPartString + ".intensity",
                                                                         directionalLightComponent->m_intensity
                                                                 );

                                                                 geometryPass->m_shader->useInteger(
                                                                         lightPartString + ".shadowSamplesCount",
                                                                         directionalLightComponent->m_samplesCount
                                                                 );


                                                                 geometryPass->m_shader->useVectorf(
                                                                         renderingPartString + ".position",
                                                                         directionalLightTransform->m_position
                                                                 );

                                                                 geometryPass->m_shader->useMatrix(
                                                                         renderingPartString + ".spaceMatrix",
                                                                         directionalLightComponent->m_spaceMatrix
                                                                 );*//*

                                                                 directionalLightComponent->m_shadowMap->bind()->clear();

                                                                 CoreMain::getRenderer()->prepareUniformBuffers(
                                                                         directionalLightComponent, nullptr
                                                                 );

                                                                 SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRender,
                                                                                                  meshesLayer,
                                                                                                  meshesEntity)
                                                                         Ref<TransformBase> transformComponent = meshesEntity.getComponent<TransformBase>();

                                                                         if (!transformComponent) continue;

                                                                         auto meshComponents =
                                                                                 meshesEntity.getComponents<Mesh>();

                                                                         for (const auto& meshComponent: meshComponents)
                                                                         {
                                                                             auto meshSubPassShader = meshComponent->m_meshData->m_material->getShader()->getSubPassShader("PBRRPDirectionalLightsPass");

                                                                             if(!meshSubPassShader) continue;

                                                                             meshSubPassShader->bind();
                                                                             meshSubPassShader->useMatrix("objectModelMatrix",
                                                                                                 transformComponent->m_modelMatrix
                                                                             );

                                                                             meshSubPassShader->useUniformBuffer(
                                                                                     CoreMain::getRenderer()->m_viewMatricesBuffer
                                                                             );

                                                                             CoreMain::getRenderer()->renderMeshData(
                                                                                     meshComponent->m_meshData,
                                                                                     meshComponent->m_meshDataRenderInfo
                                                                             );
                                                                         }
                                                                 SG_END_ITERATE_CACHED_ENTITIES

                                                                 directionalLightComponent->m_shadowMap->unbind();

                                                                 ++directionalLightsCount;
                                                             }
                                                     SG_END_ITERATE_CACHED_ENTITIES

                                                     // TODO: MOVE
                                                     *//*geometryPass->m_shader->bind();
                                                     geometryPass->m_shader->useInteger("DIRECTIONAL_LIGHTS_COUNT",
                                                                                        directionalLightsCount
                                                     );*//*
                                                 }
                                             }
    );*/
}
