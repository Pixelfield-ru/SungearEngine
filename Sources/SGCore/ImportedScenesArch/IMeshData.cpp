#include <LinearMath/btVector3.h>

#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Picking/Pickable.h"

#include "IMeshData.h"

#include "SGCore/Main/CoreMain.h"

#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"

#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/SpacePartitioning/OctreeCullable.h"

#include "SGCore/Memory/AssetManager.h"

/*SGCore::Mesh::Mesh() noexcept
{
    m_material = Ref<Memory::Assets::IMaterial>(Main::CoreMain::getRenderer().createPBRMaterial());
}*/

SGCore::IMeshData::IMeshData()
{
    // m_material = AssetManager::getInstance()->createAsset<IMaterial>();
    m_material = AssetManager::getInstance()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
}

void SGCore::IMeshData::setVertexPosition
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_vertices[vertexIdx].m_position.x = x;
    m_vertices[vertexIdx].m_position.y = y;
    m_vertices[vertexIdx].m_position.z = z;
}

void SGCore::IMeshData::getVertexPosition(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_vertices[vertexIdx].m_position.x;
    outY = m_vertices[vertexIdx].m_position.y;
    outZ = m_vertices[vertexIdx].m_position.z;
}

// ------------

void SGCore::IMeshData::setVertexUV(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_vertices[vertexIdx].m_uv.x = x;
    m_vertices[vertexIdx].m_uv.y = y;
    m_vertices[vertexIdx].m_uv.z = z;
}

void SGCore::IMeshData::getVertexUV(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_vertices[vertexIdx].m_uv.x;
    outY = m_vertices[vertexIdx].m_uv.y;
    outZ = m_vertices[vertexIdx].m_uv.z;
}

// ------------

void SGCore::IMeshData::setVertexNormal
(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    m_vertices[vertexIdx].m_normal.x = x;
    m_vertices[vertexIdx].m_normal.y = y;
    m_vertices[vertexIdx].m_normal.z = z;
}

void SGCore::IMeshData::getVertexNormal(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept
{
    outX = m_vertices[vertexIdx].m_normal.x;
    outY = m_vertices[vertexIdx].m_normal.y;
    outZ = m_vertices[vertexIdx].m_normal.z;
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

SGCore::Ref<SGCore::IVertexArray> SGCore::IMeshData::getVertexArray() const noexcept
{
    return m_vertexArray;
}

void SGCore::IMeshData::setData(const AssetRef<IMeshData>& other) noexcept
{
    if(m_vertexArray) m_vertexArray->destroy();

    /*if(m_positionsBuffer) m_positionsBuffer->destroy();
    if(m_uvBuffer) m_uvBuffer->destroy();
    if(m_normalsBuffer) m_normalsBuffer->destroy();
    if(m_tangentsBuffer) m_tangentsBuffer->destroy();
    if(m_bitangentsBuffer) m_bitangentsBuffer->destroy();*/
    for(const auto& colorsBuf : m_verticesColorsBuffers)
    {
        if(colorsBuf)
        {
            colorsBuf->destroy();
        }
    }

    m_verticesColorsBuffers.clear();

    if(m_indicesBuffer) m_indicesBuffer->destroy();

    m_indices = other->m_indices;
    m_vertices = other->m_vertices;
    m_verticesColors = other->m_verticesColors;
    /*m_positions = other->m_positions;
    m_uv = other->m_uv;
    m_normals = other->m_normals;
    m_tangents = other->m_tangents;
    m_bitangents = other->m_tangents;*/

    prepare();

    other->m_material->copyTexturesRefs(m_material.get());
}

SGCore::ECS::entity_t SGCore::IMeshData::addOnScene(const Ref<Scene>& scene, const std::string& layerName) noexcept
{
    auto registry = scene->getECSRegistry();
    
    auto meshEntity = registry->create();

    registry->emplace<Pickable>(meshEntity);
    Ref<Transform>& meshTransform = registry->emplace<Transform>(meshEntity, MakeRef<Transform>());
    Mesh& meshEntityMesh = registry->emplace<Mesh>(meshEntity);
    // NOT STANDARD
    // auto cullableMesh = registry->emplace<Ref<OctreeCullable>>(meshEntity, MakeRef<OctreeCullable>());
    // maybe can load the ram
    meshEntityMesh.m_base.setMeshData(assetRef());
    
    // meshEntityMesh.m_base.m_meshData = shared_from_this();
    
    return meshEntity;
}

void SGCore::IMeshData::migrateAndSetNewMaterial
        (const AssetRef<IMaterial>& newMaterial) noexcept
{
    m_material->copyTexturesRefs(newMaterial.get());
    m_material = newMaterial;
}

void SGCore::IMeshData::generatePhysicalMesh() noexcept
{
    std::vector<glm::vec3> positions;
    for(const auto& vertex : m_vertices)
    {
        positions.push_back(vertex.m_position);
    }

    m_physicalMesh = generatePhysicalMesh(positions, m_indices);
}

void SGCore::IMeshData::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    auto& package = parentAssetManager->getPackage();

    m_indices = package.readData<std::vector<std::uint32_t>>(m_indicesOffsetInPackage, m_indicesSizeInPackage);
    m_vertices = package.readData<std::vector<Vertex>>(m_verticesOffsetInPackage, m_verticesSizeInPackage);

    prepare();
}

void SGCore::IMeshData::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    // resolving reference
    AssetManager::resolveAssetReference(updatedAssetManager, m_material);
}

void SGCore::IMeshData::doLoad(const InterpolatedPath& path)
{

}

void SGCore::IMeshData::doLazyLoad()
{

}

void SGCore::IMeshData::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                         SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{

}
