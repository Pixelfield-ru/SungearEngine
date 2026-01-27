//
// Created by stuka on 26.01.2026.
//

#include "PBRRPVolumetricPass.h"

#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/RenderAbilities/EnableVolumetricPass.h"
#include "SGCore/Render/Volumetric/VolumetricFog.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Utils/Assert.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"

void SGCore::PBRRPVolumetricPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    IGeometryPass::create(parentRenderPipeline);

    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *parentRenderPipeline->m_shadersPaths["VolumetricShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());
}

void SGCore::PBRRPVolumetricPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto registry = scene->getECSRegistry();

    auto volumetrics = registry->view<EntityBaseInfo, Transform, VolumetricFog, EnableVolumetricPass>();

    iterateCameras(scene, [&](const CameraRenderingInfo& cameraRenderingInfo) {
        volumetrics.each([&](ECS::entity_t volumetricEntity,
                             const EntityBaseInfo& baseInfo,
                             const Transform::reg_t& transform,
                             VolumetricFog& clouds, auto) {
            const bool willRender = cameraRenderingInfo.m_camera3D->isEntityVisibleForCamera(registry, cameraRenderingInfo.m_cameraEntity, volumetricEntity);

            if(!willRender) return;

            Ref<PostProcessLayer> meshPPLayer = clouds.m_meshBase.m_layeredFrameReceiversMarkup[cameraRenderingInfo.m_cameraFrameReceiver].lock();

            if(!meshPPLayer)
            {
                meshPPLayer = cameraRenderingInfo.m_cameraFrameReceiver->getDefaultLayer();
            }

            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            // ============================== mesh rendering

            if(!clouds.m_meshBase.getMeshData() || !clouds.m_meshBase.getMaterial()) return;

            const auto& meshGeomShader = clouds.m_meshBase.getMaterial()->m_shaders["VolumetricsPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;

            if(!shaderToUse) return;

            shaderToUse->bind();

            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

            shaderToUse->useFloat("u_sgVolumetricCoverage", clouds.m_coverage);
            shaderToUse->useFloat("u_sgVolumetricDensity", clouds.m_density);
            shaderToUse->useVectorf("u_sgMeshAABBMin", transform->m_finalTransform.m_aabb.m_min);
            shaderToUse->useVectorf("u_sgMeshAABBMax", transform->m_finalTransform.m_aabb.m_max);

            shaderToUse->useMatrix("objectTransform.modelMatrix",
                                   transform->m_finalTransform.m_animatedModelMatrix);

            shaderToUse->useVectorf("objectTransform.position", transform->m_finalTransform.m_position);

            const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(volumetricEntity);
            // enable picking
            if(meshedEntityPickableComponent &&
               meshedEntityPickableComponent->isPickableForCamera(cameraRenderingInfo.m_cameraInfo->getThisEntity()))
            {
                shaderToUse->useVectorf("u_pickingColor", baseInfo.getUniqueColor());
            }
            else
            {
                shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
            }

            // meshPPLayer is always valid
            shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());

            // 14 MS FOR loc0 IN DEBUG
            size_t texUnitOffset = shaderToUse->bindMaterialTextures(clouds.m_meshBase.getMaterial());
            shaderToUse->bindTextureBindings(texUnitOffset);
            shaderToUse->useMaterialFactors(clouds.m_meshBase.getMaterial().get());

            bindUniformBuffers(shaderToUse.get());

            if(cameraRenderingInfo.m_cameraCSMTarget)
            {
                texUnitOffset = cameraRenderingInfo.m_cameraCSMTarget->bindUniformsToShader(shaderToUse.get(), cameraRenderingInfo.m_cameraRenderingBase->m_zFar, texUnitOffset);
            }

            CoreMain::getRenderer()->renderMeshData(
                clouds.m_meshBase.getMeshData().get(),
                clouds.m_meshBase.getMaterial()->m_meshRenderState
            );
        });
    });
}
