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
#include "SGCore/Render/SpacePartitioning/OctreeCullableInfo.h"
#include "SGCore/Render/SpacePartitioning/Octree.h"
#include "SGCore/Render/SpacePartitioning/ObjectsCullingOctree.h"

size_t renderedInOctrees = 0;

void SGCore::PBRRPGeometryPass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline)
{
    m_shader = MakeRef<IShader>();
    m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            parentRenderPipeline->m_shadersPaths.getByVirtualPath("StandardMeshShader").getCurrentRealization()));
}

void SGCore::PBRRPGeometryPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    Ref<PBRRPDirectionalLightsPass> dirLightsPass = renderPipeline->getRenderPass<PBRRPDirectionalLightsPass>();
    
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
    (const entity_t& cameraEntity, Ref<Camera3D>& camera3D, Ref<RenderingBase>& cameraRenderingBase, Ref<Transform>& cameraTransform) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(standardGeometryShader)
        {
            standardGeometryShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        // todo: make get receiver (postprocess or default) and render in them

        meshesView.each([&renderPipeline, &standardGeometryShader, &scene, &cameraEntity, &registry, this]
        (const entity_t& meshEntity, EntityBaseInfo& meshedEntityBaseInfo, Mesh& mesh, Ref<Transform>& meshTransform) {
            auto* tmpCullableMesh = registry.try_get<Ref<CullableMesh>>(meshEntity);
            Ref<CullableMesh> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);
            
            bool willRender = registry.try_get<IgnoreOctrees>(meshEntity) || !cullableMesh;
            
            if(willRender)
            {
                renderMesh(registry, meshEntity, meshTransform, mesh, standardGeometryShader);
            }
        });
    });
    
    // ==========================================
    // ============= rendering octrees ==========
    // ==========================================
    
    renderedInOctrees = 0;
    
    camerasView.each([&meshesView, &renderPipeline, &standardGeometryShader, &scene, &registry, this]
                             (const entity_t& cameraEntity, Ref<Camera3D>& camera3D, Ref<RenderingBase>& cameraRenderingBase, Ref<Transform>& cameraTransform) {
        
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        
        if(standardGeometryShader)
        {
            standardGeometryShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        }
        
        auto objectsCullingOctreesView = registry.view<Ref<Octree>, Ref<ObjectsCullingOctree>>();
        objectsCullingOctreesView.each([&standardGeometryShader, &scene, &cameraEntity, &registry, &cameraRenderingBase, this](Ref<Octree> octree, const Ref<ObjectsCullingOctree>&) {
            for(const auto& n : octree->m_notEmptyNodes)
            {
                renderOctreeNode(registry, cameraEntity, n, standardGeometryShader);
            }
        });
    });
    
    // std::cout << "renderedInOctrees: " << renderedInOctrees << std::endl;
}

void SGCore::PBRRPGeometryPass::renderMesh(entt::basic_registry<entity_t>& registry,
                                           const entity_t& meshEntity,
                                           const Ref<Transform>& meshTransform,
                                           Mesh& mesh,
                                           const Ref<ISubPassShader>& standardGeometryShader) noexcept
{
    ShaderComponent* entityShader = registry.try_get<ShaderComponent>(meshEntity);
    
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

void SGCore::PBRRPGeometryPass::renderOctreeNode(entt::basic_registry<entity_t>& registry,
                                                 const entity_t& forCamera,
                                                 const SGCore::Ref<SGCore::OctreeNode>& node,
                                                 const Ref<ISubPassShader>& standardGeometryShader) noexcept
{
    bool isVisibleForCamera = node->m_visibleReceivers.contains(forCamera);
    
    if(isVisibleForCamera)
    {
        // render all entities
        for(const auto& e : node->m_overlappedEntities)
        {
            auto* tmpCullableInfo = registry.try_get<Ref<OctreeCullableInfo>>(e);
            Ref<OctreeCullableInfo> cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
            
            if(!cullableInfo) continue;
            
            Mesh* mesh = registry.try_get<Mesh>(e);
            auto* tmpMeshTransform = registry.try_get<Ref<Transform>>(e);
            auto meshTransform = (tmpMeshTransform ? *tmpMeshTransform : nullptr);
            
            if(meshTransform && mesh)
            {
                ++renderedInOctrees;
                renderMesh(registry, e, meshTransform, *mesh, standardGeometryShader);
            }
        }
    }
}
