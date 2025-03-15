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

void SGCore::UIRenderPass::create(const Ref<IRenderPipeline>& parentRenderPipeline) noexcept
{
    IRenderPass::create(parentRenderPipeline);
}

void SGCore::UIRenderPass::render(const Ref<Scene>& scene,
                                  const Ref<IRenderPipeline>& renderPipeline) noexcept
{
    auto uiComponentsView = scene->getECSRegistry()->view<UI::UIComponent>();
    auto camerasView = scene->getECSRegistry()->view<LayeredFrameReceiver, EntityBaseInfo, RenderingBase, Transform>();

    camerasView.each([&uiComponentsView, this](const LayeredFrameReceiver::reg_t& cameraReceiver,
                                               const EntityBaseInfo::reg_t& cameraInfo,
                                               const RenderingBase::reg_t& cameraRenderingBase,
                                               const Transform::reg_t& cameraTransform){
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);

        cameraReceiver.m_layersFrameBuffer->bind();

        cameraReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(cameraReceiver.m_attachmentToRenderIn);

        uiComponentsView.each([this, &cameraReceiver](UI::UIComponent::reg_t& uiComponent) {
            processUIElement(cameraReceiver, uiComponent, uiComponent.m_document->m_rootElement, 0);
        });

        cameraReceiver.m_layersFrameBuffer->unbind();
    });
}

void SGCore::UIRenderPass::processUIElement(const LayeredFrameReceiver::reg_t& cameraReceiver,
                                            UI::UIComponent& uiComponent,
                                            const Ref<UI::UIElement>& currentUIElement,
                                            const size_t& currentTransformNodeIdx) noexcept
{
    if(currentTransformNodeIdx >= uiComponent.m_transformTree.m_elements.size()) return;

    auto& currentTransformNode = uiComponent.m_transformTree.m_elements[currentTransformNodeIdx];
    auto& currentElementCache = currentTransformNode.m_currentElementCache;

    // =================================================================== rendering uielement

    const auto uiElementShader = currentUIElement->m_shader;

    if(uiElementShader)
    {
        const auto defaultPPLayer = cameraReceiver.getDefaultLayer();

        SG_ASSERT(defaultPPLayer != nullptr,
                          "Can not find default PP layer in layered frame receiver were found for UIElement mesh! Can not render this UIElement.");

        uiElementShader->bind();
        uiElementShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);

        uiElementShader->useMatrix("objectTransform.modelMatrix",
                                   currentTransformNode.m_transform.m_finalTransform.m_modelMatrix);
        uiElementShader->useVectorf("objectTransform.position",
                                    currentTransformNode.m_transform.m_finalTransform.m_position);

        uiElementShader->useInteger("SGPP_CurrentLayerIndex", defaultPPLayer->getIndex());

        currentUIElement->useUniforms(currentTransformNode.m_currentElementCache);

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