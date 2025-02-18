//
// Created by stuka on 25.01.2025.
//
#include "UIElement.h"

#include "UIElementMesh/UIElementMesh.h"

SGCore::UI::UIElementType SGCore::UI::UIElement::getType() const noexcept
{
    return m_type;
}

void SGCore::UI::UIElement::useUniforms(const UIElementCache& thisElementCache) const noexcept
{
    const auto& elementCache = thisElementCache;

    m_shader->useVectorf("u_meshAABB", glm::vec4 {
        m_meshData->m_aabb.m_min.x,
        m_meshData->m_aabb.m_min.y,
        m_meshData->m_aabb.m_max.y,
        m_meshData->m_aabb.m_max.y,
    });

    m_shader->useVectorf("u_elementSize", elementCache.m_size);

    m_shader->useVectorf("u_backgroundColor", elementCache.m_backgroundColor);

    m_shader->useFloat("u_totalBorderWidth", elementCache.m_totalBorderWidth);
}

void SGCore::UI::UIElement::calculateLayout(const UIElementCache* parentElementCache,
                                            UIElementCache& thisElementCache, const Transform* parentTransform,
                                            Transform& ownTransform) noexcept
{
    checkForMeshGenerating();

    doCalculateLayout(parentElementCache, thisElementCache, parentTransform, ownTransform);
}

void SGCore::UI::UIElement::checkForMeshGenerating() noexcept
{
    if(!m_meshData)
    {
        m_meshData = MakeRef<UIElementMesh>();

        if(m_selector)
        {
            doGenerateMeshBaseSelector();
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

