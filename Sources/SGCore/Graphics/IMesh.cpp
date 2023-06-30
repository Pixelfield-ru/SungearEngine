#include "IMesh.h"
#include "SGCore/Main/Core.h"
#include "API/IVertexBufferLayout.h"

void Core::Graphics::IMesh::prepare()
{
    m_vertexArray = std::shared_ptr<API::IVertexArray>(Core::Main::Core::getRenderer().createVertexArray());
    m_vertexArray->create()->bind();

    // ---------------- preparing positions -------
    m_positionsBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    m_positionsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_positions);

    std::shared_ptr<API::IVertexBufferLayout> bufferLayout = std::shared_ptr<Core::Graphics::API::IVertexBufferLayout>(Core::Main::Core::getRenderer().createVertexBufferLayout());
    bufferLayout
    ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(bufferLayout->createVertexAttribute(0, "positionsAttribute", SGGDataType::SGG_FLOAT3)))
    ->prepare()->enableAttributes();
    // --------------------------------------------

    // ----- preparing uv -------------------------
    m_uvBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    m_uvBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_uv);

    bufferLayout->reset();
    bufferLayout
    ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(bufferLayout->createVertexAttribute(1, "UVAttribute", SGGDataType::SGG_FLOAT3)))
    ->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing normals ---------------
    m_normalsBuffer = std::shared_ptr<Core::Graphics::API::IVertexBuffer>(Core::Main::Core::getRenderer().createVertexBuffer());
    m_normalsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_normals);

    bufferLayout->reset();
    bufferLayout
    ->addAttribute(std::shared_ptr<Core::Graphics::API::IVertexAttribute>(bufferLayout->createVertexAttribute(2, "normalsAttribute", SGGDataType::SGG_FLOAT3)))
    ->prepare()->enableAttributes();
    // --------------------------------------------

    // ------ preparing indices -------------------
    m_indicesBuffer = std::shared_ptr<Core::Graphics::API::IIndexBuffer>(Core::Main::Core::getRenderer().createIndexBuffer());
    m_indicesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_indices);
    // --------------------------------------------
}

void Core::Graphics::IMesh::setVertexPosition
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_positions[vertexIdx * 3] = x;
    m_positions[vertexIdx * 3 + 1] = y;
    m_positions[vertexIdx * 3 + 2] = z;
}

void Core::Graphics::IMesh::getVertexPosition(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_positions[vertexIdx * 3];
    outY = m_positions[vertexIdx * 3 + 1];
    outZ = m_positions[vertexIdx * 3 + 2];
}

// ------------

void Core::Graphics::IMesh::setVertexUV(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_uv[vertexIdx * 3] = x;
    m_uv[vertexIdx * 3 + 1] = y;
    m_uv[vertexIdx * 3 + 2] = z;
}

void Core::Graphics::IMesh::getVertexUV(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_uv[vertexIdx * 3];
    outY = m_uv[vertexIdx * 3 + 1];
    outZ = m_uv[vertexIdx * 3 + 2];
}

// ------------

void Core::Graphics::IMesh::setVertexNormal
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_normals[vertexIdx * 3] = x;
    m_normals[vertexIdx * 3 + 1] = y;
    m_normals[vertexIdx * 3 + 2] = z;
}

void Core::Graphics::IMesh::getVertexNormal(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_normals[vertexIdx * 3];
    outY = m_normals[vertexIdx * 3 + 1];
    outZ = m_normals[vertexIdx * 3 + 2];
}

// ------------

void Core::Graphics::IMesh::setIndex(const std::uint64_t& faceIdx, const std::uint64_t& indexIdx, const std::uint64_t& value) noexcept
{
    m_indices[faceIdx * 3 + indexIdx] = value;
}

void Core::Graphics::IMesh::getFaceIndices(const std::uint64_t& faceIdx, std::uint64_t& outIdx0, std::uint64_t& outIdx1, std::uint64_t& outIdx2) noexcept
{
    outIdx0 = m_indices[faceIdx * 3];
    outIdx1 = m_indices[faceIdx * 3 + 1];
    outIdx2 = m_indices[faceIdx * 3 + 2];
}

std::shared_ptr<Core::Graphics::API::IVertexArray> Core::Graphics::IMesh::getVertexArray() noexcept
{
    return m_vertexArray;
}
