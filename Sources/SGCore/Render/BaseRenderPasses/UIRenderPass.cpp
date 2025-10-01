//
// Created by stuka on 28.01.2025.
//

#include "UIRenderPass.h"

#include "SGCore/Scene/Scene.h"

#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Transformations/TransformUtils.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/UI/UIElementMesh/UIElementMesh.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/UI/FontSpecializationRenderer.h"

void SGCore::UIRenderPass::create(const Ref<IRenderPipeline>& parentRenderPipeline) noexcept
{
    IRenderPass::create(parentRenderPipeline);
}

void SGCore::UIRenderPass::render(const Scene* scene,
                                  const Ref<IRenderPipeline>& renderPipeline) noexcept
{
    auto uiComponentsView = scene->getECSRegistry()->view<UI::UIComponent>();
    auto camerasView = scene->getECSRegistry()->view<RenderingBase, Transform>();

    // preparing cameras buffers
    camerasView.each([](const RenderingBase::reg_t& cameraRenderingBase,
                        const Transform::reg_t& cameraTransform) {
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);
    });

    uiComponentsView.each([this, scene](UI::UIComponent::reg_t& uiComponent) {
        if(!uiComponent.m_attachedToCamera.m_referencedEntity) return;

        auto* cameraReceiver = scene->getECSRegistry()->tryGet<LayeredFrameReceiver>(*uiComponent.m_attachedToCamera);

        if(!cameraReceiver) return;

        cameraReceiver->m_layersFrameBuffer->bind();

        cameraReceiver->m_layersFrameBuffer->bindAttachmentsToDrawIn(cameraReceiver->m_attachmentToRenderIn);

        processUIElement(*cameraReceiver, uiComponent, uiComponent.m_document->m_rootElement, 0);

        cameraReceiver->m_layersFrameBuffer->unbind();
    });
}

void SGCore::UIRenderPass::processUIElement(const LayeredFrameReceiver::reg_t& cameraReceiver,
                                            UI::UIComponent& uiComponent,
                                            const Ref<UI::UIElement>& currentUIElement,
                                            const size_t& currentTransformNodeIdx) noexcept
{
    if(currentTransformNodeIdx >= uiComponent.m_transformTree.m_elements.size()) return;

    auto& currentTransformNode = uiComponent.m_transformTree.m_elements[currentTransformNodeIdx];
    auto& currentElementCache = currentTransformNode.m_elementCurrentCache;

    // =================================================================== rendering uielement

    const auto uiElementShader = currentUIElement->m_shader;

    const bool isElementCustomRendered = currentUIElement->draw(cameraReceiver, currentTransformNode.m_transform, currentElementCache);

    if(!isElementCustomRendered && uiElementShader && currentUIElement->m_meshData)
    {
        const auto defaultPPLayer = cameraReceiver.getDefaultLayer();

        SG_ASSERT(defaultPPLayer != nullptr,
                          "Can not find default PP layer in layered frame receiver were found for UIElement mesh! Can not render this UIElement.");

        uiElementShader->bind();
        uiElementShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);

        uiElementShader->useMatrix("objectTransform.modelMatrix",
                                   currentTransformNode.m_transform.m_finalTransform.m_animatedModelMatrix);
        uiElementShader->useVectorf("objectTransform.position",
                                    currentTransformNode.m_transform.m_finalTransform.m_position);

        uiElementShader->useInteger("SGPP_CurrentLayerIndex", defaultPPLayer->getIndex());

        uiElementShader->useVectorf("u_pickingColor", (glm::vec3) currentUIElement->m_uniqueColor.color());

        currentUIElement->useUniforms(currentTransformNode.m_elementCurrentCache);

        CoreMain::getRenderer()->renderArray(
                currentUIElement->m_meshData->getVertexArray(),
                m_meshRenderState,
                currentUIElement->m_meshData->m_vertices.size(),
                currentUIElement->m_meshData->m_indices.size()
        );
    }

    // ===================================================================

    for(size_t i = 0; i < currentUIElement->m_children.size(); ++i)
    {
        // if count of children in transform node != in ui element then clearing UIComponent transform tree
        // to fill it again in UILayoutCalculator.
        // it can be possible if uidocument was reloaded
        if(currentUIElement->m_children.size() != currentTransformNode.m_children.size())
        {
            uiComponent.m_transformTree = { };
            return;
        }

        const auto& child = currentUIElement->m_children[i];

        processUIElement(cameraReceiver, uiComponent, child, currentTransformNode.m_children[i]);
    }
}