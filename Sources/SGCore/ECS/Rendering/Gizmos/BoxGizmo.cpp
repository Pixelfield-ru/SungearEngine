#include "BoxGizmo.h"

#include "SGCore/Main/CoreMain.h"

SGCore::BoxGizmo::BoxGizmo() noexcept
{
    build();
}

void SGCore::BoxGizmo::build()
{
    // building a cube ------------------------------

    m_meshData->m_positions.clear();

    m_meshData->m_positions.push_back(-m_size.x / 2.0f);
    m_meshData->m_positions.push_back(-m_size.y / 2.0f);
    m_meshData->m_positions.push_back(m_size.z / 2.0f);

    m_meshData->m_positions.push_back(-m_size.x / 2.0f);
    m_meshData->m_positions.push_back(-m_size.y / 2.0f);
    m_meshData->m_positions.push_back(-m_size.z / 2.0f);

    m_meshData->m_positions.push_back(-m_size.x / 2.0f);
    m_meshData->m_positions.push_back(m_size.y / 2.0f);
    m_meshData->m_positions.push_back(-m_size.z / 2.0f);

    m_meshData->m_positions.push_back(-m_size.x / 2.0f);
    m_meshData->m_positions.push_back(m_size.y / 2.0f);
    m_meshData->m_positions.push_back(m_size.z / 2.0f);



    m_meshData->m_positions.push_back(m_size.x / 2.0f);
    m_meshData->m_positions.push_back(-m_size.y / 2.0f);
    m_meshData->m_positions.push_back(m_size.z / 2.0f);

    m_meshData->m_positions.push_back(m_size.x / 2.0f);
    m_meshData->m_positions.push_back(-m_size.y / 2.0f);
    m_meshData->m_positions.push_back(-m_size.z / 2.0f);

    m_meshData->m_positions.push_back(m_size.x / 2.0f);
    m_meshData->m_positions.push_back(m_size.y / 2.0f);
    m_meshData->m_positions.push_back(-m_size.z / 2.0f);

    m_meshData->m_positions.push_back(m_size.x / 2.0f);
    m_meshData->m_positions.push_back(m_size.y / 2.0f);
    m_meshData->m_positions.push_back(m_size.z / 2.0f);

    // --------------------------------

    m_meshData->m_indices.clear();

    m_meshData->m_indices.push_back(0);
    m_meshData->m_indices.push_back(1);

    m_meshData->m_indices.push_back(1);
    m_meshData->m_indices.push_back(2);

    m_meshData->m_indices.push_back(2);
    m_meshData->m_indices.push_back(3);

    m_meshData->m_indices.push_back(3);
    m_meshData->m_indices.push_back(0);



    m_meshData->m_indices.push_back(4);
    m_meshData->m_indices.push_back(5);

    m_meshData->m_indices.push_back(5);
    m_meshData->m_indices.push_back(6);

    m_meshData->m_indices.push_back(6);
    m_meshData->m_indices.push_back(7);

    m_meshData->m_indices.push_back(7);
    m_meshData->m_indices.push_back(4);



    m_meshData->m_indices.push_back(0);
    m_meshData->m_indices.push_back(4);

    m_meshData->m_indices.push_back(3);
    m_meshData->m_indices.push_back(7);

    m_meshData->m_indices.push_back(2);
    m_meshData->m_indices.push_back(6);

    m_meshData->m_indices.push_back(1);
    m_meshData->m_indices.push_back(5);

    m_meshData->prepare();
}
