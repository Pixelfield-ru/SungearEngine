//
// Created by stuka on 24.12.2025.
//

#include "PBRRPInstancingPass.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Instancing/Instancing.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/RenderAbilities/EnableInstancingPass.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Utils/Assert.h"

void SGCore::PBRRPInstancingPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    IGeometryPass::create(parentRenderPipeline);

    const auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *parentRenderPipeline->m_shadersPaths["StandardMeshShader"]);

    m_shader = AssetManager::getInstance()->loadAssetWithAlias<IShader>("PBRRPGeometryInstancingShader", shaderFile->getPath());
    m_shader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_INSTANCED_RENDERING", ""));
    m_shader->recompile();
}

void SGCore::PBRRPInstancingPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    const auto registry = scene->getECSRegistry();

    auto instancingView = registry->view<EntityBaseInfo, Instancing, EnableInstancingPass>();

    iterateCameras(scene, [&](const CameraRenderingInfo& cameraRenderingInfo) {
        instancingView.each([&](const ECS::entity_t& instancingEntity,
                                EntityBaseInfo::reg_t& instancingEntityBaseInfo,
                                Instancing::reg_t& instancing, auto) {
            // todo: make layer choose
            Ref<PostProcessLayer> meshPPLayer = cameraRenderingInfo.m_cameraFrameReceiver->getDefaultLayer();

            // todo: make transparent pass
            const bool isTransparentPass = false;

            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            if(!instancing.getBaseMeshData() ||
               !instancing.getBaseMaterial()) return;

            const auto& meshGeomShader = instancing.getBaseMaterial()->m_shaders["GeometryInstancingPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;

            if(!shaderToUse) return;

            shaderToUse->bind();

            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

            const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(instancingEntity);
            // enable picking
            if(meshedEntityPickableComponent &&
               meshedEntityPickableComponent->isPickableForCamera(cameraRenderingInfo.m_cameraInfo->getThisEntity()))
            {
                shaderToUse->useVectorf("u_pickingColor", instancingEntityBaseInfo.getUniqueColor());
            }
            else
            {
                shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
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

            bindUniformBuffers(shaderToUse.get());

            shaderToUse->useInteger("u_isAnimatedMesh", 0);

            if(cameraRenderingInfo.m_cameraCSMTarget)
            {
                texUnitOffset = cameraRenderingInfo.m_cameraCSMTarget->bindUniformsToShader(
                    shaderToUse.get(), cameraRenderingInfo.m_cameraRenderingBase->m_zFar, texUnitOffset);
            }

            CoreMain::getRenderer()->renderArrayInstanced(
                    instancing.getVertexArray(),
                    instancing.getBaseMaterial()->m_meshRenderState,
                    instancing.getBaseMeshData()->m_vertices.size(),
                    instancing.getBaseMeshData()->m_indices.size(),
                    instancing.getActiveEntitiesCount()
            );
        });
    });
}
