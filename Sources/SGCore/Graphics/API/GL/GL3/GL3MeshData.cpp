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
    m_positionsBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
            );
    m_positionsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_positions);

    std::shared_ptr<IVertexBufferLayout> bufferLayout = std::shared_ptr<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(0,
                                                        "positionsAttribute",
                                                        SGGDataType::SGG_FLOAT3))
                                                        )
            ->prepare()->enableAttributes();
    // --------------------------------------------

    // ----- preparing uv -------------------------
    m_uvBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
            );
    m_uvBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_uv);

    bufferLayout->reset();
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(1,
                                                        "UVAttribute",
                                                        SGGDataType::SGG_FLOAT3))
                                                        )
            ->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing normals ---------------
    m_normalsBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
            );
    m_normalsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_normals);

    bufferLayout->reset();
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(2,
                                                        "normalsAttribute",
                                                        SGGDataType::SGG_FLOAT3))
                                                        )
            ->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing tangents ---------------
    m_tangentsBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
    );
    m_tangentsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_tangents);

    bufferLayout->reset();
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(3,
                                                        "tangentsAttribute",
                                                        SGGDataType::SGG_FLOAT3))
            )
            ->prepare()->enableAttributes();
    // --------------------------------------------

    // ---------- preparing bitangents ---------------
    m_bitangentsBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
    );
    m_bitangentsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_bitangents);

    bufferLayout->reset();
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(4,
                                                        "bitangentsAttribute",
                                                        SGGDataType::SGG_FLOAT3))
            )
            ->prepare()->enableAttributes();
    // --------------------------------------------

    // ------ preparing indices -------------------
    m_indicesBuffer = std::shared_ptr<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
    m_indicesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_indices);
    // --------------------------------------------
}
