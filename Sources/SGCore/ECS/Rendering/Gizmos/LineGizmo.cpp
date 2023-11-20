#include "LineGizmo.h"

#include "SGCore/Graphics/Defines.h"
#include "SGCore/Main/CoreMain.h"

SGCore::LineGizmo::LineGizmo() noexcept
{
    m_meshData = Ref<IMeshData>(CoreMain::getRenderer().createMeshData());

    m_meshData->m_useIndices = false;

    // first point
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(0.0);

    // second point
    m_meshData->m_positions.push_back(0.0);
    m_meshData->m_positions.push_back(10.0);
    m_meshData->m_positions.push_back(0.0);
}

void SGCore::LineGizmo::setVertexPosition
(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    if(vertexIdx > 1) return;

    m_meshData->setVertexPosition(vertexIdx, x, y, z);

    // TODO: MOVE IN RENDER PIPELINE
    /*if(x != curX || y != curY || z != curZ)
    {
        const auto& materialShader = m_mesh->m_material->getCurrentShader();

        if(!materialShader) return;

        materialShader->bind();

        materialShader->useVectorf(
                "verticesPositions[" + std::to_string(vertexIdx) + "]",
                x, y, z);
    }*/
}
