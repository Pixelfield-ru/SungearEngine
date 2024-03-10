//
// Created by stuka on 25.11.2023.
//

#include <entt/entt.hpp>

#include "PBRRPGeometryPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Render/PostProcessFrameReceiver.h"
#include "SGCore/Render/Mesh.h"
#include "PBRRPDirectionalLightsPass.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/ShaderComponent.h"
#include "SGCore/Render/SpacePartitioning/CullableMesh.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"

void SGCore::PBRRPGeometryPass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline)
{
    m_shader = MakeRef<IShader>();
    m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            parentRenderPipeline->m_shadersPaths.getByVirtualPath("StandardMeshShader").getCurrentRealization()));
}

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

    // scene->getECSRegistry();
    auto& registry = scene->getECSRegistry();
    
    auto camerasView = registry.view<Ref<Camera3D>, Ref<RenderingBase>, Ref<Transform>>();
    auto meshesView = registry.view<EntityBaseInfo, Mesh, Ref<Transform>>(entt::exclude<DisableMeshGeometryPass>);
    
    Ref<ISubPassShader> standardGeometryShader;
    if(m_shader)
    {
        standardGeometryShader = m_shader->getSubPassShader("GeometryPass");
    }
    
    if(standardGeometryShader)
    {
        standardGeometryShader->bind();
    }
    
    camerasView.each([&meshesView, &renderPipeline, &standardGeometryShader, &scene, &registry, this]
    (const entt::entity& cameraEntity, Ref<Camera3D>& camera3D, Ref<RenderingBase>& cameraRenderingBase, Ref<Transform>& cameraTransform) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(standardGeometryShader)
        {
            standardGeometryShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        // todo: make get receiver (postprocess or default) and render in them

        meshesView.each([&renderPipeline, &standardGeometryShader, &scene, &cameraEntity, &registry, this]
        (const entt::entity& meshEntity, EntityBaseInfo& meshedEntityBaseInfo, Mesh& mesh, Ref<Transform>& meshTransform) {
            auto* tmpCullableMesh = registry.try_get<Ref<CullableMesh>>(meshEntity);
            Ref<CullableMesh> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);
            
            bool willRender = registry.try_get<IgnoreOctrees>(meshEntity) || !cullableMesh || cullableMesh->m_visibleCameras.contains(cameraEntity);
            
            if(willRender)
            {
                ShaderComponent* entityShader = scene->getECSRegistry().try_get<ShaderComponent>(meshEntity);
                
                auto meshGeomShader = (entityShader && entityShader->m_shader)
                                      ? entityShader->m_shader->getSubPassShader("GeometryPass") : nullptr;
                auto shaderToUse = meshGeomShader ? meshGeomShader : standardGeometryShader;
                
                if(shaderToUse)
                {
                    if(shaderToUse == meshGeomShader)
                    {
                        shaderToUse->bind();
                        shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
                    }
                    
                    {
                        shaderToUse->useMatrix("objectTransform.modelMatrix",
                                               meshTransform->m_finalTransform.m_modelMatrix);
                        shaderToUse->useVectorf("objectTransform.position", meshTransform->m_finalTransform.m_position);
                    }
                    
                    size_t offset0 = shaderToUse->bindMaterialTextures(mesh.m_base.m_meshData->m_material);
                    shaderToUse->bindTextureBindings(offset0);
                    
                    auto uniformBuffsIt = m_uniformBuffersToUse.begin();
                    while(uniformBuffsIt != m_uniformBuffersToUse.end())
                    {
                        if(auto lockedUniformBuf = uniformBuffsIt->lock())
                        {
                            shaderToUse->useUniformBuffer(lockedUniformBuf);
                            
                            ++uniformBuffsIt;
                        }
                        else
                        {
                            uniformBuffsIt = m_uniformBuffersToUse.erase(uniformBuffsIt);
                        }
                    }
                    
                    CoreMain::getRenderer()->renderMeshData(
                            mesh.m_base.m_meshData,
                            mesh.m_base.m_meshDataRenderInfo
                    );
                    
                    shaderToUse->unbindMaterialTextures(mesh.m_base.m_meshData->m_material);
                    
                    if(shaderToUse == meshGeomShader)
                    {
                        if(standardGeometryShader)
                        {
                            standardGeometryShader->bind();
                        }
                    }
                }
            }
        });
    });

    /*SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            auto cameraTransformComponent = cameraEntity.getComponent<TransformBase>();
            if(!cameraTransformComponent) continue;
            auto cameraComponent = cameraEntity.getComponent<PostProcessFrameReceiver>();
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
                    Ref<TransformBase> transformComponent = meshesEntity.second.getComponent<TransformBase>();

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

    SG_END_ITERATE_CACHED_ENTITIES*/
}
