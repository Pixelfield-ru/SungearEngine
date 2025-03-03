//
// Created by stuka on 18.01.2025.
//
#include "CSSSelector.h"

const std::string& SGCore::UI::CSSSelector::getName() const noexcept
{
    return m_name;
}

void SGCore::UI::CSSSelector::calculateCache(const UIElementCache* parentElementCache,
                                             UIElementCache& thisElementCache) noexcept
{
    if(m_padding.containsAlternative())
    {
        thisElementCache.m_topPadding = m_padding.getFromAlternativeValue<0>()->calculate(&thisElementCache.m_size.y);
        thisElementCache.m_rightPadding = m_padding.getFromAlternativeValue<1>()->calculate(&thisElementCache.m_size.x);
        thisElementCache.m_bottomPadding = m_padding.getFromAlternativeValue<2>()->calculate(&thisElementCache.m_size.y);
        thisElementCache.m_leftPadding = m_padding.getFromAlternativeValue<3>()->calculate(&thisElementCache.m_size.x);
    }

    if(m_gap.containsAlternative())
    {
        thisElementCache.m_gap.x = m_gap.getFromAlternativeValue<0>()->calculate(&thisElementCache.m_size.x);
        thisElementCache.m_gap.y = m_gap.getFromAlternativeValue<1>()->calculate(&thisElementCache.m_size.y);
    }

    if(m_width.containsAlternative())
    {
        thisElementCache.m_size.x = m_width.getFromAlternativeValue<0>()->calculate(
            parentElementCache ? &parentElementCache->m_size.x : nullptr);

        thisElementCache.m_finalSize.x = thisElementCache.m_size.x;
    }

    if(m_height.containsAlternative())
    {
        thisElementCache.m_size.y = m_height.getFromAlternativeValue<0>()->calculate(
            parentElementCache ? &parentElementCache->m_size.y : nullptr);

        thisElementCache.m_finalSize.y = thisElementCache.m_size.y;
    }

    if(m_backgroundColor.containsAlternative())
    {
        thisElementCache.m_backgroundColor.r = m_backgroundColor.getFromAlternativeValue<0>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.r : nullptr);

        thisElementCache.m_backgroundColor.g = m_backgroundColor.getFromAlternativeValue<1>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.g : nullptr);

        thisElementCache.m_backgroundColor.b = m_backgroundColor.getFromAlternativeValue<2>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.b : nullptr);

        thisElementCache.m_backgroundColor.a = m_backgroundColor.getFromAlternativeValue<3>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.a : nullptr);
    }

    if(m_topLeftBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_topLeftBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[0].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_size.x);

        thisElementCache.m_borderRadiusCache[0].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_size.y);
    }

    if(m_topRightBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_topRightBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[1].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_size.x);

        thisElementCache.m_borderRadiusCache[1].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_size.y);
    }

    if(m_bottomRightBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_bottomRightBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[2].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_size.x);

        thisElementCache.m_borderRadiusCache[2].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_size.y);
    }

    if(m_bottomLeftBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_bottomLeftBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[3].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_size.x);

        thisElementCache.m_borderRadiusCache[3].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_size.y);
    }
}

void SGCore::UI::CSSSelector::doLoad(const InterpolatedPath& path)
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::UI::CSSSelector::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::CSSSelector::doReloadFromDisk(AssetsLoadPolicy loadPolicy,
                                               Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}
