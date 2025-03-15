//
// Created by stuka on 25.01.2025.
//

#include "UIRoot.h"
#include "SGCore/UI/NineSlice.h"
#include "SGCore/UI/CSS/Math/CSSMathNumericNode.h"
#include "SGCore/UI/UIElementMesh/UIElementMesh.h"

SGCore::UI::UIRoot::UIRoot() noexcept
{
    m_shader = AssetManager::getInstance()->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");
    // default style for root
    m_selector = AssetManager::getInstance()->getOrAddAssetByAlias<CSSSelector>("sgui_root_style");

    auto bottomRightRadius = MakeRef<CSSMathNumericNode>();
    bottomRightRadius->m_value = 30.0f;

    m_selector->m_bottomRightBorderRadius.setWithAlternative({});
    m_selector->m_bottomRightBorderRadius.m_value = BorderRadiusAlternativeValue {
        .m_radiusX = bottomRightRadius,
        .m_radiusY = bottomRightRadius
    };

    auto topPadding = MakeRef<CSSMathNumericNode>();
    topPadding->m_value = 250;

    m_selector->m_padding.setWithAlternative({});
    m_selector->m_padding.getFromAlternativeValue<0>() = topPadding;
    m_selector->m_padding.getFromAlternativeValue<3>() = topPadding;
    
    m_selector->m_display = UI::DisplayKeyword::KW_BLOCK;
}

void SGCore::UI::UIRoot::doCalculateLayout(const UIElementCache* parentElementCache,
                                           UIElementCache& thisElementCache,
                                           const Transform* parentTransform, Transform& ownTransform) noexcept
{
    // todo: move into CSSSelector struct to calculate all selector props
    if(m_selector)
    {
        m_selector->calculateCache(parentElementCache, thisElementCache);
    }
    else
    {
        thisElementCache.m_topPadding = { };
        thisElementCache.m_rightPadding = { };
        thisElementCache.m_bottomPadding = { };
        thisElementCache.m_rightPadding = { };

        thisElementCache.m_gap = { };
    }

    thisElementCache.m_backgroundColor.r = 255.0f;
    thisElementCache.m_backgroundColor.a = 255.0f;

    int windowSizeX = 0;
    int windowSizeY = 0;

    CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

    thisElementCache.m_finalSize.x = windowSizeX;
    thisElementCache.m_finalSize.y = windowSizeY;
}

void SGCore::UI::UIRoot::doGenerateMeshBaseSelector(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept
{
    m_selector->calculateCache(parentElementCache, thisElementCache);

    NineSlice::generate9SlicedQuad<std::uint32_t>(thisElementCache.m_borderRadiusCache, 0, m_meshData->m_vertices, m_meshData->m_indices);
}

void SGCore::UI::UIRoot::doGenerateBasicMesh() noexcept
{
    NineSlice::generate9SlicedQuad<std::uint32_t>({ }, 0, m_meshData->m_vertices, m_meshData->m_indices);
}
