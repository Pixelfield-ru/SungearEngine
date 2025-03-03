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
            processUIElement(cameraReceiver, -1, uiComponent, uiComponent.m_document->m_rootElement, nullptr);
        });

        cameraReceiver.m_layersFrameBuffer->unbind();
    });
}

std::int64_t SGCore::UIRenderPass::processUIElement(const LayeredFrameReceiver::reg_t& cameraReceiver,
                                                    const std::int64_t& parentUITreeNodeIdx,
                                                    UI::UIComponent& uiComponent,
                                                    const Ref<UI::UIElement>& currentUIElement,
                                                    const Ref<UI::UIElement>& parentUIElement) noexcept
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
    auto& currentElementCache = currentTransformNode.m_currentElementCache;

    currentTransformNode.m_lastElementCache = currentElementCache;

    UI::UITransformTreeElement* parentTransformNode { };
    if(parentUITreeNodeIdx != -1)
    {
        parentTransformNode = &uiComponent.m_transformTree.m_elements[parentUITreeNodeIdx];
    }

    const Transform* parentTransform = parentTransformNode ? &parentTransformNode->m_transform : nullptr;
    UI::UIElementCache* parentElementCache = parentTransformNode ? &parentTransformNode->m_currentElementCache : nullptr;

    // =================================================================== calculating transform

    currentUIElement->calculateLayout(parentElementCache,
                                      currentTransformNode.m_lastElementCache,
                                      currentTransformNode.m_currentElementCache,
                                      parentTransform,
                                      currentTransformNode.m_transform);

    if(parentTransformNode)
    {
        calculateElementLayout(parentUIElement, *parentTransformNode, currentTransformNode);
    }

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

        currentUIElement->useUniforms(currentTransformNode.m_currentElementCache);

        CoreMain::getRenderer()->renderArray(
                currentUIElement->m_meshData->getVertexArray(),
                m_meshRenderState,
                currentUIElement->m_meshData->m_vertices.size(),
                currentUIElement->m_meshData->m_indices.size()
        );
    }

    // ===================================================================

    // =================================================================== reset some cache data

    currentTransformNode.m_currentElementCache.m_freeSpaceSize = currentTransformNode.m_currentElementCache.m_size;
    currentTransformNode.m_currentElementCache.m_curLocalPositionForElements = glm::vec3 { currentTransformNode.m_currentElementCache.m_size, 0.0f } / -2.0;
    currentTransformNode.m_currentElementCache.m_curLocalPositionForElements.x += currentElementCache.m_leftPadding;
    currentTransformNode.m_currentElementCache.m_curLocalPositionForElements.y += currentElementCache.m_topPadding;

    for(const auto& child : currentUIElement->m_children)
    {
        const std::int64_t newChildNodeIdx = processUIElement(cameraReceiver, currentUITransformNodeIdx, uiComponent,
                                                              child, currentUIElement);

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

void SGCore::UIRenderPass::calculateElementLayout(const Ref<UI::UIElement>& parentUIElement,
                                                  UI::UITransformTreeElement& parentElementTransform,
                                                  UI::UITransformTreeElement& currentElementTransform) noexcept
{
    auto& parentElementCache = parentElementTransform.m_currentElementCache;
    auto& currentElementCache = currentElementTransform.m_currentElementCache;

    auto parentSelector = parentUIElement->m_selector;

    if(!parentSelector) return;

    if(parentSelector->m_display == UI::DisplayKeyword::KW_FLEX)
    {
        // moving cursor to a new line if current element is bigger than (containerSize.x / 2 - rightPadding)
        if(parentElementCache.m_curLocalPositionForElements.x + currentElementCache.m_size.x > parentElementCache.m_size.x / 2.0f - parentElementCache.m_rightPadding)
        {
            parentElementCache.m_curLocalPositionForElements.x = parentElementCache.m_size.x / -2.0f + parentElementCache.m_leftPadding;
            parentElementCache.m_curLocalPositionForElements.y += parentElementCache.m_lastRowSize.y + parentElementCache.m_gap.y;
            parentElementCache.m_lastRowSize.y = 0.0f;
        }

        if(currentElementCache.m_size.y > parentElementCache.m_lastRowSize.y)
        {
            parentElementCache.m_lastRowSize.y = currentElementCache.m_size.y;
        }

        glm::vec3 currentElementPos = parentElementCache.m_curLocalPositionForElements;
        currentElementPos += glm::vec3 { currentElementCache.m_size, 0.0 } / 2.0f;

        currentElementTransform.m_transform.m_ownTransform.m_position = currentElementPos;

        parentElementCache.m_curLocalPositionForElements.x += currentElementCache.m_size.x + parentElementCache.m_gap.x;
    }
}
