//
// Created by stuka on 25.01.2025.
//
#include "UIElement.h"

#include "SGCore/ImportedScenesArch/IMeshData.h"

SGCore::UI::UIElementType SGCore::UI::UIElement::getType() const noexcept
{
    return m_type;
}

void SGCore::UI::UIElement::useUniforms(const CSSSelectorCache& thisCSSSelectorCache) const noexcept
{
    const auto& selectorCache = thisCSSSelectorCache;

    m_shader->useVectorf("u_meshAABB", glm::vec4 {
        m_meshData->m_aabb.m_min.x,
        m_meshData->m_aabb.m_min.y,
        m_meshData->m_aabb.m_max.y,
        m_meshData->m_aabb.m_max.y,
    });

    m_shader->useVectorf("u_elementSize", glm::vec2 { selectorCache.m_width, selectorCache.m_height });

    m_shader->useVectorf("u_backgroundColor", selectorCache.m_backgroundColor);
}

void SGCore::UI::UIElement::calculateLayout(const CSSSelectorCache* parentSelectorCache,
                                            CSSSelectorCache& thisSelectorCache, const Transform* parentTransform,
                                            Transform& ownTransform) noexcept
{
    checkForMeshGenerating();

    doCalculateLayout(parentSelectorCache, thisSelectorCache, parentTransform, ownTransform);
}

void SGCore::UI::UIElement::checkForMeshGenerating() noexcept
{
    if(!m_meshData)
    {
        m_meshData = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());

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

