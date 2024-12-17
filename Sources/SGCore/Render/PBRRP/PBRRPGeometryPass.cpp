//
// Created by stuka on 25.11.2023.
//

#include <entt/entt.hpp>

#include "PBRRPGeometryPass.h"
#include "SGCore/Render/Picking/Pickable.h"
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
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/SpacePartitioning/OctreeCullable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Render/SpacePartitioning/Octree.h"
#include "SGCore/Render/SpacePartitioning/ObjectsCullingOctree.h"

size_t renderedInOctrees = 0;

void SGCore::PBRRPGeometryPass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline)
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            parentRenderPipeline->m_shadersPaths["StandardMeshShader"]->resolved());

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    // configuring default material to use standard pbr shader
    auto defaultMaterial = AssetManager::getInstance()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
    defaultMaterial->m_shader = m_shader;
}

void SGCore::PBRRPGeometryPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    Ref<PBRRPDirectionalLightsPass> dirLightsPass = renderPipeline->getRenderPass<PBRRPDirectionalLightsPass>();
    
    // scene->getECSRegistry();
    auto registry = scene->getECSRegistry();
    
    auto camerasView = registry->view<Ref<Camera3D>, Ref<RenderingBase>, Ref<Transform>>();
    auto meshesView = registry->view<EntityBaseInfo, Mesh, Ref<Transform>>(entt::exclude<DisableMeshGeometryPass>);
    
    if(m_shader)
    {
        m_shader->bind();
    }
    
    camerasView.each([&meshesView, &renderPipeline, &scene, &registry, this]
    (const entity_t& cameraEntity, Ref<Camera3D>& camera3D, Ref<RenderingBase>& cameraRenderingBase, Ref<Transform>& cameraTransform) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(m_shader)
        {
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        LayeredFrameReceiver* cameraLayeredFrameReceiver = registry->try_get<LayeredFrameReceiver>(cameraEntity);
        if(cameraLayeredFrameReceiver) cameraLayeredFrameReceiver->clearPostProcessFrameBuffers();
        
        // todo: make get receiver (postprocess or default) and render in them

        meshesView.each([&cameraLayeredFrameReceiver, &registry, this]
        (const entity_t& meshEntity, EntityBaseInfo& meshedEntityBaseInfo, Mesh& mesh, Ref<Transform>& meshTransform) {
            auto* tmpCullableMesh = registry->try_get<Ref<OctreeCullable>>(meshEntity);
            Ref<OctreeCullable> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);
            
            bool willRender = registry->try_get<IgnoreOctrees>(meshEntity) || !cullableMesh;
            
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

                if(cameraLayeredFrameReceiver)
                {
                    SG_ASSERT(meshPPLayer != nullptr,
                              "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

                    cameraLayeredFrameReceiver->m_layersFrameBuffer->bind();
                    cameraLayeredFrameReceiver->m_layersFrameBuffer->bindAttachmentsToDrawIn(
                            cameraLayeredFrameReceiver->m_attachmentToRenderIn
                    );
                }

                renderMesh(registry, meshEntity, meshTransform, mesh, meshedEntityBaseInfo, meshPPLayer);

                if(cameraLayeredFrameReceiver)
                {
                    cameraLayeredFrameReceiver->m_layersFrameBuffer->unbind();
                }
            }
        });
    });
    
    // ==========================================
    // ============= rendering octrees ==========
    // ==========================================
    
    renderedInOctrees = 0;
    
    camerasView.each([&meshesView, &renderPipeline, &scene, &registry, this]
                             (const entity_t& cameraEntity, Ref<Camera3D>& camera3D, Ref<RenderingBase>& cameraRenderingBase, Ref<Transform>& cameraTransform) {
        
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(m_shader)
        {
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        LayeredFrameReceiver* cameraLayeredFrameReceiver = registry->try_get<LayeredFrameReceiver>(cameraEntity);
        
        auto objectsCullingOctreesView = registry->view<Ref<Octree>, Ref<ObjectsCullingOctree>>();
        objectsCullingOctreesView.each([&cameraLayeredFrameReceiver, &scene, &cameraEntity, &registry, &cameraRenderingBase, this]
        (Ref<Octree> octree, const Ref<ObjectsCullingOctree>&) {
            for(const auto& n : octree->m_notEmptyNodes)
            {
                renderOctreeNode(registry, cameraEntity, cameraLayeredFrameReceiver, n);
            }
        });
    });
    
    // std::cout << "renderedInOctrees: " << renderedInOctrees << std::endl;
}

void SGCore::PBRRPGeometryPass::renderMesh(const Ref<registry_t>& registry,
                                           const entity_t& meshEntity,
                                           const Ref<Transform>& meshTransform,
                                           Mesh& mesh,
                                           EntityBaseInfo& meshedEntityBaseInfo,
                                           const Ref<PostProcessLayer>& meshPPLayer) noexcept
{
    /*if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial() ||
       !mesh.m_base.getMaterial()->m_shader) return;*/

    if(!mesh.m_base.getMeshData() ||
       !mesh.m_base.getMaterial()) return;

    auto meshGeomShader = mesh.m_base.getMaterial()->m_shader;
    auto shaderToUse = meshGeomShader ? meshGeomShader : m_shader;
    
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
                                   meshTransform->m_finalTransform.m_modelMatrix);
            shaderToUse->useVectorf("objectTransform.position", meshTransform->m_finalTransform.m_position);

            if(registry->any_of<Pickable>(meshEntity))
            {
                shaderToUse->useVectorf("u_pickingColor", meshedEntityBaseInfo.getUniqueColor());
            }
        }

        if(meshPPLayer)
        {
            shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());
        }
        
        size_t offset0 = shaderToUse->bindMaterialTextures(mesh.m_base.getMaterial());
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
                mesh.m_base.getMeshData().get(),
                mesh.m_base.m_meshDataRenderInfo
        );
        
        shaderToUse->unbindMaterialTextures(mesh.m_base.getMaterial());

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

void SGCore::PBRRPGeometryPass::renderOctreeNode(const Ref<registry_t>& registry,
                                                 const entity_t& forCamera,
                                                 LayeredFrameReceiver* cameraLayeredFrameReceiver,
                                                 const SGCore::Ref<SGCore::OctreeNode>& node) noexcept
{
    bool isVisibleForCamera = node->m_visibleReceivers.contains(forCamera);
    
    if(isVisibleForCamera)
    {
        // render all entities
        for(const auto& e : node->m_overlappedEntities)
        {
            auto* tmpCullableInfo = registry->try_get<Ref<OctreeCullable>>(e);
            Ref<OctreeCullable> cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
            
            if(!cullableInfo) continue;
            
            Mesh* mesh = registry->try_get<Mesh>(e);
            auto* tmpMeshTransform = registry->try_get<Ref<Transform>>(e);
            auto meshTransform = (tmpMeshTransform ? *tmpMeshTransform : nullptr);
            
            if(meshTransform && mesh)
            {
                ++renderedInOctrees;
                
                EntityBaseInfo* entityBaseInfo = registry->try_get<EntityBaseInfo>(e);
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
                
                renderMesh(registry, e, meshTransform, *mesh, *entityBaseInfo, meshPPLayer);

                if(cameraLayeredFrameReceiver)
                {
                    cameraLayeredFrameReceiver->m_layersFrameBuffer->unbind();
                }
            }
        }
    }
}
