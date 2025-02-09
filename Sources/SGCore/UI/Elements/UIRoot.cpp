//
// Created by stuka on 25.01.2025.
//

#include "UIRoot.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

void SGCore::UI::UIRoot::doCalculateLayout(const CSSSelectorCache* parentSelectorCache,
                                           CSSSelectorCache& thisSelectorCache,
                                           const Transform* parentTransform, Transform& ownTransform) noexcept
{
    if(m_selector)
    {
        auto lockedParent = m_parent.lock();

        if(m_selector->m_width.containsAlternative())
        {
            thisSelectorCache.m_width = m_selector->m_width.getFromAlternativeValue<0>()->calculate(
                parentSelectorCache ? &parentSelectorCache->m_width : nullptr);
        }

        if(m_selector->m_height.containsAlternative())
        {
            thisSelectorCache.m_height = m_selector->m_height.getFromAlternativeValue<0>()->calculate(
                parentSelectorCache ? &parentSelectorCache->m_height : nullptr);
        }
    }
    else
    {
        int windowSizeX = 0;
        int windowSizeY = 0;

        CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

        thisSelectorCache.m_width = windowSizeX;
        thisSelectorCache.m_height = windowSizeY;
    }
}

void SGCore::UI::UIRoot::doGenerateMeshBaseSelector() noexcept
{
    doGenerateBasicMesh();
}

void SGCore::UI::UIRoot::doGenerateBasicMesh() noexcept
{
    m_meshData->m_vertices.resize(4);

    /*m_meshData->m_vertices[0] = {
        .m_position = { -1, -1, 0.0f }
    };

    m_meshData->m_vertices[1] = {
        .m_position = { -1, 1, 0.0f }
    };

    m_meshData->m_vertices[2] = {
        .m_position = { 1, 1, 0.0f }
    };

    m_meshData->m_vertices[3] = {
        .m_position = { 1, -1, 0.0f }
    };*/

    m_meshData->m_vertices[0] = {
        .m_position = { 0, 0, 0.0f }
    };

    m_meshData->m_vertices[1] = {
        .m_position = { 0, 0.4, 0.0f }
    };

    m_meshData->m_vertices[2] = {
        .m_position = { 1, 1, 0.0f }
    };

    m_meshData->m_vertices[3] = {
        .m_position = { 1, 0, 0.0f }
    };

    // =======================================================

    m_meshData->m_indices.resize(6);

    m_meshData->m_indices[0] = 0;
    m_meshData->m_indices[1] = 2;
    m_meshData->m_indices[2] = 1;
    m_meshData->m_indices[3] = 0;
    m_meshData->m_indices[4] = 3;
    m_meshData->m_indices[5] = 2;

    m_meshData->prepare();
}
