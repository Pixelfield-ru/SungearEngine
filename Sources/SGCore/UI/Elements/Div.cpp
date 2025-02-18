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
    ownTransform.m_ownTransform.m_position.x = -100.0f;

    thisElementCache.m_size.x = 100;
    thisElementCache.m_size.y = 100;
    thisElementCache.m_totalBorderWidth = 20.0f;
}

void SGCore::UI::Div::doGenerateMeshBaseSelector() noexcept
{
    doGenerateBasicMesh();
}

void SGCore::UI::Div::doGenerateBasicMesh() noexcept
{
    NineSlice::generate9SlicedQuad<std::uint32_t>(20, 0, m_meshData->m_vertices, m_meshData->m_indices);

    m_meshData->prepare();
}
