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

    m_currentProceedUIElements = 0;
    //size_t current

    camerasView.each([&uiComponentsView, this](const LayeredFrameReceiver::reg_t& cameraReceiver,
                                               const EntityBaseInfo::reg_t& cameraInfo,
                                               const RenderingBase::reg_t& cameraRenderingBase,
                                               const Transform::reg_t& cameraTransform){
        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);

        cameraReceiver.m_layersFrameBuffer->bind();

        cameraReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(cameraReceiver.m_attachmentToRenderIn);

        uiComponentsView.each([this, &cameraReceiver](UI::UIComponent::reg_t& uiComponent) {
            processUIElement(cameraReceiver, -1, uiComponent, uiComponent.m_document->m_rootElement);
        });

        cameraReceiver.m_layersFrameBuffer->unbind();
    });
}

std::int64_t SGCore::UIRenderPass::processUIElement(const LayeredFrameReceiver::reg_t& cameraReceiver,
                                                    const std::int64_t& parentUITreeNodeIdx,
                                                    UI::UIComponent& uiComponent,
                                                    const Ref<UI::UIElement>& currentUIElement) noexcept
{
    m_currentProceedUIElements++;

    const std::int64_t currentUITransformNodeIdx = m_currentProceedUIElements - 1;

    bool isCreatedNewTransformNode = false;

    // if count of UI nodes in document > count of nodes in transform tree
    if(m_currentProceedUIElements > uiComponent.m_transformTree.m_elements.size())
    {
        // creating new transform node
        uiComponent.m_transformTree.m_elements.emplace_back();

        uiComponent.m_transformTree.m_elements[currentUITransformNodeIdx].m_parent = parentUITreeNodeIdx;

        isCreatedNewTransformNode = true;
    }

    auto& currentTransformNode = uiComponent.m_transformTree.m_elements[currentUITransformNodeIdx];

    UI::UITransformTreeElement* parentTransformNode { };
    if(parentUITreeNodeIdx != -1)
    {
        parentTransformNode = &uiComponent.m_transformTree.m_elements[parentUITreeNodeIdx];
    }

    const Transform* parentTransform = parentTransformNode ? &parentTransformNode->m_transform : nullptr;
    const UI::CSSSelectorCache* parentCSSSelectorCache = parentTransformNode ? &parentTransformNode->m_selectorCache : nullptr;

    // =================================================================== calculating transform

    currentUIElement->calculateLayout(parentCSSSelectorCache, currentTransformNode.m_selectorCache,
                                      parentTransform, currentTransformNode.m_transform);

    TransformUtils::calculateTransform(currentTransformNode.m_transform, parentTransform);

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

        currentUIElement->useUniforms(currentTransformNode.m_selectorCache);

        CoreMain::getRenderer()->renderArray(
                currentUIElement->m_meshData->getVertexArray(),
                m_meshRenderState,
                currentUIElement->m_meshData->m_vertices.size(),
                currentUIElement->m_meshData->m_indices.size()
        );
    }

    // ===================================================================

    for(const auto& child : currentUIElement->m_children)
    {
        const std::int64_t newChildNodeIdx = processUIElement(cameraReceiver, currentUITransformNodeIdx, uiComponent, child);

        // new child was created
        if(newChildNodeIdx != -1)
        {
            uiComponent.m_transformTree.m_elements[currentUITransformNodeIdx].m_children.push_back(newChildNodeIdx);
        }
    }

    if(isCreatedNewTransformNode)
    {
        return currentUITransformNodeIdx;
    }

    return -1;
}
