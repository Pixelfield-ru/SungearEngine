//
// Created by stuka on 01.05.2025.
//

#include "PBRRPDecalsPass.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/Decals/Decal.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Instancing/Instancing.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"
#include "SGCore/Utils/Assert.h"

void SGCore::PBRRPDecalsPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *parentRenderPipeline->m_shadersPaths["StandardDecalShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    m_instancingShader = AssetManager::getInstance()->loadAssetWithAlias<IShader>("PBRRPDecalsInstancingShader", shaderFile->getPath());
    m_instancingShader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_INSTANCED_RENDERING", ""));
    m_instancingShader->recompile();
}

void SGCore::PBRRPDecalsPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto registry = scene->getECSRegistry();

    auto camerasView = registry->view<EntityBaseInfo, LayeredFrameReceiver, RenderingBase, Transform>();
    auto decalsView = registry->view<EntityBaseInfo, Transform, Decal, Mesh>(ECS::ExcludeTypes<DisableMeshGeometryPass>{});
    auto instancedDecalsView = registry->view<EntityBaseInfo, Decal, Instancing>(ECS::ExcludeTypes<DisableMeshGeometryPass>{});

    m_meshRenderState.use();
    m_renderState.use();

    camerasView.each([&decalsView, &registry, &instancedDecalsView, this](const ECS::entity_t& cameraEntity,
                                                                          const EntityBaseInfo::reg_t& camera3DBaseInfo,
                                                                          LayeredFrameReceiver&
                                                                          cameraLayeredFrameReceiver,
                                                                          RenderingBase::reg_t& cameraRenderingBase,
                                                                          Transform::reg_t& cameraTransform) {

        const auto* cameraCSMTarget = registry->tryGet<CSMTarget>(cameraEntity);

        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);

        if(m_shader)
        {
            m_shader->bind();

            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            m_shader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
        }

        cameraLayeredFrameReceiver.m_layersFrameBuffer->bind();
        cameraLayeredFrameReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(
                cameraLayeredFrameReceiver.m_attachmentToRenderIn
        );
        cameraLayeredFrameReceiver.m_layersFrameBuffer->bindAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4, 0);
        cameraLayeredFrameReceiver.m_layersFrameBuffer->bindAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT5, 1);

        decalsView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, &cameraCSMTarget, &cameraRenderingBase, this](
        const ECS::entity_t& decalEntity,
        const EntityBaseInfo::reg_t& decalInfo,
        const Transform::reg_t& decalTransform,
        const Decal::reg_t& decal,
        Mesh::reg_t& decalMesh) {

            Ref<PostProcessLayer> meshPPLayer =
                decalMesh.m_base.m_layeredFrameReceiversMarkup[&cameraLayeredFrameReceiver].lock();

            if(!meshPPLayer)
            {
                meshPPLayer = cameraLayeredFrameReceiver.getDefaultLayer();
            }

            SG_ASSERT(meshPPLayer != nullptr,
                          "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            if(!decalMesh.m_base.getMeshData() ||
               !decalMesh.m_base.getMaterial()) return;

            const auto& meshGeomShader = decalMesh.m_base.getMaterial()->m_shaders["DecalsPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_shader;

            if(!shaderToUse) return;

            // binding shaderToUse only if it is custom shader
            if(shaderToUse == meshGeomShader)
            {
                shaderToUse->bind();
                shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
                shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
            }

            {
                shaderToUse->useMatrix("objectTransform.modelMatrix",
                                       decalTransform->m_finalTransform.m_animatedModelMatrix);
                shaderToUse->useVectorf("objectTransform.position", decalTransform->m_finalTransform.m_position);

                const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(decalEntity);
                // enable picking
                if(meshedEntityPickableComponent &&
                   meshedEntityPickableComponent->isPickableForCamera(camera3DBaseInfo.getThisEntity()))
                {
                    shaderToUse->useVectorf("u_pickingColor", decalInfo.getUniqueColor());
                }
                else
                {
                    shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
                }
            }

            shaderToUse->useInteger("u_isTransparentPass", 0);
            shaderToUse->useInteger("u_verticesColorsAttributesCount", decalMesh.m_base.getMeshData()->m_verticesColors.size());

            if(meshPPLayer)
            {
                shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());
            }

            // 2 because we bound SGG_COLOR_ATTACHMENT4 (gbuffer world pos) and SGG_COLOR_ATTACHMENT5 (gbuffer normal) to 0 and 1 location
            shaderToUse->useTextureBlock("u_GBufferWorldPos", 0);
            shaderToUse->useTextureBlock("u_GBufferNormal", 1);
            size_t texUnitOffset = shaderToUse->bindMaterialTextures(decalMesh.m_base.getMaterial(), 2);
            texUnitOffset = shaderToUse->bindTextureBindings(texUnitOffset);
            shaderToUse->useMaterialFactors(decalMesh.m_base.getMaterial().get());
            if(cameraCSMTarget)
            {
                texUnitOffset = cameraCSMTarget->bindUniformsToShader(shaderToUse.get(), cameraRenderingBase->m_zFar, texUnitOffset);
            }

            bindUniformBuffers(shaderToUse.get());

            CoreMain::getRenderer()->renderMeshData(
                    decalMesh.m_base.getMeshData().get(),
                    decalMesh.m_base.getMaterial()->m_meshRenderState
            );

            // if we used custom shader then we must bind standard pipeline shader
            if(shaderToUse == meshGeomShader)
            {
                if(m_shader)
                {
                    m_shader->bind();
                }
            }
        });

        // =======================================================================================
        // =======================================================================================
        // =======================================================================================

        if(m_instancingShader)
        {
            m_instancingShader->bind();

            m_instancingShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
            m_instancingShader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
        }

        instancedDecalsView.each([&cameraLayeredFrameReceiver, &registry, &camera3DBaseInfo, &cameraCSMTarget, &cameraRenderingBase, this](
        const ECS::entity_t& decalEntity,
        const EntityBaseInfo::reg_t& decalInfo,
        const Decal::reg_t& decal,
        Instancing::reg_t& instancing) {

            Ref<PostProcessLayer> meshPPLayer = cameraLayeredFrameReceiver.getDefaultLayer();

            SG_ASSERT(meshPPLayer != nullptr,
                          "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            if(!instancing.getBaseMeshData() ||
               !instancing.getBaseMaterial()) return;

            const auto& meshGeomShader = instancing.getBaseMaterial()->m_shaders["DecalsPass"];
            const auto& shaderToUse = meshGeomShader ? meshGeomShader : m_instancingShader;

            if(!shaderToUse) return;

            // binding shaderToUse only if it is custom shader
            if(shaderToUse == meshGeomShader)
            {
                shaderToUse->bind();
                shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
                shaderToUse->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);
            }

            {
                const auto* meshedEntityPickableComponent = registry->tryGet<Pickable>(decalEntity);
                // enable picking
                if(meshedEntityPickableComponent &&
                   meshedEntityPickableComponent->isPickableForCamera(camera3DBaseInfo.getThisEntity()))
                {
                    shaderToUse->useVectorf("u_pickingColor", decalInfo.getUniqueColor());
                }
                else
                {
                    shaderToUse->useVectorf("u_pickingColor", { 0, 0, 0 });
                }
            }

            shaderToUse->useInteger("u_isTransparentPass", 0);
            shaderToUse->useInteger("u_verticesColorsAttributesCount", instancing.getBaseMeshData()->m_verticesColors.size());

            if(meshPPLayer)
            {
                shaderToUse->useInteger("SGPP_CurrentLayerIndex", meshPPLayer->getIndex());
            }

            // 2 because we bound SGG_COLOR_ATTACHMENT4 (gbuffer world pos) and SGG_COLOR_ATTACHMENT5 (gbuffer normal) to 0 and 1 location
            shaderToUse->useTextureBlock("u_GBufferWorldPos", 0);
            shaderToUse->useTextureBlock("u_GBufferNormal", 1);
            size_t texUnitOffset = shaderToUse->bindMaterialTextures(instancing.getBaseMaterial(), 2);
            texUnitOffset = shaderToUse->bindTextureBindings(texUnitOffset);
            shaderToUse->useMaterialFactors(instancing.getBaseMaterial().get());
            if(cameraCSMTarget)
            {
                texUnitOffset = cameraCSMTarget->bindUniformsToShader(shaderToUse.get(), cameraRenderingBase->m_zFar, texUnitOffset);
            }

            bindUniformBuffers(shaderToUse.get());

            CoreMain::getRenderer()->renderArrayInstanced(
                instancing.getVertexArray(),
                instancing.getBaseMaterial()->m_meshRenderState,
                instancing.getBaseMeshData()->m_vertices.size(),
                instancing.getBaseMeshData()->m_indices.size(),
                instancing.getActiveEntitiesCount()
            );

            // if we used custom shader then we must bind standard pipeline shader
            if(shaderToUse == meshGeomShader)
            {
                if(m_instancingShader)
                {
                    m_instancingShader->bind();
                }
            }
        });

        cameraLayeredFrameReceiver.m_layersFrameBuffer->unbind();
    });
}
