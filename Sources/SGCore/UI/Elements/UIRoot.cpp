//
// Created by stuka on 25.01.2025.
//

#include "UIRoot.h"
#include "SGCore/UI/NineSlice.h"
#include "SGCore/UI/Math/CSSMathNumericNode.h"
#include "SGCore/UI/UIElementMesh/UIElementMesh.h"

SGCore::UI::UIRoot::UIRoot() noexcept
{
    m_shader = AssetManager::getInstance()->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");
    // default style for root
    // m_mainStyle = AssetManager::getInstance()->getOrAddAssetByAlias<Style>("sgui_root_style");

    /*auto bottomRightRadius = MakeRef<CSSMathNumericNode>();
    bottomRightRadius->m_value = 30.0f;

    m_mainStyle->m_bottomRightBorderRadius.setWithAlternative({});
    m_mainStyle->m_bottomRightBorderRadius.m_value = BorderRadiusAlternativeValue {
        .m_radiusX = bottomRightRadius,
        .m_radiusY = bottomRightRadius
    };
    */

    /*auto topPadding = MakeRef<CSSMathNumericNode>();
    topPadding->m_value = 250;

    m_mainStyle->m_padding.setWithAlternative({});
    m_mainStyle->m_padding.getFromAlternativeValue<0>() = topPadding;
    m_mainStyle->m_padding.getFromAlternativeValue<3>() = topPadding;*/
    
    m_mainStyle->m_display = UI::DisplayKeyword::BLOCK;
}

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIRoot::copy() const noexcept
{
    auto element = MakeRef<UIRoot>();
    UIElement::doCopy(element);

    return element;
}

void SGCore::UI::UIRoot::doCalculateLayout(const UIElementCache* parentElementCache,
                                           UIElementCache& thisElementCache,
                                           const Transform* parentTransform, Transform& ownTransform) noexcept
{
    for(auto* style : thisElementCache.m_currentFrameStyles)
    {
        // todo: move into Styles struct to calculate all selector props
        style->calculateCache(parentElementCache, thisElementCache);
    }

    if(thisElementCache.m_currentFrameStyles.empty())
    {
        thisElementCache.m_topPadding = { };
        thisElementCache.m_rightPadding = { };
        thisElementCache.m_bottomPadding = { };
        thisElementCache.m_rightPadding = { };

        thisElementCache.m_gap = { };
    }

    // thisElementCache.m_backgroundColor.r = 255.0f;
    thisElementCache.m_backgroundColor.a = 0.0f;

    int windowSizeX = 0;
    int windowSizeY = 0;

    CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

    thisElementCache.m_finalSize.x = windowSizeX;
    thisElementCache.m_finalSize.y = windowSizeY;
}

void SGCore::UI::UIRoot::doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept
{
    NineSlice::generate9SlicedQuad<std::uint32_t>(thisElementCache.m_borderRadiusCache, 0, m_meshData->m_vertices, m_meshData->m_indices);
}
