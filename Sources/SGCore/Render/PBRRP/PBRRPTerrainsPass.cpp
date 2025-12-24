//
// Created by stuka on 25.12.2025.
//

#include "PBRRPTerrainsPass.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/Terrain/Terrain.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Utils/Assert.h"

void SGCore::PBRRPTerrainsPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    IGeometryPass::create(parentRenderPipeline);

    const auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *parentRenderPipeline->m_shadersPaths["BatchingShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    m_meshRenderState.m_useFacesCulling = false;
    m_meshRenderState.m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
    m_meshRenderState.m_drawMode = SGDrawMode::SGG_PATCHES;
}

void SGCore::PBRRPTerrainsPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    const auto registry = scene->getECSRegistry();

    auto terrainsView = registry->view<EntityBaseInfo, Mesh, Transform, Terrain>(ECS::ExcludeTypes<DisableMeshGeometryPass>{});

    m_meshRenderState.use();

    iterateCameras(scene, [&](const CameraRenderingInfo& cameraRenderingInfo) {
        terrainsView.each([&](const ECS::entity_t& terrainEntity,
                              EntityBaseInfo::reg_t& terrainEntityBaseInfo,
                              Mesh::reg_t& mesh,
                              Transform::reg_t& terrainTransform,
                              const Terrain::reg_t& terrain) {
            Ref<PostProcessLayer> meshPPLayer = mesh.m_base.m_layeredFrameReceiversMarkup[cameraRenderingInfo.m_cameraFrameReceiver].lock();

            if(!meshPPLayer)
            {
                meshPPLayer = cameraRenderingInfo.m_cameraFrameReceiver->getDefaultLayer();
            }

            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            if(!mesh.m_base.getMeshData() ||
               !mesh.m_base.getMaterial())
                return;

            const auto& meshGeomShader = mesh.m_base.getMaterial()->m_shaders["GeometryPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;

            if(!shaderToUse) return;

            shaderToUse->bind();

            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

            shaderToUse->useMatrix("objectTransform.modelMatrix",
                                   terrainTransform->m_finalTransform.m_animatedModelMatrix);
            shaderToUse->useVectorf("objectTransform.position", terrainTransform->m_finalTransform.m_position);

            const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(terrainEntity);
            // enable picking
            if(meshedEntityPickableComponent &&
               meshedEntityPickableComponent->isPickableForCamera(cameraRenderingInfo.m_cameraInfo->getThisEntity()))
            {
                shaderToUse->useVectorf("u_pickingColor", terrainEntityBaseInfo.getUniqueColor());
            }
            else
            {
                shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
            }

            shaderToUse->useInteger("u_verticesColorsAttributesCount", mesh.m_base.getMeshData()->m_verticesColors.size());
            // meshPPLayer is always valid
            shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());
            shaderToUse->useFloat("u_terrainHeightScale", terrain.m_heightScale);
                shaderToUse->useVectorf("u_terrainUVScale", terrain.m_uvScale);

            size_t offset0 = shaderToUse->bindMaterialTextures(mesh.m_base.getMaterial());
            shaderToUse->bindTextureBindings(offset0);
            shaderToUse->useMaterialFactors(mesh.m_base.getMaterial().get());

            bindUniformBuffers(shaderToUse.get());

            CoreMain::getRenderer()->renderMeshData(
                mesh.m_base.getMeshData().get(),
                mesh.m_base.getMaterial()->m_meshRenderState
            );
        });
    });
}
