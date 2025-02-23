//
// Created by stuka on 25.01.2025.
//
#include "UIElement.h"

#include "UIElementMesh/UIElementMesh.h"

SGCore::UI::UIElementType SGCore::UI::UIElement::getType() const noexcept
{
    return m_type;
}

void SGCore::UI::UIElement::useUniforms(UIElementCache& thisElementCache) const noexcept
{
    auto& elementCache = thisElementCache;

    m_shader->useVectorf("u_meshAABB", glm::vec4 {
        m_meshData->m_aabb.m_min.x,
        m_meshData->m_aabb.m_min.y,
        m_meshData->m_aabb.m_max.y,
        m_meshData->m_aabb.m_max.y,
    });

    m_shader->useVectorf("u_elementSize", elementCache.m_size);

    m_shader->useVectorf("u_backgroundColor", elementCache.m_backgroundColor);

    elementCache.m_totalBorderSize.x = std::ranges::max({
        elementCache.m_borderRadiusCache[0].x,
        elementCache.m_borderRadiusCache[1].x,
        elementCache.m_borderRadiusCache[2].x,
        elementCache.m_borderRadiusCache[3].x
    });

    elementCache.m_totalBorderSize.y = std::ranges::max({
        elementCache.m_borderRadiusCache[0].y,
        elementCache.m_borderRadiusCache[1].y,
        elementCache.m_borderRadiusCache[2].y,
        elementCache.m_borderRadiusCache[3].y
    });

    m_shader->useVectorf("u_totalBorderSize", elementCache.m_totalBorderSize);
}

void SGCore::UI::UIElement::calculateLayout(const UIElementCache* parentElementCache,
                                            UIElementCache& thisElementCache, const Transform* parentTransform,
                                            Transform& ownTransform) noexcept
{
    checkForMeshGenerating(parentElementCache, thisElementCache);

    doCalculateLayout(parentElementCache, thisElementCache, parentTransform, ownTransform);
}

void SGCore::UI::UIElement::checkForMeshGenerating(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept
{
    if(!m_meshData)
    {
        m_meshData = MakeRef<UIElementMesh>();

        if(m_selector)
        {
            doGenerateMeshBaseSelector(parentElementCache, thisElementCache);
        }
        else
        {
            doGenerateBasicMesh();
        }

        if(m_meshData)
        {
            m_meshData->m_aabb.calculate(m_meshData->m_vertices);
        }
    }
}

