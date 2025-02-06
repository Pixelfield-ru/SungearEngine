//
// Created by stuka on 28.01.2025.
//

#include "UIRenderPass.h"

#include "SGCore/Scene/Scene.h"

#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"

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
                                               const RenderingBase::reg_t&,
                                               const Transform::reg_t& cameraTransform){
        uiComponentsView.each([this](UI::UIComponent::reg_t& uiComponent) {
            processUIElement(-1, uiComponent, uiComponent.m_document->m_rootElement);
        });
    });
}

std::int64_t SGCore::UIRenderPass::processUIElement(const std::int64_t& parentUITreeNodeIdx,
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

    // =================================================================== calculating transform

    // =================================================================== rendering uielement

    // ===================================================================

    for(const auto& child : currentUIElement->m_children)
    {
        const std::int64_t newChildNodeIdx = processUIElement(currentUITransformNodeIdx, uiComponent, child);

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
