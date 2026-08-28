//
// Created by 8bitniksis on 17.08.2026.
//

#include "ScreenBlit.h"

#include <cstdint>
#include <glm/vec3.hpp>

#include "IDevice.h"
#include "RHIShaderLoader.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Logger/Logger.h"

namespace
{
    struct QuadVertex
    {
        glm::vec3 m_position;
        glm::vec3 m_uv;
    };

    // same quad IRenderer::init() builds for the legacy path
    constexpr QuadVertex quad_vertices[4] = {
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
        { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
        { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    };
    constexpr std::uint32_t quad_indices[6] = { 0, 2, 1, 0, 3, 2 };
}

bool SGCore::ScreenBlit::init(IDevice& device) noexcept
{
    m_device = &device;
    m_ready = false;

    const auto loaded = RHIShaderLoader::load(device, "${enginePath}/Resources/sg_shaders/features/screen.sgshader");
    if(!loaded.ok())
    {
        SG_LOG_E("ScreenBlit: screen shader failed to load:\n{}", loaded.m_log);
        return false;
    }
    m_program = loaded.m_program;

    const auto& reflection = m_program->getReflection();
    const auto* textureBinding = reflection.findBinding("u_bufferToDisplay");
    const auto* legacyBlock = reflection.findBinding("SGLegacyUniforms_fragment");
    const auto* flipMember = reflection.findMember("SGLegacyUniforms_fragment", "u_flipOutput");
    if(!textureBinding || !legacyBlock || !flipMember)
    {
        SG_LOG_E("ScreenBlit: screen shader reflection lacks u_bufferToDisplay / u_flipOutput.");
        return false;
    }
    m_textureBinding = textureBinding->m_binding;
    m_flipOffset = flipMember->m_offset;

    GPUBufferDesc bufferDesc;
    bufferDesc.m_size = sizeof(quad_vertices);
    bufferDesc.m_usage = GPUBufferUsage::SGG_VERTEX_BUFFER;
    bufferDesc.m_access = GPUMemoryAccess::SGG_DEVICE_LOCAL;
    bufferDesc.m_debugName = "screen_blit_vbo";
    m_vertexBuffer = device.createBuffer(bufferDesc);

    bufferDesc.m_size = sizeof(quad_indices);
    bufferDesc.m_usage = GPUBufferUsage::SGG_INDEX_BUFFER;
    bufferDesc.m_debugName = "screen_blit_ibo";
    m_indexBuffer = device.createBuffer(bufferDesc);

    bufferDesc.m_size = legacyBlock->m_blockSize;
    bufferDesc.m_usage = GPUBufferUsage::SGG_UNIFORM_BUFFER;
    bufferDesc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    bufferDesc.m_debugName = "screen_blit_ubo";
    m_uniformBuffer = device.createBuffer(bufferDesc);

    m_descriptorSet = device.createDescriptorSet();
    m_descriptorSet->setUniformBuffer(legacyBlock->m_binding, m_uniformBuffer);

    PipelineStateDesc pipelineDesc;
    pipelineDesc.m_program = m_program;
    pipelineDesc.m_debugName = "screen_blit";
    pipelineDesc.m_renderState.m_useDepthTest = false;
    pipelineDesc.m_renderState.m_useStencilTest = false;
    pipelineDesc.m_blendingState.m_useBlending = false;
    pipelineDesc.m_meshRenderState.m_useFacesCulling = false;
    pipelineDesc.m_meshRenderState.m_useIndices = true;
    pipelineDesc.m_vertexInput.m_slots = { { 0, sizeof(QuadVertex), false } };
    pipelineDesc.m_vertexInput.m_attributes = {
        { 0, 0, SGGDataType::SGG_FLOAT, 3, offsetof(QuadVertex, m_position), false },
        { 1, 0, SGGDataType::SGG_FLOAT, 3, offsetof(QuadVertex, m_uv), false },
    };
    m_pipeline = device.getOrCreatePipeline(pipelineDesc);

    m_commandList = device.createCommandList();
    m_commandList->begin();
    m_commandList->uploadData(m_vertexBuffer, quad_vertices, sizeof(quad_vertices));
    m_commandList->uploadData(m_indexBuffer, quad_indices, sizeof(quad_indices));
    m_commandList->end();
    device.submit(m_commandList);

    m_ready = true;
    return true;
}

void SGCore::ScreenBlit::blit(const ITexture2D* texture, bool flipOutput, int x, int y, int width, int height) noexcept
{
    if(!m_ready || !texture || width <= 0 || height <= 0) return;

    if(!m_flipWritten || m_lastFlip != flipOutput)
    {
        // GLSL bool inside a std140 block occupies 4 bytes
        const std::uint32_t flip = flipOutput ? 1u : 0u;
        m_uniformBuffer->write(&flip, sizeof(flip), m_flipOffset);
        m_lastFlip = flipOutput;
        m_flipWritten = true;
    }

    // textures are still legacy objects: the descriptor set holds a non-owning reference
    // through a Ref with a no-op deleter
    m_descriptorSet->setTexture(m_textureBinding, Ref<ITexture2D>(const_cast<ITexture2D*>(texture), [](ITexture2D*) { }));

    m_commandList->begin();

    RenderPassBeginDesc pass;
    pass.m_frameBuffer = nullptr;
    pass.m_colorLoadOp = LoadOp::SGG_LOAD;
    pass.m_depthLoadOp = LoadOp::SGG_LOAD;
    m_commandList->beginRenderPass(pass);

    m_commandList->bindPipeline(m_pipeline);
    m_commandList->bindDescriptorSet(0, m_descriptorSet);
    m_commandList->bindVertexBuffer(0, m_vertexBuffer);
    m_commandList->bindIndexBuffer(m_indexBuffer, SGIndexType::SGG_UINT32);
    m_commandList->setViewport({ float(x), float(y), float(width), float(height) });
    m_commandList->drawIndexed(6);

    m_commandList->endRenderPass();
    m_commandList->end();
    m_device->submit(m_commandList);
}
