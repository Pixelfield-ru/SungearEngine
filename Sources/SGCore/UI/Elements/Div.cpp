//
// Created by stuka on 23.01.2025.
//
#include "Div.h"

#include "SGCore/UI/NineSlice.h"
#include "SGCore/UI/UIElementMesh/UIElementMesh.h"

SGCore::UI::Div::Div() noexcept
{
    m_shader = AssetManager::getInstance()->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");
}

void SGCore::UI::Div::doCalculateLayout(const UIElementCache* parentElementCache,
                                        UIElementCache& thisElementCache,
                                        const Transform* parentTransform, Transform& ownTransform) noexcept
{
    for(auto* style : thisElementCache.m_currentFrameStyles)
    {
        style->calculateCache(parentElementCache, thisElementCache);
    }
}

void SGCore::UI::Div::doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept
{
    NineSlice::generate9SlicedQuad<std::uint32_t>(thisElementCache.m_borderRadiusCache, 0, m_meshData->m_vertices, m_meshData->m_indices);
}
