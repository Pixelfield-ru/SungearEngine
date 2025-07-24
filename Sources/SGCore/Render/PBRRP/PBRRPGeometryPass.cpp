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
#include "SGCore/Render/Batching/Batch.h"
#include "SGCore/Render/Decals/Decal.h"
#include "SGCore/Render/Terrain/Terrain.h"
#include "SGCore/Render/Batching/Batch.h"
#include "SGCore/Render/Instancing/Instancing.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"

size_t renderedInOctrees = 0;

void SGCore::PBRRPGeometryPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    // loading object standard shader
    {
        auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *parentRenderPipeline->m_shadersPaths["StandardMeshShader"]);

        m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

        m_instancingShader = AssetManager::getInstance()->loadAssetWithAlias<IShader>("PBRRPGeometryInstancingShader", shaderFile->getPath());
        m_instancingShader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_INSTANCED_RENDERING", ""));
        m_instancingShader->recompile();
    }

    // configuring default material to use standard pbr shader
    auto defaultMaterial = AssetManager::getInstance()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
    defaultMaterial->m_shaders["GeometryPass"] = m_shader;

    {
        auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *parentRenderPipeline->m_shadersPaths["BatchingShader"]);

        m_batchShader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());
    }

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
    auto batchesView = registry->view<Batch>(ECS::ExcludeTypes<DisableMeshGeometryPass>{});
    auto instancingView = registry->view<EntityBaseInfo, Instancing>(ECS::ExcludeTypes<DisableMeshGeometryPass, Decal>{});
    
    camerasView.each([&opaqueMeshesView, &transparentMeshesView, &terrainsView, &batchesView, &registry, &instancingView, this]
                             (const ECS::entity_t& cameraEntity,
                              const EntityBaseInfo::reg_t& camera3DBaseInfo,
                              RenderingBase::reg_t& cameraRenderingBase, Transform::reg_t& cameraTransform) {
        const auto* cameraCSMTarget = registry->tryGet<CSMTarget>(cameraEntity);

        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(m_shader)
        {
            m_shader->bind();
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
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
            EntityBaseInfo::reg_t& terrainEntityBaseInfo,
            Mesh::reg_t& mesh,
            Transform::reg_t& terrainTransform,
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

                renderTerrainMesh(registry, terrainEntity, terrainTransform, mesh, terrain, terrainEntityBaseInfo, camera3DBaseInfo, meshPPLayer);
            });

        // =====================================================================================================
        // =====================================================================================================
        // =====================================================================================================

        opaqueMeshesView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, &cameraRenderingBase, &cameraCSMTarget, this]
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
                           cameraRenderingBase, cameraCSMTarget,
                           meshPPLayer, false, cameraLayeredFrameReceiver);
            }
        });

        // =====================================================================================================
        // =====================================================================================================
        // =====================================================================================================

        // rendering batches
        batchesView.each([&cameraLayeredFrameReceiver, &registry, cameraEntity, &cameraRenderingBase, &cameraCSMTarget, this](Batch& batch) {
            // todo: add getting batch layer
            Ref<PostProcessLayer> meshPPLayer = cameraLayeredFrameReceiver->getDefaultLayer();

            batch.bind(m_batchShader.get());

            auto uniformBuffsIt = m_uniformBuffersToUse.begin();
            while(uniformBuffsIt != m_uniformBuffersToUse.end())
            {
                if(auto lockedUniformBuf = uniformBuffsIt->lock())
                {
                    m_batchShader.get()->useUniformBuffer(lockedUniformBuf);

                    ++uniformBuffsIt;
                }
                else
                {
                    uniformBuffsIt = m_uniformBuffersToUse.erase(uniformBuffsIt);
                }
            }

            if(cameraCSMTarget)
            {
                cameraCSMTarget->bindUniformsToShader(m_batchShader.get(), cameraRenderingBase->m_zFar, 5);
            }

            CoreMain::getRenderer()->renderArray(
                batch.getVertexArray(),
                batch.m_batchRenderState,
                batch.getTrianglesCount(),
                0
            );
        });

        // =====================================================================================================
        // =====================================================================================================
        // =====================================================================================================

        if(m_instancingShader)
        {
            m_instancingShader->bind();
            m_instancingShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            m_instancingShader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
        }

        // rendering instancing
        instancingView.each(
            [&cameraLayeredFrameReceiver, &camera3DBaseInfo, &registry, cameraEntity, &cameraRenderingBase, &cameraCSMTarget, this](
        const ECS::entity_t& instancingEntity,
        EntityBaseInfo::reg_t& instancingEntityBaseInfo,
        Instancing::reg_t& instancing) {
            // todo: make layer choose
            Ref<PostProcessLayer> meshPPLayer = cameraLayeredFrameReceiver->getDefaultLayer();

            // todo: make transparent pass
            bool isTransparentPass = false;

            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            if(!instancing.getBaseMeshData() ||
               !instancing.getBaseMaterial()) return;

            const auto& meshGeomShader = instancing.getBaseMaterial()->m_shaders["GeometryPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_instancingShader;

            if(shaderToUse)
            {
                // binding shaderToUse only if it is custom shader
                if(shaderToUse == meshGeomShader)
                {
                    shaderToUse->bind();
                    shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
                    shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
                }

                {
                    const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(instancingEntity);
                    // enable picking
                    if(meshedEntityPickableComponent &&
                       meshedEntityPickableComponent->isPickableForCamera(camera3DBaseInfo.getThisEntity()))
                    {
                        shaderToUse->useVectorf("u_pickingColor", instancingEntityBaseInfo.getUniqueColor());
                    }
                    else
                    {
                        shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
                    }
                }

                shaderToUse->useInteger("u_isTransparentPass", isTransparentPass ? 1 : 0);
                shaderToUse->useInteger("u_verticesColorsAttributesCount",
                                        instancing.getBaseMeshData()->m_verticesColors.size());
                // meshPPLayer is always valid
                shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());

                // 14 MS FOR loc0 IN DEBUG
                size_t texUnitOffset = shaderToUse->bindMaterialTextures(instancing.getBaseMaterial());
                shaderToUse->bindTextureBindings(texUnitOffset);
                shaderToUse->useMaterialFactors(instancing.getBaseMaterial().get());

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
                shaderToUse->useInteger("u_isAnimatedMesh", 0);

                if(cameraCSMTarget)
                {
                    texUnitOffset = cameraCSMTarget->bindUniformsToShader(
                        shaderToUse.get(), cameraRenderingBase->m_zFar, texUnitOffset);
                }

                bool lastUseFacesCulling = instancing.getBaseMaterial()->m_meshRenderState.m_useFacesCulling;
                if(isTransparentPass)
                {
                    instancing.getBaseMaterial()->m_meshRenderState.m_useFacesCulling = false;
                }

                CoreMain::getRenderer()->renderArrayInstanced(
                    instancing.getVertexArray(),
                    instancing.getBaseMaterial()->m_meshRenderState,
                    instancing.getBaseMeshData()->m_vertices.size(),
                    instancing.getBaseMeshData()->m_indices.size(),
                    instancing.m_entities.size()
                );

                std::cout << "rendering instanced: " << instancing.m_entities.size() << std::endl;

                if(isTransparentPass)
                {
                    instancing.getBaseMaterial()->m_meshRenderState.m_useFacesCulling = lastUseFacesCulling;
                }

                // if we used custom shader then we must bind standard pipeline shader
                if(shaderToUse == meshGeomShader)
                {
                    if(m_instancingShader)
                    {
                        m_instancingShader->bind();
                    }
                }
            }
        });

        // =====================================================================================================
        // =====================================================================================================
        // =====================================================================================================

        if(m_shader)
        {
            m_shader->bind();
        }

        m_transparentEntitiesRenderState.use();
        if(cameraLayeredFrameReceiver)
        {
            cameraLayeredFrameReceiver->m_layersFrameBuffer->useStates();
        }

        transparentMeshesView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, &cameraRenderingBase, &cameraCSMTarget, this]
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
                           meshedEntityBaseInfo, camera3DBaseInfo, cameraRenderingBase, cameraCSMTarget, meshPPLayer, true, cameraLayeredFrameReceiver);
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
        const auto* cameraCSMTarget = registry->tryGet<CSMTarget>(cameraEntity);

        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(m_shader)
        {
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
        }
        
        LayeredFrameReceiver* cameraLayeredFrameReceiver = registry->tryGet<LayeredFrameReceiver>(cameraEntity);
        
        auto objectsCullingOctreesView = registry->view<Octree, ObjectsCullingOctree>();
        objectsCullingOctreesView.each([&cameraLayeredFrameReceiver, &scene, &cameraEntity, &registry, &camera3DBaseInfo, &cameraRenderingBase, &cameraCSMTarget, this]
        (Octree::reg_t& octree, const ObjectsCullingOctree::reg_t&) {
            for(const auto& n : octree->m_notEmptyNodes)
            {
                renderOctreeNode(registry, camera3DBaseInfo, cameraRenderingBase, cameraCSMTarget, cameraEntity,
                                 cameraLayeredFrameReceiver, n);
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
                                           const RenderingBase::reg_t& cameraRenderingBase,
                                           const CSMTarget::reg_t* cameraCSMTarget,
                                           const Ref<PostProcessLayer>& meshPPLayer,
                                           bool isTransparentPass,
                                           LayeredFrameReceiver::reg_t* forLayeredFrameReceiver) noexcept
{
    /*if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial() ||
       !mesh.m_base.getMaterial()->m_shader) return;*/

    if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial()) return;

    const auto& meshGeomShader = mesh.m_base.getMaterial()->m_shaders["GeometryPass"];
    const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;
    
    if(shaderToUse)
    {
        // binding shaderToUse only if it is custom shader
        if(shaderToUse == meshGeomShader)
        {
            shaderToUse->bind();
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
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
        // meshPPLayer is always valid
        shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());

        // 14 MS FOR loc0 IN DEBUG
        size_t texUnitOffset = shaderToUse->bindMaterialTextures(mesh.m_base.getMaterial());
        shaderToUse->bindTextureBindings(texUnitOffset);
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
            bonesLockedBuffer->bind(texUnitOffset);
            shaderToUse->useTextureBlock("u_bonesMatricesUniformBuffer", texUnitOffset);
            ++texUnitOffset;

            shaderToUse->useInteger("u_isAnimatedMesh", 1);
        }
        else
        {
            shaderToUse->useInteger("u_isAnimatedMesh", 0);
        }

        if(cameraCSMTarget)
        {
            texUnitOffset = cameraCSMTarget->bindUniformsToShader(shaderToUse.get(), cameraRenderingBase->m_zFar, texUnitOffset);
        }

        bool lastUseFacesCulling = mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling;
        if(isTransparentPass)
        {
            mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling = false;
        }

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

void SGCore::PBRRPGeometryPass::renderTerrainMesh(const Ref<ECS::registry_t>& registry,
                                                  const ECS::entity_t& terrainEntity,
                                                  const Transform::reg_t& terrainTransform,
                                                  Mesh::reg_t& mesh,
                                                  const Terrain::reg_t& terrain,
                                                  EntityBaseInfo::reg_t& terrainEntityBaseInfo,
                                                  const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                                                  const Ref<PostProcessLayer>& terrainPPLayer) noexcept
{
    if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial()) return;

    const auto& meshGeomShader = mesh.m_base.getMaterial()->m_shaders["GeometryPass"];
    const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;

    if(shaderToUse)
    {
        // binding shaderToUse only if it is custom shader
        if(shaderToUse == meshGeomShader)
        {
            shaderToUse->bind();
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
        }

        {
            shaderToUse->useMatrix("objectTransform.modelMatrix",
                                   terrainTransform->m_finalTransform.m_animatedModelMatrix);
            shaderToUse->useVectorf("objectTransform.position", terrainTransform->m_finalTransform.m_position);

            const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(terrainEntity);
            // enable picking
            if(meshedEntityPickableComponent &&
               meshedEntityPickableComponent->isPickableForCamera(forCamera3DBaseInfo.getThisEntity()))
            {
                shaderToUse->useVectorf("u_pickingColor", terrainEntityBaseInfo.getUniqueColor());
            }
            else
            {
                shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
            }
        }

        shaderToUse->useInteger("u_verticesColorsAttributesCount", mesh.m_base.getMeshData()->m_verticesColors.size());
        // meshPPLayer is always valid
        shaderToUse->useInteger("SGPP_CurrentLayerIndex", terrainPPLayer->getIndex());
        shaderToUse->useFloat("u_terrainHeightScale", terrain.m_heightScale);
        shaderToUse->useVectorf("u_terrainUVScale", terrain.m_uvScale);

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

        CoreMain::getRenderer()->renderMeshData(
                mesh.m_base.getMeshData().get(),
                mesh.m_base.getMaterial()->m_meshRenderState
        );

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
                                                 const RenderingBase::reg_t& cameraRenderingBase,
                                                 const CSMTarget::reg_t* cameraCSMTarget,
                                                 const ECS::entity_t& forCamera,
                                                 LayeredFrameReceiver* cameraLayeredFrameReceiver,
                                                 const SGCore::Ref<SGCore::OctreeNode>& node) noexcept
{
    bool isVisibleForCamera = node->m_visibleReceivers.contains(forCamera);
    
    if(!isVisibleForCamera) return;

    if(cameraLayeredFrameReceiver)
    {
        cameraLayeredFrameReceiver->m_layersFrameBuffer->bind();

        cameraLayeredFrameReceiver->m_layersFrameBuffer->bindAttachmentsToDrawIn(
            cameraLayeredFrameReceiver->m_attachmentToRenderIn);
    }

    // render all entities
    for(const auto& e : node->m_overlappedEntities)
    {
        /*auto* tmpCullableInfo = registry->tryGet<OctreeCullable>(e);
        Ref<OctreeCullable> cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);*/

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
            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            renderMesh(registry, e, meshTransform, *mesh, *entityBaseInfo, forCamera3DBaseInfo,
                       cameraRenderingBase,
                       cameraCSMTarget,
                       meshPPLayer, false, cameraLayeredFrameReceiver);
        }
    }

    if(cameraLayeredFrameReceiver)
    {
        cameraLayeredFrameReceiver->m_layersFrameBuffer->unbind();
    }
}
