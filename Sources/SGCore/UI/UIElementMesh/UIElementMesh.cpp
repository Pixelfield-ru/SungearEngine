//
// Created by stuka on 15.02.2025.
//

#include "UIElementMesh.h"

#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IVertexAttribute.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IVertexArray.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::UI::UIElementMesh::prepare() noexcept
{
    m_vertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_vertexArray->create();
    m_vertexArray->bind();

    // ============================================================ preparing vertices

    m_verticesBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
            );
    m_verticesBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_verticesBuffer->create();
    m_verticesBuffer->bind();
    m_verticesBuffer->putData(m_vertices);
    m_vertexArray->addVertexBuffer(m_verticesBuffer.get());

    // ============================================================ preparing positions

    std::shared_ptr<IVertexBufferLayout> bufferLayout = std::shared_ptr<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());
    auto positionsAttrib = bufferLayout->createVertexAttribute(0,
                                                               "positionsAttribute",
                                                               SGGDataType::SGG_FLOAT3,
                                                               3,
                                                               false,
                                                               sizeof(UIVertex),
                                                               0,
                                                               0);

    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(positionsAttrib))->prepare()->enableAttributes();

    // ============================================================ preparing UV

    auto uvAttrib = bufferLayout->createVertexAttribute(1,
                                                        "UVAttribute",
                                                        SGGDataType::SGG_FLOAT2,
                                                        2,
                                                        false,
                                                        sizeof(UIVertex),
                                                        offsetof(UIVertex, m_uv),
                                                        0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(uvAttrib))->prepare()->enableAttributes();

    // ============================================================ preparing slice index

    auto sliceIndexAttrib = bufferLayout->createVertexAttribute(2,
                                                                "sliceIndexAttribute",
                                                                SGGDataType::SGG_INT,
                                                                1,
                                                                false,
                                                                sizeof(UIVertex),
                                                                offsetof(UIVertex, m_sliceIndex),
                                                                0);

    bufferLayout->reset();
    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(sliceIndexAttrib))->prepare()->enableAttributes();

    // ============================================================ preparing indices

    m_indicesBuffer = std::shared_ptr<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
    m_indicesBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_indicesBuffer->create();
    m_indicesBuffer->bind();
    m_indicesBuffer->putData(m_indices);
    m_vertexArray->setIndexBuffer(m_indicesBuffer.get());
}

void SGCore::UI::UIElementMesh::update() noexcept
{
    m_vertexArray->bind();

    if(m_verticesBuffer)
    {
        m_verticesBuffer->bind();
        m_verticesBuffer->subData(m_vertices, 0);
    }

    if(m_indicesBuffer)
    {
        m_indicesBuffer->bind();
        m_indicesBuffer->subData(m_indices, 0);
    }
}

SGCore::Ref<SGCore::IVertexArray> SGCore::UI::UIElementMesh::getVertexArray() const noexcept
{
    return m_vertexArray;
}
