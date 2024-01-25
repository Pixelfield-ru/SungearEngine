//
// Created by stuka on 25.11.2023.
//

#include "PBRRPGeometryPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Render/ICamera.h"
#include "SGCore/Render/Mesh.h"
#include "PBRRPDirectionalLightsPass.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Render/IRenderPipeline.h"

void SGCore::PBRRPGeometryPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    Ref<PBRRPDirectionalLightsPass> dirLightsPass = renderPipeline->getRenderPass<PBRRPDirectionalLightsPass>();

    // m_shader->bind();

    /*if(dirLightsPass && dirLightsPass->m_active)
    {
        const auto& shadowsMapsTexturesBlock =
                m_shaderMarkup.m_texturesBlocks[SGTextureType::SGTP_SHADOW_MAP2D];

        std::uint8_t currentShadowsCaster = 0;

        SG_BEGIN_ITERATE_CACHED_ENTITIES(*dirLightsPass->m_componentsToRenderIn, shadowsCastersLayer,
                                         shadowsCasterEntity)
                // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
                auto shadowsCaster = shadowsCasterEntity.getComponent<PBRRPDirectionalLight>();

                if(!shadowsCaster) continue;

                shadowsCaster->m_shadowMap->bindAttachment(
                        SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                        shadowsMapsTexturesBlock.m_offset + currentShadowsCaster
                );

                currentShadowsCaster++;
        SG_END_ITERATE_CACHED_ENTITIES
    }*/



    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            auto cameraTransformComponent = cameraEntity.getComponent<Transform>();
            if(!cameraTransformComponent) continue;
            auto cameraComponent = cameraEntity.getComponent<ICamera>();
            if(!cameraComponent) continue;

            CoreMain::getRenderer()->prepareUniformBuffers(cameraComponent, cameraTransformComponent);
            // m_shader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            // todo: make less bindings

            for(auto& meshesLayer : *m_componentsToRender)
            {
                const auto& layer = meshesLayer.first;

                // cameraComponent->bindPostProcessFrameBuffer(layer);

                for(auto& meshesEntity: meshesLayer.second)
                {
                    Ref<Transform> transformComponent = meshesEntity.second.getComponent<Transform>();

                    if(!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second.getComponents<Mesh>();

                    for(const auto& meshComponent: meshComponents)
                    {
                        auto meshGeometryShader = meshComponent->m_meshData->m_material->getShader()->getSubPassShader("PBRRPGeometryPass");

                        if(!meshGeometryShader) continue;

                        meshGeometryShader->bind();
                        meshGeometryShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
                        meshGeometryShader->useMatrix("objectModelMatrix",
                                                      transformComponent->m_modelMatrix
                        );

                        CoreMain::getRenderer()->renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
                        );
                    }
                }
            }

    SG_END_ITERATE_CACHED_ENTITIES
}
