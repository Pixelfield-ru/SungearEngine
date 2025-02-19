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

    auto xPaddingMathNode = MakeRef<CSSMathNumericNode>();
    xPaddingMathNode->m_value = 25;
    auto yPaddingMathNode = MakeRef<CSSMathNumericNode>();
    yPaddingMathNode->m_value = 10;

    auto xGapMathNode = MakeRef<CSSMathNumericNode>();
    xGapMathNode->m_value = 10;
    auto yGapMathNode = MakeRef<CSSMathNumericNode>();
    yGapMathNode->m_value = 10;

    m_selector->m_padding.setWithAlternative({ xPaddingMathNode, yPaddingMathNode });
    m_selector->m_gap.setWithAlternative({ xGapMathNode, yGapMathNode });
}

void SGCore::UI::UIRoot::doCalculateLayout(const UIElementCache* parentElementCache,
                                           UIElementCache& thisElementCache,
                                           const Transform* parentTransform, Transform& ownTransform) noexcept
{
    /*if(m_selector)
    {
        auto lockedParent = m_parent.lock();

        if(m_selector->m_width.containsAlternative())
        {
            thisElementCache.m_size.x = m_selector->m_width.getFromAlternativeValue<0>()->calculate(
                parentElementCache ? &parentElementCache->m_size.x : nullptr);
        }

        if(m_selector->m_height.containsAlternative())
        {
            thisElementCache.m_size.y = m_selector->m_height.getFromAlternativeValue<0>()->calculate(
                parentElementCache ? &parentElementCache->m_size.y : nullptr);
        }
    }
    else
    {
        int windowSizeX = 0;
        int windowSizeY = 0;

        CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

        thisElementCache.m_size.x = windowSizeX;
        thisElementCache.m_size.y = windowSizeY;

        std::cout << "window size: " << windowSizeX << ", " << windowSizeY << std::endl;
    }*/

    // todo: move into CSSSelector struct to calculate all selector props
    if(m_selector)
    {
        auto lockedParent = m_parent.lock();

        if(m_selector->m_padding.containsAlternative())
        {
            thisElementCache.m_padding.x = m_selector->m_padding.getFromAlternativeValue<0>()->calculate(
                parentElementCache ? &parentElementCache->m_padding.x : nullptr);

            thisElementCache.m_padding.y = m_selector->m_padding.getFromAlternativeValue<1>()->calculate(
                parentElementCache ? &parentElementCache->m_padding.y : nullptr);
        }

        if(m_selector->m_gap.containsAlternative())
        {
            thisElementCache.m_gap.x = m_selector->m_gap.getFromAlternativeValue<0>()->calculate(
                parentElementCache ? &parentElementCache->m_gap.x : nullptr);

            thisElementCache.m_gap.y = m_selector->m_gap.getFromAlternativeValue<1>()->calculate(
                parentElementCache ? &parentElementCache->m_gap.y : nullptr);
        }
    }
    else
    {
        thisElementCache.m_padding = { };
        thisElementCache.m_gap = { };
    }

    int windowSizeX = 0;
    int windowSizeY = 0;

    CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

    thisElementCache.m_size.x = windowSizeX;
    thisElementCache.m_size.y = windowSizeY;

    // TODO: CONSIDER border-radius AND border-width
    thisElementCache.m_totalBorderWidth = 90.0f;
}

void SGCore::UI::UIRoot::doGenerateMeshBaseSelector() noexcept
{
    doGenerateBasicMesh();
}

void SGCore::UI::UIRoot::doGenerateBasicMesh() noexcept
{
    NineSlice::generate9SlicedQuad<std::uint32_t>(90, 0, m_meshData->m_vertices, m_meshData->m_indices);

    m_meshData->prepare();
}
