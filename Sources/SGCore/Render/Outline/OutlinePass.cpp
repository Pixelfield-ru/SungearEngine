//
// Created by stuka on 17.12.2024.
//

#include "OutlinePass.h"

#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Utils/Paths.h"

void SGCore::OutlinePass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline) noexcept
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            Paths::getDefaultPaths()["Shaders/StandardOutline"]->resolved());

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    m_renderState.m_useFacesCulling = false;
    m_renderState.m_useDepthTest = false;
}

void SGCore::OutlinePass::render(const SGCore::Ref<SGCore::Scene>& scene,
                                 const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline) noexcept
{
    auto registry = scene->getECSRegistry();

    auto camerasView = registry->view<Ref<Camera3D>, EntityBaseInfo, Ref<RenderingBase>, Ref<Transform>, LayeredFrameReceiver>();
    auto meshesView = registry->view<EntityBaseInfo, Mesh, Ref<Transform>>();

    // IDK WHY BUT WE MUST FORCE RENDER STATE TO WORK OUTLINE CORRECTLY.
    m_renderState.use(true);

    m_shader->bind();

    auto shader = m_shader;
    auto& renderState = m_renderState;
    const auto& outlineThickness = m_outlineThickness;
    const auto& outlineColor = m_outlineColor;

    camerasView.each([&meshesView,
                      &shader,
                      &renderState,
                      &outlineThickness,
                      &outlineColor](const Ref<Camera3D>& camera3D,
                                     const EntityBaseInfo& cameraBaseInfo,
                                     const Ref<RenderingBase>& cameraRenderingBase,
                                     const Ref<Transform>& cameraTransform,
                                     const LayeredFrameReceiver& layeredFrameReceiver) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);

        layeredFrameReceiver.m_layersFXFrameBuffer->bind();
        layeredFrameReceiver.m_layersFXFrameBuffer->bindAttachmentToDrawIn(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7);

        meshesView.each([&shader,
                         &renderState,
                         &camera3D,
                         &outlineThickness,
                         &outlineColor](const entity_t& meshEntity,
                                          const EntityBaseInfo& meshBaseInfo,
                                          const Mesh& mesh,
                                          const Ref<Transform>& meshTransform) mutable {
            if(!camera3D->m_pickedEntities.contains(meshEntity)) return;

            shader->useMatrix("objectTransform.modelMatrix", meshTransform->m_finalTransform.m_modelMatrix);
            shader->useVectorf("objectTransform.position", meshTransform->m_finalTransform.m_position);

            shader->useFloat("u_outlineThickness", 0);
            shader->useVectorf("u_outlineColor", { 0, 0, 0, 0 });
            shader->useInteger("u_isFirstPass", 1);

            renderState.m_stencilFunc = SGStencilFunc::SGG_ALWAYS;
            renderState.m_stencilMask = 0xFF;

            CoreMain::getRenderer()->renderMeshData(
                    mesh.m_base.getMeshData().get(),
                    renderState
            );

            shader->useFloat("u_outlineThickness", outlineThickness);
            shader->useVectorf("u_outlineColor", outlineColor);
            shader->useInteger("u_isFirstPass", 0);

            renderState.m_stencilFunc = SGStencilFunc::SGG_NOTEQUAL;
            renderState.m_stencilMask = 0x00;

            CoreMain::getRenderer()->renderMeshData(
                    mesh.m_base.getMeshData().get(),
                    renderState
            );
        });

        layeredFrameReceiver.m_layersFXFrameBuffer->unbind();
    });
}
