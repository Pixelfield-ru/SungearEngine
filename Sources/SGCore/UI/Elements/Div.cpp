#include "Div.h"

#include "Button.h"
#include "SGCore/UI/NineSlice.h"
#include "SGCore/UI/UIElementMesh/UIElementMesh.h"

SGCore::UI::Div::Div() noexcept
{
    m_shader = AssetManager::getInstance()->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");
}

std::unique_ptr<SGCore::UI::UIElement> SGCore::UI::Div::copy() const noexcept
{
    auto element = MakeScope<Div>();
    UIElement::doCopy(*element);

    return element;
}

void SGCore::UI::Div::doCalculateLayout(const UIElementCache* parentSelectorCache,
                                        UIElementCache& thisSelectorCache,
                                        const Transform* parentTransform, Transform& ownTransform) noexcept
{
    for(auto* style : thisSelectorCache.m_currentFrameStyles)
    {
        style->calculateCache(parentSelectorCache, thisSelectorCache);
    }
}

void SGCore::UI::Div::doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept
{
    NineSlice::generate9SlicedQuad<std::uint32_t>(thisElementCache.m_borderRadiusCache, 0, m_meshData->m_vertices, m_meshData->m_indices);
}
