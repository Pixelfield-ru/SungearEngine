#include "IMesh.h"

#include "SGCore/Main/CoreMain.h"

/*Core::ImportedScene::IMesh::IMesh() noexcept
{
    m_material = std::shared_ptr<Memory::Assets::IMaterial>(Main::CoreMain::getRenderer().createPBRMaterial());
}*/

void Core::ImportedScene::IMesh::setVertexPosition
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_positions[vertexIdx * 3] = x;
    m_positions[vertexIdx * 3 + 1] = y;
    m_positions[vertexIdx * 3 + 2] = z;
}

void Core::ImportedScene::IMesh::getVertexPosition(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_positions[vertexIdx * 3];
    outY = m_positions[vertexIdx * 3 + 1];
    outZ = m_positions[vertexIdx * 3 + 2];
}

// ------------

void Core::ImportedScene::IMesh::setVertexUV(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_uv[vertexIdx * 3] = x;
    m_uv[vertexIdx * 3 + 1] = y;
    m_uv[vertexIdx * 3 + 2] = z;
}

void Core::ImportedScene::IMesh::getVertexUV(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_uv[vertexIdx * 3];
    outY = m_uv[vertexIdx * 3 + 1];
    outZ = m_uv[vertexIdx * 3 + 2];
}

// ------------

void Core::ImportedScene::IMesh::setVertexNormal
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_normals[vertexIdx * 3] = x;
    m_normals[vertexIdx * 3 + 1] = y;
    m_normals[vertexIdx * 3 + 2] = z;
}

void Core::ImportedScene::IMesh::getVertexNormal(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_normals[vertexIdx * 3];
    outY = m_normals[vertexIdx * 3 + 1];
    outZ = m_normals[vertexIdx * 3 + 2];
}

// ------------

void Core::ImportedScene::IMesh::setIndex(const std::uint64_t& faceIdx, const std::uint64_t& indexIdx, const std::uint64_t& value) noexcept
{
    m_indices[faceIdx * 3 + indexIdx] = value;
}

void Core::ImportedScene::IMesh::getFaceIndices(const std::uint64_t& faceIdx, std::uint64_t& outIdx0, std::uint64_t& outIdx1, std::uint64_t& outIdx2) noexcept
{
    outIdx0 = m_indices[faceIdx * 3];
    outIdx1 = m_indices[faceIdx * 3 + 1];
    outIdx2 = m_indices[faceIdx * 3 + 2];
}

std::shared_ptr<Core::Graphics::IVertexArray> Core::ImportedScene::IMesh::getVertexArray() noexcept
{
    return m_vertexArray;
}

void Core::ImportedScene::IMesh::migrateAndSetNewMaterial
(const std::shared_ptr<Memory::Assets::IMaterial>& newMaterial) noexcept
{
    m_material->copyTextures(newMaterial);
    m_material = newMaterial;
}
