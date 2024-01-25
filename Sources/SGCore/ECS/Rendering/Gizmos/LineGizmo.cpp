#include "LineGizmo.h"

#include "SGCore/Main/CoreMain.h"

SGCore::LineGizmo::LineGizmo() noexcept
{
    onMeshCreated("Gizmos/LinesGizmosShader");

    m_meshData->m_useIndices = false;

    // first point
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(0.0);

    // second point
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(0.0);
}
