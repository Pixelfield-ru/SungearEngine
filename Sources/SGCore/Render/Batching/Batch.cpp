//
// Created by stuka on 15.06.2025.
//

#include "Batch.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexAttribute.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Render/Mesh.h"

SGCore::Batch::Batch() noexcept
{
    m_batchRenderState.m_drawMode = SGDrawMode::SGG_POINTS;
    m_batchRenderState.m_useIndices = false;

    m_fakeVertexArray = Ref<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_fakeVertexArray->create()->bind();

    m_fakeVerticesBuffer = Ref<IVertexBuffer>(CoreMain::getRenderer()->createVertexBuffer());

    m_fakeVerticesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_instanceTriangles);

    std::shared_ptr<IVertexBufferLayout> bufferLayout = Ref<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());
    auto instanceTriangleAttrib = bufferLayout->createVertexAttribute(0,
                                                                      "instanceTriangle",
                                                                      SGGDataType::SGG_INT2);

    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instanceTriangleAttrib))->prepare()->enableAttributes();

    // ==============================================================
    // creating true buffers

    m_verticesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_verticesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_verticesBuffer->m_isTextureBuffer = true;
    m_verticesBuffer->create<char>(reinterpret_cast<char*>(m_vertices.data()), m_vertices.size(), 1, 1,
                                   SGGColorInternalFormat::SGG_RGB32_FLOAT,
                                   SGGColorFormat::SGG_RGB);

    m_indicesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_indicesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_indicesBuffer->m_isTextureBuffer = true;
    m_indicesBuffer->create<char>(reinterpret_cast<char*>(m_indices.data()), m_indices.size(), 1, 1,
                                  SGGColorInternalFormat::SGG_R32_INT,
                                  SGGColorFormat::SGG_R);

    m_instancesTransformsBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_instancesTransformsBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_instancesTransformsBuffer->m_isTextureBuffer = true;
    m_instancesTransformsBuffer->create<char>(reinterpret_cast<char*>(m_transforms.data()), m_transforms.size(), 1, 1,
                                              SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                              SGGColorFormat::SGG_RGBA);
}

void SGCore::Batch::addEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept
{
    const auto* tmpMesh = fromRegistry.tryGet<Mesh>(entity);
    if(!tmpMesh) return;

    const Mesh& mesh = *tmpMesh;

    const size_t meshDataHash = mesh.m_base.getMeshData()->getHash();


}
