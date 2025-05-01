//
// Created by stuka on 25.11.2023.
//

#include <entt/entt.hpp>

#include "PBRRPGeometryPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "PBRRPDirectionalLightsPass.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/SpacePartitioning/OctreeCullable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Render/SpacePartitioning/Octree.h"
#include "SGCore/Render/SpacePartitioning/ObjectsCullingOctree.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Render/Terrain.h"
#include "SGCore/Render/Decals/Decal.h"

size_t renderedInOctrees = 0;

void SGCore::PBRRPGeometryPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *parentRenderPipeline->m_shadersPaths["StandardMeshShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    // configuring default material to use standard pbr shader
    auto defaultMaterial = AssetManager::getInstance()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
    defaultMaterial->m_shader = m_shader;

    m_opaqueEntitiesRenderState.m_depthFunc = SGDepthStencilFunc::SGG_LESS;
    m_opaqueEntitiesRenderState.m_globalBlendingState.m_useBlending = false;

    // m_transparentEntitiesRenderState.m_depthMask = false;
    m_transparentEntitiesRenderState.m_globalBlendingState.m_useBlending = true;
    /*m_transparentEntitiesRenderState.m_globalBlendingState.m_useBlending = true;
    m_transparentEntitiesRenderState.m_globalBlendingState.m_blendingEquation = SGEquation::SGG_FUNC_ADD;*/
}

void SGCore::PBRRPGeometryPass::render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline)
{
    Ref<PBRRPDirectionalLightsPass> dirLightsPass = renderPipeline->getRenderPass<PBRRPDirectionalLightsPass>();
    
    // scene->getECSRegistry();
    auto registry = scene->getECSRegistry();
    
    auto camerasView = registry->view<EntityBaseInfo, RenderingBase, Transform>();
    auto opaqueMeshesView = registry->view<EntityBaseInfo, Mesh, Transform, OpaqueEntityTag>(ECS::ExcludeTypes<DisableMeshGeometryPass, Decal>{});
    auto transparentMeshesView = registry->view<EntityBaseInfo, Mesh, Transform, TransparentEntityTag>(ECS::ExcludeTypes<DisableMeshGeometryPass, Decal>{});
    auto terrainsView = registry->view<EntityBaseInfo, Mesh, Transform, Terrain>(ECS::ExcludeTypes<DisableMeshGeometryPass, Decal>{});

    if(m_shader)
    {
        m_shader->bind();
    }
    
    camerasView.each([&opaqueMeshesView, &transparentMeshesView, &terrainsView, &renderPipeline, &scene, &registry, this]
                             (const ECS::entity_t& cameraEntity,
                              const EntityBaseInfo::reg_t& camera3DBaseInfo,
                              RenderingBase::reg_t& cameraRenderingBase, Transform::reg_t& cameraTransform) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(m_shader)
        {
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        LayeredFrameReceiver* cameraLayeredFrameReceiver = registry->tryGet<LayeredFrameReceiver>(cameraEntity);

        if(cameraLayeredFrameReceiver)
        {
            cameraLayeredFrameReceiver->m_layersFrameBuffer->bind();
            cameraLayeredFrameReceiver->m_layersFrameBuffer->bindAttachmentsToDrawIn(
                    cameraLayeredFrameReceiver->m_attachmentToRenderIn
            );
        }

        m_opaqueEntitiesRenderState.use();

        terrainsView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, this](
            const ECS::entity_t& terrainEntity,
            EntityBaseInfo::reg_t& meshedEntityBaseInfo,
            Mesh::reg_t& mesh,
            Transform::reg_t& meshTransform,
            const Terrain::reg_t& terrain) {
                Ref<PostProcessLayer> meshPPLayer = mesh.m_base.m_layeredFrameReceiversMarkup[cameraLayeredFrameReceiver].lock();

                if(cameraLayeredFrameReceiver)
                {
                    if(!meshPPLayer)
                    {
                        meshPPLayer = cameraLayeredFrameReceiver->getDefaultLayer();
                    }
                }

                SG_ASSERT(meshPPLayer != nullptr,
                          "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

                glPatchParameteri(GL_PATCH_VERTICES, 4);

                renderMesh(registry, terrainEntity, meshTransform, mesh,
                           meshedEntityBaseInfo, camera3DBaseInfo, meshPPLayer, false, cameraLayeredFrameReceiver);
            });

        // =====================================================================================================
        // =====================================================================================================
        // =====================================================================================================

        opaqueMeshesView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, this]
                                (const ECS::entity_t& meshEntity, EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                                 Mesh::reg_t& mesh, Transform::reg_t& meshTransform,
                                 const auto&) {
            auto* tmpCullableMesh = registry->tryGet<OctreeCullable>(meshEntity);
            Ref<OctreeCullable> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);
            
            bool willRender = registry->tryGet<IgnoreOctrees>(meshEntity) || !cullableMesh;
            
            if(willRender)
            {
                Ref<PostProcessLayer> meshPPLayer = mesh.m_base.m_layeredFrameReceiversMarkup[cameraLayeredFrameReceiver].lock();

                if(cameraLayeredFrameReceiver)
                {
                    if(!meshPPLayer)
                    {
                        meshPPLayer = cameraLayeredFrameReceiver->getDefaultLayer();
                    }
                }

                SG_ASSERT(meshPPLayer != nullptr,
                          "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

                renderMesh(registry, meshEntity, meshTransform, mesh, meshedEntityBaseInfo, camera3DBaseInfo,
                           meshPPLayer, false, cameraLayeredFrameReceiver);
            }
        });

        m_transparentEntitiesRenderState.use();
        if(cameraLayeredFrameReceiver)
        {
            cameraLayeredFrameReceiver->m_layersFrameBuffer->useStates();
        }

        transparentMeshesView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, this]
                                      (const ECS::entity_t& meshEntity, EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                                       Mesh::reg_t& mesh, Transform::reg_t& meshTransform,
                                       const auto&) {
            auto* tmpCullableMesh = registry->tryGet<OctreeCullable>(meshEntity);
            Ref<OctreeCullable> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);

            bool willRender = registry->tryGet<IgnoreOctrees>(meshEntity) || !cullableMesh;

            if(willRender)
            {
                Ref<PostProcessLayer> meshPPLayer = mesh.m_base.m_layeredFrameReceiversMarkup[cameraLayeredFrameReceiver].lock();

                if(cameraLayeredFrameReceiver)
                {
                    if(!meshPPLayer)
                    {
                        meshPPLayer = cameraLayeredFrameReceiver->getDefaultLayer();
                    }
                }

                SG_ASSERT(meshPPLayer != nullptr,
                          "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

                renderMesh(registry, meshEntity, meshTransform, mesh,
                           meshedEntityBaseInfo, camera3DBaseInfo, meshPPLayer, true, cameraLayeredFrameReceiver);
            }
        });

        if(cameraLayeredFrameReceiver)
        {
            cameraLayeredFrameReceiver->m_layersFrameBuffer->unbind();
        }
    });
    
    // ==========================================
    // ============= rendering octrees ==========
    // ==========================================
    
    renderedInOctrees = 0;
    
    camerasView.each([&opaqueMeshesView, &transparentMeshesView,
                      &renderPipeline, &scene, &registry, this]
                             (const ECS::entity_t& cameraEntity,
                              const EntityBaseInfo::reg_t& camera3DBaseInfo,
                              RenderingBase::reg_t& cameraRenderingBase,
                              Transform::reg_t& cameraTransform) {
        
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(m_shader)
        {
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        LayeredFrameReceiver* cameraLayeredFrameReceiver = registry->tryGet<LayeredFrameReceiver>(cameraEntity);
        
        auto objectsCullingOctreesView = registry->view<Octree, ObjectsCullingOctree>();
        objectsCullingOctreesView.each([&cameraLayeredFrameReceiver, &scene, &cameraEntity, &registry, &camera3DBaseInfo, &cameraRenderingBase, this]
        (Octree::reg_t& octree, const ObjectsCullingOctree::reg_t&) {
            for(const auto& n : octree->m_notEmptyNodes)
            {
                renderOctreeNode(registry, camera3DBaseInfo, cameraEntity, cameraLayeredFrameReceiver, n);
            }
        });
    });

    m_afterRenderState.use();

    // std::cout << "renderedInOctrees: " << renderedInOctrees << std::endl;
}

void SGCore::PBRRPGeometryPass::renderMesh(const Ref<ECS::registry_t>& registry,
                                           const ECS::entity_t& meshEntity,
                                           const Ref<Transform>& meshTransform,
                                           Mesh& mesh,
                                           EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                                           const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                                           const Ref<PostProcessLayer>& meshPPLayer,
                                           bool isTransparentPass,
                                           LayeredFrameReceiver::reg_t* forLayeredFrameReceiver) noexcept
{
    /*if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial() ||
       !mesh.m_base.getMaterial()->m_shader) return;*/

    if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial()) return;

    const auto& meshGeomShader = mesh.m_base.getMaterial()->m_shader;
    const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;
    
    if(shaderToUse)
    {
        // binding shaderToUse only if it is custom shader
        if(shaderToUse == meshGeomShader)
        {
            shaderToUse->bind();
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        {
            shaderToUse->useMatrix("objectTransform.modelMatrix",
                                   meshTransform->m_finalTransform.m_animatedModelMatrix);
            shaderToUse->useVectorf("objectTransform.position", meshTransform->m_finalTransform.m_position);

            const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(meshEntity);
            // enable picking
            if(meshedEntityPickableComponent &&
               meshedEntityPickableComponent->isPickableForCamera(forCamera3DBaseInfo.getThisEntity()))
            {
                shaderToUse->useVectorf("u_pickingColor", meshedEntityBaseInfo.getUniqueColor());
            }
            else
            {
                shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
            }
        }

        shaderToUse->useInteger("u_isTransparentPass", isTransparentPass ? 1 : 0);
        shaderToUse->useInteger("u_verticesColorsAttributesCount", mesh.m_base.getMeshData()->m_verticesColors.size());

        if(meshPPLayer)
        {
            shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());
        }

        // 14 MS FOR loc0 IN DEBUG
        size_t offset0 = shaderToUse->bindMaterialTextures(mesh.m_base.getMaterial());
        shaderToUse->bindTextureBindings(offset0);
        shaderToUse->useMaterialFactors(mesh.m_base.getMaterial().get());
        
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

        // using texture buffer with bones animated transformations
        if(auto bonesLockedBuffer = mesh.m_base.m_bonesBuffer.lock())
        {
            bonesLockedBuffer->bind(offset0);
            shaderToUse->useTextureBlock("u_bonesMatricesUniformBuffer", offset0);
            ++offset0;

            shaderToUse->useInteger("u_isAnimatedMesh", 1);
        }
        else
        {
            shaderToUse->useInteger("u_isAnimatedMesh", 0);
        }

        bool lastUseFacesCulling = mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling;
        if(isTransparentPass)
        {
            mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling = false;
        }

        // 15 ms for map loc0 IN DEBUG
        CoreMain::getRenderer()->renderMeshData(
                mesh.m_base.getMeshData().get(),
                mesh.m_base.getMaterial()->m_meshRenderState
        );

        if(isTransparentPass)
        {
            mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling = lastUseFacesCulling;
        }

        // HOLY SHMOLY!! 10 MS FOR MAP loc0 IN DEBUG. DO NOT USE THIS!!!
        // shaderToUse->unbindMaterialTextures(mesh.m_base.getMaterial());

        // if we used custom shader then we must bind standard pipeline shader
        if(shaderToUse == meshGeomShader)
        {
            if(m_shader)
            {
                m_shader->bind();
            }
        }
    }
}

void SGCore::PBRRPGeometryPass::renderOctreeNode(const Ref<ECS::registry_t>& registry,
                                                 const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                                                 const ECS::entity_t& forCamera,
                                                 LayeredFrameReceiver* cameraLayeredFrameReceiver,
                                                 const SGCore::Ref<SGCore::OctreeNode>& node) noexcept
{
    bool isVisibleForCamera = node->m_visibleReceivers.contains(forCamera);
    
    if(isVisibleForCamera)
    {
        // render all entities
        for(const auto& e : node->m_overlappedEntities)
        {
            auto* tmpCullableInfo = registry->tryGet<OctreeCullable>(e);
            Ref<OctreeCullable> cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
            
            if(!cullableInfo) continue;
            
            Mesh* mesh = registry->tryGet<Mesh>(e);
            auto* tmpMeshTransform = registry->tryGet<Transform>(e);
            auto meshTransform = (tmpMeshTransform ? *tmpMeshTransform : nullptr);
            
            if(meshTransform && mesh)
            {
                ++renderedInOctrees;
                
                auto* entityBaseInfo = registry->tryGet<EntityBaseInfo>(e);
                SG_ASSERT(entityBaseInfo != nullptr, "In octree culling: can not render entity that contains mesh does not contain EntityBaseInfo.");
                Ref<PostProcessLayer> meshPPLayer = mesh->m_base.m_layeredFrameReceiversMarkup[cameraLayeredFrameReceiver].lock();
                
                if(cameraLayeredFrameReceiver)
                {
                    if(!meshPPLayer)
                    {
                        meshPPLayer = cameraLayeredFrameReceiver->getDefaultLayer();
                    }
                }

                if(cameraLayeredFrameReceiver)
                {
                    SG_ASSERT(meshPPLayer != nullptr,
                              "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

                    cameraLayeredFrameReceiver->m_layersFrameBuffer->bind();
                    cameraLayeredFrameReceiver->m_layersFrameBuffer->bindAttachmentsToDrawIn(
                            cameraLayeredFrameReceiver->m_attachmentToRenderIn
                    );
                }
                
                renderMesh(registry, e, meshTransform, *mesh, *entityBaseInfo, forCamera3DBaseInfo,
                           meshPPLayer, false, cameraLayeredFrameReceiver);

                if(cameraLayeredFrameReceiver)
                {
                    cameraLayeredFrameReceiver->m_layersFrameBuffer->unbind();
                }
            }
        }
    }
}
