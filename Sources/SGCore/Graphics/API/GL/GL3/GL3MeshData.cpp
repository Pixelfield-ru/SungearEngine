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
                        bufferLayout->createVertexAttribute(i + 5,
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
