//
// Created by stuka on 22.12.2025.
//

#include "PBRRPTransparentMeshesPass.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Decals/Decal.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"
#include "SGCore/Utils/Assert.h"

void SGCore::PBRRPTransparentMeshesPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    IGeometryPass::create(parentRenderPipeline);

    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *parentRenderPipeline->m_shadersPaths["StandardMeshShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    m_renderState.m_globalBlendingState.m_useBlending = true;
}

void SGCore::PBRRPTransparentMeshesPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto registry = scene->getECSRegistry();

    auto transparentMeshesView = registry->view<EntityBaseInfo, Mesh, Transform, TransparentEntityTag>(ECS::ExcludeTypes<DisableMeshGeometryPass, Decal>{});

    m_renderState.use();

    iterateCameras(scene, [&](const CameraRenderingInfo& cameraRenderingInfo) {
        transparentMeshesView.each([&](const ECS::entity_t& meshEntity,
                                       EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                                       Mesh::reg_t& mesh, Transform::reg_t& meshTransform,
                                       const auto&) {
            const bool willRender = cameraRenderingInfo.m_camera3D->isEntityVisibleForCamera(registry, cameraRenderingInfo.m_cameraEntity, meshEntity);

            if(!willRender) return;

            Ref<PostProcessLayer> meshPPLayer = mesh.m_base.m_layeredFrameReceiversMarkup[cameraRenderingInfo.m_cameraFrameReceiver].lock();

            if(!meshPPLayer)
            {
                meshPPLayer = cameraRenderingInfo.m_cameraFrameReceiver->getDefaultLayer();
            }

            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            // ============================== mesh rendering

            if(!mesh.m_base.getMeshData() || !mesh.m_base.getMaterial()) return;

            const auto& meshGeomShader = mesh.m_base.getMaterial()->m_shaders["GeometryPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;

            if(!shaderToUse) return;

            shaderToUse->bind();

            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

            shaderToUse->useMatrix("objectTransform.modelMatrix",
                                   meshTransform->m_finalTransform.m_animatedModelMatrix);

            shaderToUse->useVectorf("objectTransform.position", meshTransform->m_finalTransform.m_position);

            const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(meshEntity);
            // enable picking
            if(meshedEntityPickableComponent &&
               meshedEntityPickableComponent->isPickableForCamera(cameraRenderingInfo.m_cameraInfo->getThisEntity()))
            {
                shaderToUse->useVectorf("u_pickingColor", meshedEntityBaseInfo.getUniqueColor());
            }
            else
            {
                shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
            }

            shaderToUse->useInteger("u_isTransparentPass", true);
            shaderToUse->useInteger("u_verticesColorsAttributesCount", mesh.m_base.getMeshData()->m_verticesColors.size());
            // meshPPLayer is always valid
            shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());

            // 14 MS FOR loc0 IN DEBUG
            size_t texUnitOffset = shaderToUse->bindMaterialTextures(mesh.m_base.getMaterial());
            shaderToUse->bindTextureBindings(texUnitOffset);
            shaderToUse->useMaterialFactors(mesh.m_base.getMaterial().get());

            bindUniformBuffers(shaderToUse.get());

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

            if(cameraRenderingInfo.m_cameraCSMTarget)
            {
                texUnitOffset = cameraRenderingInfo.m_cameraCSMTarget->bindUniformsToShader(
                    shaderToUse.get(), cameraRenderingInfo.m_cameraRenderingBase->m_zFar, texUnitOffset);
            }

            const bool lastUseFacesCulling = mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling;
            mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling = false;

            CoreMain::getRenderer()->renderMeshData(
                mesh.m_base.getMeshData().get(),
                mesh.m_base.getMaterial()->m_meshRenderState
            );

            mesh.m_base.getMaterial()->m_meshRenderState.m_useFacesCulling = lastUseFacesCulling;
        });
    });
}
