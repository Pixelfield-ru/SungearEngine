//
// Created by stuka on 25.01.2025.
//

#include "UIRoot.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

void SGCore::UI::UIRoot::doCalculateLayout(const Transform* parentTransform, Transform& ownTransform) noexcept
{

}

void SGCore::UI::UIRoot::doGenerateMeshBaseSelector() noexcept
{
    doGenerateBasicMesh();
}

void SGCore::UI::UIRoot::doGenerateBasicMesh() noexcept
{
    m_meshData->m_vertices.resize(4);

    m_meshData->m_vertices[0] = {
        .m_position = { -0.5f, -0.5f, 0.0f }
    };

    m_meshData->m_vertices[1] = {
        .m_position = { -0.5f, 0.5f, 0.0f }
    };

    m_meshData->m_vertices[2] = {
        .m_position = { 0.5f, 0.5f, 0.0f }
    };

    m_meshData->m_vertices[3] = {
        .m_position = { 0.5f, -0.5f, 0.0f }
    };

    // =======================================================

    m_meshData->m_indices.resize(6);

    m_meshData->m_indices[0] = 0;
    m_meshData->m_indices[1] = 1;
    m_meshData->m_indices[2] = 2;
    m_meshData->m_indices[3] = 0;
    m_meshData->m_indices[4] = 3;
    m_meshData->m_indices[5] = 2;

    m_meshData->prepare();
}
