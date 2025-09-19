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
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Utils/Paths.h"

void SGCore::OutlinePass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline) noexcept
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *Paths::getDefaultPaths()["Shaders/StandardOutline"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());

    m_meshRenderState.m_useFacesCulling = false;
    m_renderState.m_useDepthTest = false;

    m_postProcessQuad = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(2);
    m_postProcessQuad->m_indices.push_back(1);

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(3);
    m_postProcessQuad->m_indices.push_back(2);

    // 4 vertices

    m_postProcessQuad->m_vertices.push_back({ .m_position = { -1, -1, 0 }});

    m_postProcessQuad->m_vertices.push_back({ .m_position = { -1, 1, 0 }});

    m_postProcessQuad->m_vertices.push_back({ .m_position = { 1, 1, 0 }});

    m_postProcessQuad->m_vertices.push_back({ .m_position = { 1, -1, 0 }});

    m_postProcessQuad->prepare();
}

void SGCore::OutlinePass::render(const Scene* scene,
                                 const Ref<IRenderPipeline>& renderPipeline) noexcept
{
    auto registry = scene->getECSRegistry();

    auto camerasView = registry->view<Camera3D,
            EntityBaseInfo,
            RenderingBase,
            Transform,
            LayeredFrameReceiver>();
    auto meshesView = registry->view<EntityBaseInfo, Mesh, Transform>();

    // IDK WHY BUT WE MUST FORCE RENDER STATE TO WORK OUTLINE CORRECTLY.
    m_renderState.use();

    m_shader->bind();
    m_shader->useFloat("u_outlineThickness", m_outlineThickness);

    camerasView.each([&meshesView,
                      this](const Camera3D::reg_t& camera3D,
                            const EntityBaseInfo::reg_t& cameraBaseInfo,
                            const RenderingBase::reg_t& cameraRenderingBase,
                            const Transform::reg_t& cameraTransform,
                            const LayeredFrameReceiver::reg_t& layeredFrameReceiver) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
        m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);

        layeredFrameReceiver.m_layersFXFrameBuffer->bind();
        // layeredFrameReceiver.m_layersFXFrameBuffer->useStates();
        layeredFrameReceiver.m_layersFXFrameBuffer->bindAttachmentsToDrawIn(
                std::vector { SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                              SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1 });
        layeredFrameReceiver.m_layersFXFrameBuffer->clearAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
        layeredFrameReceiver.m_layersFXFrameBuffer->clearAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1);

        m_shader->useInteger("u_pass", 1);

        // firstly rendering outlined object to other frame buffer attachment
        // color of outlined objects = u_outlineColor
        meshesView.each([&camera3D,
                         this](const ECS::entity_t& meshEntity,
                               const EntityBaseInfo::reg_t& meshBaseInfo,
                               const Mesh::reg_t& mesh,
                               const Transform::reg_t& meshTransform) mutable {
            if(!camera3D->m_pickedEntities.contains(meshEntity)) return;

            m_shader->useVectorf("u_outlineColor", meshBaseInfo.m_outlineColor);
            m_shader->useMatrix("objectTransform.modelMatrix", meshTransform->m_finalTransform.m_animatedModelMatrix);
            m_shader->useVectorf("objectTransform.position", meshTransform->m_finalTransform.m_position);

            if(auto bonesLockedBuffer = mesh.m_base.m_bonesBuffer.lock())
            {
                bonesLockedBuffer->bind(0);
                m_shader->useTextureBlock("u_bonesMatricesUniformBuffer", 0);

                m_shader->useInteger("u_isAnimatedMesh", 1);
            }
            else
            {
                m_shader->useInteger("u_isAnimatedMesh", 0);
            }

            CoreMain::getRenderer()->renderMeshData(
                    mesh.m_base.getMeshData().get(),
                    m_meshRenderState
            );
        });

        // then scaling up attachment0 (contains outlining objects)

        if(!camera3D->m_pickedEntities.empty())
        {
            // rendering outline
            m_shader->useInteger("u_pass", 2);

            m_shader->useTextureBlock("u_outlineBuffer", 0);

            layeredFrameReceiver.m_layersFXFrameBuffer->bindAttachment(
                    SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, 0
            );

            CoreMain::getRenderer()->renderMeshData(
                    m_postProcessQuad.get(),
                    m_meshRenderState
            );
        }

        // committing changes to framebuffer
        layeredFrameReceiver.m_layersFXFrameBuffer->unbind();

        // binding framebuffer again and binding its color attachment 7 to write in it
        layeredFrameReceiver.m_layersFXFrameBuffer->bind();
        layeredFrameReceiver.m_layersFXFrameBuffer->bindAttachmentToDrawIn(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7);

        // then combining outline buffer with attachment7 that contains final scene color
        m_shader->useInteger("u_pass", 3);

        m_shader->useTextureBlock("u_scaledOutlineBuffer", 1);

        // using attachment that contains scaled outline
        layeredFrameReceiver.m_layersFXFrameBuffer->bindAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, 1
        );

        CoreMain::getRenderer()->renderMeshData(
                m_postProcessQuad.get(),
                m_meshRenderState
        );

        layeredFrameReceiver.m_layersFXFrameBuffer->unbind();
    });
}
