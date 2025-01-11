#include "GL3MeshData.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"

#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"

// todo: define names
void SGCore::GL3MeshData::prepare()
{
    m_vertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_vertexArray->create()->bind();

    // ---------------- preparing positions -------
    m_verticesBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
            );
    m_verticesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_vertices);

    std::shared_ptr<IVertexBufferLayout> bufferLayout = std::shared_ptr<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());
    auto positionsAttrib = bufferLayout->createVertexAttribute(0,
                                                               "positionsAttribute",
                                                               SGGDataType::SGG_FLOAT3,
                                                               3,
                                                               false,
                                                               sizeof(Vertex),
                                                               0,
                                                               0);

    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(positionsAttrib))->prepare()->enableAttributes();
    // --------------------------------------------

    // ----- preparing uv -------------------------
    auto uvAttrib = bufferLayout->createVertexAttribute(1,
                                                        "UVAttribute",
                                                        SGGDataType::SGG_FLOAT3,
                                                        3,
                                                        false,
                                                        sizeof(Vertex),
                                                        offsetof(Vertex, m_uv),
                                                        0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(uvAttrib))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing normals ---------------
    auto normalAttrib = bufferLayout->createVertexAttribute(2,
                                                            "normalsAttribute",
                                                            SGGDataType::SGG_FLOAT3,
                                                            3,
                                                            false,
                                                            sizeof(Vertex),
                                                            offsetof(Vertex, m_normal),
                                                            0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(normalAttrib))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing tangents ---------------
    auto tangentAttrib = bufferLayout->createVertexAttribute(3,
                                                             "tangentsAttribute",
                                                             SGGDataType::SGG_FLOAT3,
                                                             3,
                                                             false,
                                                             sizeof(Vertex),
                                                             offsetof(Vertex, m_tangent),
                                                             0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(tangentAttrib))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing bitangents ---------------
    auto bitangentAttrib = bufferLayout->createVertexAttribute(4,
                                                               "bitangentsAttribute",
                                                               SGGDataType::SGG_FLOAT3,
                                                               3,
                                                               false,
                                                               sizeof(Vertex),
                                                               offsetof(Vertex, m_bitangent),
                                                               0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(bitangentAttrib))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing bones ids ---------------
    auto bonesIDsAttrib0 = bufferLayout->createVertexAttribute(5,
                                                               "bonesIDsAttribute0",
                                                               SGGDataType::SGG_INT4,
                                                               4,
                                                               false,
                                                               sizeof(Vertex),
                                                               offsetof(Vertex, m_bonesIDs),
                                                               0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(bonesIDsAttrib0))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing bones ids ---------------
    auto bonesIDsAttrib1 = bufferLayout->createVertexAttribute(6,
                                                               "bonesIDsAttribute1",
                                                               SGGDataType::SGG_INT4,
                                                               4,
                                                               false,
                                                               sizeof(Vertex),
                                                               offsetof(Vertex, m_bonesIDs) + 4 * sizeof(std::int32_t),
                                                               0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(bonesIDsAttrib1))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing bones ids ---------------
    auto bonesWeightsAttrib0 = bufferLayout->createVertexAttribute(7,
                                                                   "bonesWeightsAttribute0",
                                                                   SGGDataType::SGG_FLOAT4,
                                                                   4,
                                                                   false,
                                                                   sizeof(Vertex),
                                                                   offsetof(Vertex, m_bonesWeights),
                                                                   0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(bonesWeightsAttrib0))->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing bones ids ---------------
    auto bonesWeightsAttrib1 = bufferLayout->createVertexAttribute(8,
                                                                   "bonesWeightsAttribute",
                                                                   SGGDataType::SGG_FLOAT4,
                                                                   4,
                                                                   false,
                                                                   sizeof(Vertex),
                                                                   offsetof(Vertex, m_bonesWeights) + 4 * sizeof(float),
                                                                   0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(bonesWeightsAttrib1))->prepare()->enableAttributes();
    // --------------------------------------------

    std::cout << "created mesh with " << m_vertices.size() << " vertices." << std::endl;

    // ---------------- preparing vertices color -------

    m_verticesColorsBuffers.clear();

    for(size_t i = 0; i < m_verticesColors.size(); ++i)
    {
        auto verticesColorsBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        verticesColorsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_verticesColors[i].m_colors);

        bufferLayout->reset();
        bufferLayout
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(i + 9,
                                                            "vertexColor" + std::to_string(i) + "Attribute",
                                                            SGGDataType::SGG_FLOAT4))
                )
                ->prepare()->enableAttributes();

        m_verticesColorsBuffers.push_back(verticesColorsBuffer);
    }

    // -------------------------------------------------

    // ------ preparing indices -------------------
    m_indicesBuffer = std::shared_ptr<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
    m_indicesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_indices);
    // --------------------------------------------
}
