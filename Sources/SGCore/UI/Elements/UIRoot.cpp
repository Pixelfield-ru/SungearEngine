//
// Created by stuka on 25.01.2025.
//

#include "UIRoot.h"
#include "SGCore/UI/NineSlice.h"
#include "SGCore/UI/UIElementMesh/UIElementMesh.h"

SGCore::UI::UIRoot::UIRoot() noexcept
{
    m_shader = AssetManager::getInstance()->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");
}

void SGCore::UI::UIRoot::doCalculateLayout(const UIElementCache* parentElementCache,
                                           UIElementCache& thisElementCache,
                                           const Transform* parentTransform, Transform& ownTransform) noexcept
{
    if(m_selector)
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
        // thisSelectorCache.m_width = 120;
        // thisSelectorCache.m_height = 145;
        // thisSelectorCache.m_width = 200;
        // thisSelectorCache.m_height = 200;

        std::cout << "window size: " << windowSizeX << ", " << windowSizeY << std::endl;
    }

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
