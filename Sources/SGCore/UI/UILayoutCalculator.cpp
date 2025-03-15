//
// Created by stuka on 03.03.2025.
//

#include "UILayoutCalculator.h"

#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/TransformUtils.h"

void SGCore::UI::UILayoutCalculator::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto scene = getScene();

    auto uiComponentsView = scene->getECSRegistry()->view<UIComponent>();

    uiComponentsView.each([this](UIComponent::reg_t& uiComponent) {
        m_currentProceedUIElements = 0;
        
        bool isTreeFormed = true;
        
        processUIElement(-1, uiComponent, uiComponent.m_document->m_rootElement, nullptr, isTreeFormed);
    });
}

std::int64_t SGCore::UI::UILayoutCalculator::processUIElement(const std::int64_t& parentUITreeNodeIdx,
                                                              UIComponent::reg_t& uiComponent,
                                                              const Ref<UIElement>& currentUIElement,
                                                              const Ref<UIElement>& parentUIElement,
                                                              bool& isTreeFormed) noexcept
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
        isTreeFormed = false;
    }

    auto& currentTransformNode = uiComponent.m_transformTree.m_elements[currentUITransformNodeIdx];
    auto& currentElementCache = currentTransformNode.m_currentElementCache;

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

    for(const auto& child : currentUIElement->m_children)
    {
        const std::int64_t newChildNodeIdx = processUIElement(currentUITransformNodeIdx, uiComponent,
                                                              child, currentUIElement, isTreeFormed);

        // new child was created
        if(newChildNodeIdx != -1)
        {
            uiComponent.m_transformTree.m_elements[currentUITransformNodeIdx].m_children.push_back(newChildNodeIdx);
        }
    }
    
    if(isTreeFormed)
    {
        if(parentTransformNode)
        {
            calculateElementLayout(parentUIElement, *parentTransformNode, currentTransformNode);
        }
        
        currentElementCache.m_curLocalPositionForElements =
                glm::vec3 { currentTransformNode.m_currentElementCache.m_finalSize, 0.0f } / -2.0;
        currentElementCache.m_curLocalPositionForElements.x += currentElementCache.m_leftPadding;
        currentElementCache.m_curLocalPositionForElements.y += currentElementCache.m_topPadding;
        currentElementCache.m_lastRowSize = {};
        currentElementCache.m_contentSize.x = currentElementCache.m_leftPadding + currentElementCache.m_rightPadding;
        currentElementCache.m_contentSize.y = currentElementCache.m_topPadding + currentElementCache.m_bottomPadding;
        
        TransformUtils::calculateTransform(currentTransformNode.m_transform, parentTransform);
        
        currentTransformNode.m_lastElementCache = currentElementCache;
    }

    // =================================================================== reset some cache data

    if(isCreatedNewTransformNode)
    {
        return currentUITransformNodeIdx;
    }

    return -1;
}

void SGCore::UI::UILayoutCalculator::calculateElementLayout(const Ref<UIElement>& parentUIElement,
                                                            UITransformTreeElement& parentElementTransform,
                                                            UITransformTreeElement& currentElementTransform) noexcept
{
    auto& parentElementCache = parentElementTransform.m_currentElementCache;
    auto& lastParentElementCache = parentElementTransform.m_lastElementCache;
    auto& currentElementCache = currentElementTransform.m_currentElementCache;

    auto parentSelector = parentUIElement->m_selector;

    if(!parentSelector) return;

    if(parentSelector->m_display == UI::DisplayKeyword::KW_FLEX)
    {
        if(parentSelector->m_flexDirection == UI::FlexboxKeyword::KW_ROW)
        {
            // moving cursor to a new line if current element is bigger than (containerSize.x / 2 - rightPadding)
            if(parentElementCache.m_curLocalPositionForElements.x + currentElementCache.m_finalSize.x > parentElementCache.m_finalSize.x / 2.0f - parentElementCache.m_rightPadding)
            {
                parentElementCache.m_curLocalPositionForElements.x = parentElementCache.m_finalSize.x / -2.0f + parentElementCache.m_leftPadding;
                parentElementCache.m_curLocalPositionForElements.y += parentElementCache.m_lastRowSize.y + parentElementCache.m_gap.y;
                parentElementCache.m_contentSize.y += parentElementCache.m_lastRowSize.y + parentElementCache.m_gap.y;

                parentElementCache.m_lastRowSize.y = 0.0f;
            }

            if(currentElementCache.m_finalSize.y > parentElementCache.m_lastRowSize.y)
            {
                parentElementCache.m_lastRowSize.y = currentElementCache.m_finalSize.y;
            }

            glm::vec3 currentElementPos = parentElementCache.m_curLocalPositionForElements;
            currentElementPos += glm::vec3 { currentElementCache.m_finalSize, 0.0 } / 2.0f;

            currentElementTransform.m_transform.m_ownTransform.m_position = currentElementPos;

            parentElementCache.m_curLocalPositionForElements.x += currentElementCache.m_finalSize.x + parentElementCache.m_gap.x;

            if(parentElementCache.m_contentSize.y + parentElementCache.m_lastRowSize.y > parentElementCache.m_finalSize.y)
            {
                parentElementCache.m_finalSize.y = parentElementCache.m_contentSize.y + parentElementCache.m_lastRowSize.y;

                // parentElementTransform.m_transform.m_ownTransform.m_position.x += (parentElementCache.m_finalSize.x - lastParentElementCache.m_finalSize.x) / 2.0f;
                parentElementTransform.m_transform.m_ownTransform.m_position.y += (parentElementCache.m_finalSize.y - lastParentElementCache.m_finalSize.y) / 2.0f;
            }
        }
        else if(parentSelector->m_flexDirection == UI::FlexboxKeyword::KW_COLUMN)
        {

        }
    }
    else if(parentSelector->m_display == DisplayKeyword::KW_BLOCK)
    {
        glm::vec3 currentElementPos = parentElementCache.m_curLocalPositionForElements;
        currentElementPos += glm::vec3 { currentElementCache.m_finalSize, 0.0 } / 2.0f;

        currentElementTransform.m_transform.m_ownTransform.m_position = currentElementPos;

        currentElementCache.m_contentSize.y += currentElementCache.m_finalSize.y + parentElementCache.m_gap.y;

        parentElementCache.m_curLocalPositionForElements.y += currentElementCache.m_finalSize.y + parentElementCache.m_gap.y;
    }
}
