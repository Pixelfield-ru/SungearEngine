#include "IMeshData.h"

#include <LinearMath/btVector3.h>

#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Picking/Pickable.h"

#include "SGCore/Graphics/API/GPUDeviceInfo.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/RenderAbilities/EnableMeshPass.h"

/*SGCore::Mesh::Mesh() noexcept
{
    m_material = Ref<Memory::Assets::IMaterial>(Main::CoreMain::getRenderer().createPBRMaterial());
}*/

SGCore::IMeshData::IMeshData()
{
    // m_material = AssetManager::getInstance()->createAsset<IMaterial>();
    m_material = AssetManager::getInstance()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
}

void SGCore::IMeshData::prepare()
{
    // any RHI mirror built from the previous data is stale now
    m_rhi = RHIData { };

    m_vertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_vertexArray->create();
    m_vertexArray->bind();

    // ---------------- preparing positions -------
    m_verticesBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
            );
    m_verticesBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_verticesBuffer->create();
    m_verticesBuffer->bind();
    m_verticesBuffer->putData(m_vertices);

    m_vertexArray->addVertexBuffer(m_verticesBuffer.get());

    m_verticesColorsBuffers.clear();

    for(size_t i = 0; i < m_verticesColors.size(); ++i)
    {
        auto verticesColorsBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );

        verticesColorsBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
        verticesColorsBuffer->create();
        verticesColorsBuffer->bind();
        verticesColorsBuffer->putData(m_verticesColors[i].m_colors);

        m_vertexArray->addVertexBuffer(verticesColorsBuffer.get());

        m_verticesColorsBuffers.push_back(verticesColorsBuffer);
    }

    // -------------------------------------------------

    // ------ preparing indices -------------------
    m_indicesBuffer = std::shared_ptr<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
    m_indicesBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_indicesBuffer->create();
    m_indicesBuffer->bind();
    m_indicesBuffer->putData(m_indices);

    m_vertexArray->setIndexBuffer(m_indicesBuffer.get());
    // -------------------------------------------

    bindBuffersToVertexArray(m_vertexArray);
}

void SGCore::IMeshData::destroy() noexcept
{
    m_rhi = RHIData { };

    if(m_vertexArray) m_vertexArray->destroy();

    for(const auto& colorsBuf : m_verticesColorsBuffers)
    {
        if(colorsBuf)
        {
            colorsBuf->destroy();
        }
    }

    if(m_verticesBuffer) m_verticesBuffer->destroy();
    if(m_indicesBuffer) m_indicesBuffer->destroy();

    m_vertices.clear();
    m_vertices.shrink_to_fit();

    m_indices.clear();
    m_indices.shrink_to_fit();

    m_tmpVertexWeights.clear();
    m_tmpVertexWeights.shrink_to_fit();

    m_verticesColors.clear();
    m_verticesColors.shrink_to_fit();
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

SGCore::ECS::entity_t SGCore::IMeshData::addOnScene(const Ref<Scene>& scene) noexcept
{
    auto registry = scene->getECSRegistry();
    
    auto meshEntity = registry->create();

    registry->emplace<Pickable>(meshEntity);
    registry->emplace<Transform>(meshEntity);
    registry->emplace<EnableMeshPass>(meshEntity);

    Mesh& meshEntityMesh = registry->emplace<Mesh>(meshEntity);
    // maybe can load the ram
    meshEntityMesh.m_base.setMeshData(sharedFromBase<IMeshData>());

    if(m_material->m_transparencyType == MaterialTransparencyType::MAT_BLEND ||
       m_material->m_transparencyType == MaterialTransparencyType::MAT_MASK)
    {
        registry->emplace<TransparentEntityTag>(meshEntity);
    }
    else
    {
        registry->emplace<OpaqueEntityTag>(meshEntity);
    }

    auto& meshEntityBaseInfo = registry->get<EntityBaseInfo>(meshEntity);

    meshEntityBaseInfo.setRawName(m_name);

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
    m_physicalMesh = generatePhysicalMesh(m_vertices, m_indices);
}

namespace
{
    /// Puts a buffer's recorded attribute layout back the way it was. Only the records are touched:
    /// the vertex array state was already written by useAttributes(), and addAttribute does not
    /// itself touch the graphics API.
    void restoreAttributes(SGCore::IVertexBuffer& buffer, const std::vector<SGCore::IVertexBuffer::AttributeDesc>& saved) noexcept
    {
        buffer.clearAttributes();
        for(const auto& attribute : saved)
        {
            buffer.addAttribute(attribute.m_location, attribute.m_scalarsCount, attribute.m_dataType,
                                attribute.m_isNormalized, attribute.m_stride, attribute.m_offsetInStruct, attribute.m_divisor);
        }
    }
}

void SGCore::IMeshData::bindBuffersToVertexArray(const Ref<IVertexArray>& toVertexArray,
                                                 std::uint16_t vertexAttribsIDOffset) noexcept
{
    const auto maxVertexAttribsCount = GPUDeviceInfo::getMaxVertexAttribsCount();

    // This function defines the layout of the mesh buffers FOR THE VERTEX ARRAY BEING BOUND, and the
    // same buffers are bound into more than one of them: the mesh has its own array (offset 0) and
    // Instancing binds the very same buffers into its array at offset 7, behind the per-instance
    // transform attributes. addAttribute only appends and useAttributes replays the WHOLE recorded
    // list, so without clearing first the second call wrote both layouts into the instancing array
    // and locations 0..6 — the instance transform — were overwritten with vertex data. The geometry
    // came out torn on every backend (found 2026-08-22, the first time instancing was ever run).
    const auto savedVerticesAttributes = m_verticesBuffer->getAttributes();
    m_verticesBuffer->clearAttributes();

    toVertexArray->bind();

    // Register with the array being bound, not just with the mesh's own one. GL records the buffer
    // bindings in the vertex array object, so this was invisible there; the explicit backends have no
    // such object and rebuild the draw from IVertexArray::getVertexBuffers()/getIndexBuffer(), so for
    // the Instancing array they saw only the per-instance buffer — no positions, no indices, and the
    // draw silently produced nothing (no validation error either: VulkanPipelineState drops shader
    // inputs that nothing feeds). Both calls are idempotent for the mesh's own array.
    toVertexArray->addVertexBuffer(m_verticesBuffer.get());
    toVertexArray->setIndexBuffer(m_indicesBuffer.get());

    m_verticesBuffer->bind();

    // ---------------- adding positions -------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 0, 3, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), 0);
    // --------------------------------------------

    // ----- adding uv -------------------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 1, 3, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_uv));
    // --------------------------------------------

    // ---------- adding normals ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 2, 3, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_normal));
    // --------------------------------------------

    // ---------- adding tangents ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 3, 3, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_tangent));
    // --------------------------------------------

    // ---------- adding bitangents ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 4, 3, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_bitangent));
    // --------------------------------------------

    // ---------- adding bones ids ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 5, 4, SGGDataType::SGG_INT, false, sizeof(Vertex), offsetof(Vertex, m_bonesIDs));
    // --------------------------------------------

    // ---------- adding bones ids ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 6, 4, SGGDataType::SGG_INT, false, sizeof(Vertex), offsetof(Vertex, m_bonesIDs) + 4 * sizeof(std::int32_t));
    // --------------------------------------------

    // ---------- adding bones weights ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 7, 4, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_bonesWeights));
    // --------------------------------------------

    // ---------- preparing bones weights ---------------
    m_verticesBuffer->addAttribute(vertexAttribsIDOffset + 8, 4, SGGDataType::SGG_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_bonesWeights) + 4 * sizeof(float));
    // --------------------------------------------

    // enabling attributes
    m_verticesBuffer->useAttributes();

    // ---------------- adding vertices color -------

    for(size_t i = 0; i < m_verticesColors.size(); ++i)
    {
        if(vertexAttribsIDOffset + i + 9 >= maxVertexAttribsCount) break;

        m_verticesColorsBuffers[i]->bind();
        toVertexArray->addVertexBuffer(m_verticesColorsBuffers[i].get());

        const auto savedColorAttributes = m_verticesColorsBuffers[i]->getAttributes();
        m_verticesColorsBuffers[i]->clearAttributes();

        m_verticesColorsBuffers[i]->addAttribute(vertexAttribsIDOffset + i + 9, 4, SGGDataType::SGG_FLOAT, false, 4 * sizeof(float), 0);
        m_verticesColorsBuffers[i]->useAttributes();

        if(vertexAttribsIDOffset != 0)
        {
            toVertexArray->setBufferAttributes(m_verticesColorsBuffers[i].get(), m_verticesColorsBuffers[i]->getAttributes());
            restoreAttributes(*m_verticesColorsBuffers[i], savedColorAttributes);
        }
    }

    // -------------------------------------------------

    // ------ adding indices -------------------
    m_indicesBuffer->bind();
    // --------------------------------------------

    // A foreign vertex array borrows these buffers at its own location offset, but the recorded
    // layout has to keep describing the MESH: prepareMeshRHI builds the pipeline vertex input from
    // it lazily, and would otherwise pick up the shifted locations of whoever bound the mesh last.
    if(vertexAttribsIDOffset != 0)
    {
        // the shifted layout belongs to THIS array; the buffer keeps describing the mesh
        toVertexArray->setBufferAttributes(m_verticesBuffer.get(), m_verticesBuffer->getAttributes());
        restoreAttributes(*m_verticesBuffer, savedVerticesAttributes);
    }
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
