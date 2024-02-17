#include <SGCore/Scene/Scene.h>
#include <SGCore/Scene/EntityBaseInfo.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Render/Mesh.h>

#include "IMeshData.h"

#include "SGCore/Main/CoreMain.h"

#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"

/*SGCore::Mesh::Mesh() noexcept
{
    m_material = Ref<Memory::Assets::IMaterial>(Main::CoreMain::getRenderer().createPBRMaterial());
}*/

SGCore::IMeshData::IMeshData()
{
    m_material = IMaterial::create();;
}

void SGCore::IMeshData::setVertexPosition
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_positions[vertexIdx * 3] = x;
    m_positions[vertexIdx * 3 + 1] = y;
    m_positions[vertexIdx * 3 + 2] = z;
}

void SGCore::IMeshData::getVertexPosition(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_positions[vertexIdx * 3];
    outY = m_positions[vertexIdx * 3 + 1];
    outZ = m_positions[vertexIdx * 3 + 2];
}

// ------------

void SGCore::IMeshData::setVertexUV(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_uv[vertexIdx * 3] = x;
    m_uv[vertexIdx * 3 + 1] = y;
    m_uv[vertexIdx * 3 + 2] = z;
}

void SGCore::IMeshData::getVertexUV(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_uv[vertexIdx * 3];
    outY = m_uv[vertexIdx * 3 + 1];
    outZ = m_uv[vertexIdx * 3 + 2];
}

// ------------

void SGCore::IMeshData::setVertexNormal
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_normals[vertexIdx * 3] = x;
    m_normals[vertexIdx * 3 + 1] = y;
    m_normals[vertexIdx * 3 + 2] = z;
}

void SGCore::IMeshData::getVertexNormal(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_normals[vertexIdx * 3];
    outY = m_normals[vertexIdx * 3 + 1];
    outZ = m_normals[vertexIdx * 3 + 2];
}

// ------------

void SGCore::IMeshData::setIndex(const std::uint64_t& faceIdx, const std::uint64_t& indexIdx, const std::uint64_t& value) noexcept
{
    m_indices[faceIdx * 3 + indexIdx] = value;
}

void SGCore::IMeshData::getFaceIndices(const std::uint64_t& faceIdx, std::uint64_t& outIdx0, std::uint64_t& outIdx1, std::uint64_t& outIdx2) noexcept
{
    outIdx0 = m_indices[faceIdx * 3];
    outIdx1 = m_indices[faceIdx * 3 + 1];
    outIdx2 = m_indices[faceIdx * 3 + 2];
}

SGCore::Ref<SGCore::IVertexArray> SGCore::IMeshData::getVertexArray() noexcept
{
    return m_vertexArray;
}

void SGCore::IMeshData::setData(const SGCore::Ref<SGCore::IMeshData>& other) noexcept
{
    if(m_vertexArray) m_vertexArray->destroy();

    if(m_positionsBuffer) m_positionsBuffer->destroy();
    if(m_uvBuffer) m_uvBuffer->destroy();
    if(m_normalsBuffer) m_normalsBuffer->destroy();
    if(m_tangentsBuffer) m_tangentsBuffer->destroy();
    if(m_bitangentsBuffer) m_bitangentsBuffer->destroy();

    if(m_indicesBuffer) m_indicesBuffer->destroy();

    m_indices = other->m_indices;
    m_positions = other->m_positions;
    m_uv = other->m_uv;
    m_normals = other->m_normals;
    m_tangents = other->m_tangents;
    m_bitangents = other->m_tangents;

    prepare();

    other->m_material->copyTextures(m_material);
}

void SGCore::IMeshData::migrateAndSetNewMaterial
(const Ref<IMaterial>& newMaterial) noexcept
{
    m_material->copyTextures(newMaterial);
    m_material = newMaterial;
}

entt::entity SGCore::IMeshData::addOnScene(const Ref<Scene>& scene, const std::string& layerName) noexcept
{
    auto& registry = scene->getECSRegistry();
    
    entt::entity meshEntity = registry.create();
    
    EntityBaseInfo& meshEntityBaseInfo = registry.emplace<EntityBaseInfo>(meshEntity);
    Transform& meshTransform = registry.emplace<Transform>(meshEntity);
    Mesh& meshEntityMesh = registry.emplace<Mesh>(meshEntity);
    meshEntityMesh.m_base.m_meshData->setData(shared_from_this());
    
    return meshEntity;
}
