//
// Created by stuka on 10.10.2025.
//

#include "UIElementCache.h"

SGCore::UI::UIElementCache::UIElementCache(const UIElementCache& other) noexcept
{
    this->operator=(other);
}

SGCore::UI::UIElementCache::UIElementCache(UIElementCache&& other) noexcept
{
    this->operator=(std::move(other));
}

SGCore::UI::UIElementCache& SGCore::UI::UIElementCache::operator=(const UIElementCache& other) noexcept
{
    if(this == &other) return *this;

    m_basicSize = other.m_basicSize;

    m_finalSize = other.m_finalSize;
    m_backgroundColor = other.m_backgroundColor;
    m_color = other.m_color;

    m_totalBorderSize = other.m_totalBorderSize;

    m_gap = other.m_gap;

    m_topPadding = other.m_topPadding;
    m_rightPadding = other.m_rightPadding;
    m_bottomPadding = other.m_bottomPadding;
    m_leftPadding = other.m_leftPadding;

    m_borderRadiusCache = other.m_borderRadiusCache;

    m_fontSize = other.m_fontSize;

    m_currentFrameStyles = other.m_currentFrameStyles;

    m_contentSize = other.m_contentSize;

    m_curLocalPositionForElements = other.m_curLocalPositionForElements;

    m_lastRowSize = other.m_lastRowSize;

    m_childrenElementsScale = other.m_childrenElementsScale;

    m_layer = other.m_layer;

    return *this;
}

SGCore::UI::UIElementCache& SGCore::UI::UIElementCache::operator=(UIElementCache&& other) noexcept
{
    if(this == &other) return *this;

    m_basicSize = other.m_basicSize;

    m_finalSize = other.m_finalSize;
    m_backgroundColor = other.m_backgroundColor;
    m_color = other.m_color;

    m_totalBorderSize = other.m_totalBorderSize;

    m_gap = other.m_gap;

    m_topPadding = other.m_topPadding;
    m_rightPadding = other.m_rightPadding;
    m_bottomPadding = other.m_bottomPadding;
    m_leftPadding = other.m_leftPadding;

    m_borderRadiusCache = other.m_borderRadiusCache;

    m_fontSize = other.m_fontSize;

    m_currentFrameStyles = std::move(other.m_currentFrameStyles);

    m_contentSize = other.m_contentSize;

    m_curLocalPositionForElements = other.m_curLocalPositionForElements;

    m_lastRowSize = other.m_lastRowSize;

    m_childrenElementsScale = other.m_childrenElementsScale;

    m_layer = other.m_layer;

    return *this;
}
