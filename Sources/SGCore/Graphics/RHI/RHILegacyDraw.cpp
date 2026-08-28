//
// Created by 8bitniksis on 20.08.2026.
//

#include "RHILegacyDraw.h"

#include <algorithm>

#include "RHIIndexBuffer.h"
#include "RHIVertexBuffer.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

bool SGCore::RHILegacyDraw::prepareMesh(IDevice& device, IMeshData& meshData) noexcept
{
    auto& rhi = meshData.m_rhi;
    if(rhi.m_prepared) return true;
    // A mesh may carry indices only: the post-process quads keep no vertices at all and their vertex
    // shader builds the quad from gl_VertexIndex, so there is nothing to upload and no vertex input
    // to declare. Refusing such a mesh silently dropped every FX draw on Vulkan (final frame black).
    if(meshData.m_vertices.empty() && meshData.m_indices.empty()) return false;

    rhi.m_vertexInput = VertexInputDesc { };
    rhi.m_vertexBuffer = nullptr;
    rhi.m_vertexColorsBuffers.clear();
    rhi.m_indexBuffer = nullptr;

    // write() on a device-local buffer stages and submits immediately, which is what a one-off mesh
    // upload wants (and it works outside a render pass, unlike a command list)
    GPUBufferDesc bufferDesc;
    bufferDesc.m_access = GPUMemoryAccess::SGG_DEVICE_LOCAL;

    if(!meshData.m_vertices.empty())
    {
        const auto& verticesBuffer = meshData.getVerticesBuffer();
        if(!verticesBuffer || verticesBuffer->getAttributes().empty()) return false;

        // vertex layout: slot 0 = interleaved Vertex, slots 1.. = per-vertex colour sets (as on GL46)
        rhi.m_vertexInput.m_slots.push_back({ 0, static_cast<std::uint32_t>(sizeof(Vertex)), false });
        for(const auto& attribute : verticesBuffer->getAttributes())
        {
            rhi.m_vertexInput.m_attributes.push_back({ attribute.m_location, 0, attribute.m_dataType,
                                                       static_cast<std::uint32_t>(attribute.m_scalarsCount),
                                                       static_cast<std::uint32_t>(attribute.m_offsetInStruct),
                                                       attribute.m_isNormalized });
        }

        const auto& colorsBuffers = meshData.getVerticesColorsBuffers();
        for(std::size_t i = 0; i < colorsBuffers.size(); ++i)
        {
            const auto& colorsBuffer = colorsBuffers[i];
            if(!colorsBuffer) continue;
            const auto slot = static_cast<std::uint32_t>(1 + i);
            std::uint32_t stride = 4 * sizeof(float);
            for(const auto& attribute : colorsBuffer->getAttributes())
            {
                stride = static_cast<std::uint32_t>(attribute.m_stride);
                rhi.m_vertexInput.m_attributes.push_back({ attribute.m_location, slot, attribute.m_dataType,
                                                           static_cast<std::uint32_t>(attribute.m_scalarsCount),
                                                           static_cast<std::uint32_t>(attribute.m_offsetInStruct),
                                                           attribute.m_isNormalized });
            }
            rhi.m_vertexInput.m_slots.push_back({ slot, stride, false });
        }

        bufferDesc.m_usage = GPUBufferUsage::SGG_VERTEX_BUFFER;
        bufferDesc.m_size = meshData.m_vertices.size() * sizeof(Vertex);
        bufferDesc.m_debugName = "mesh_vertices";
        rhi.m_vertexBuffer = device.createBuffer(bufferDesc);
        if(!rhi.m_vertexBuffer) return false;
        rhi.m_vertexBuffer->write(meshData.m_vertices.data(), bufferDesc.m_size);

        for(std::size_t i = 0; i < meshData.m_verticesColors.size(); ++i)
        {
            const auto& colors = meshData.m_verticesColors[i].m_colors;
            bufferDesc.m_size = colors.size() * sizeof(colors[0]);
            bufferDesc.m_debugName = "mesh_colors";
            auto buffer = bufferDesc.m_size > 0 ? device.createBuffer(bufferDesc) : nullptr;
            if(buffer) buffer->write(colors.data(), bufferDesc.m_size);
            rhi.m_vertexColorsBuffers.push_back(buffer);
        }
    }

    if(!meshData.m_indices.empty())
    {
        bufferDesc.m_usage = GPUBufferUsage::SGG_INDEX_BUFFER;
        bufferDesc.m_size = meshData.m_indices.size() * sizeof(std::uint32_t);
        bufferDesc.m_debugName = "mesh_indices";
        rhi.m_indexBuffer = device.createBuffer(bufferDesc);
        if(rhi.m_indexBuffer) rhi.m_indexBuffer->write(meshData.m_indices.data(), bufferDesc.m_size);
    }

    rhi.m_prepared = true;
    return true;
}

void SGCore::RHILegacyDraw::drawMesh(IDevice& device, ICommandList& commandList, RHILegacyShader& shader,
                                     const RenderState& renderState, const IMeshData& meshData,
                                     const MeshRenderState& meshRenderState) noexcept
{
    const auto& program = shader.getRHIProgram();
    if(!program || !program->isValid()) return;

    // the draw has to join the render pass the framebuffer facade has open: a pass lives inside one
    // command list, so a separate one could not draw into it
    if(!commandList.isRecording()) return;

    if(!prepareMesh(device, const_cast<IMeshData&>(meshData))) return;

    const auto& rhi = meshData.m_rhi;
    const bool indexed = meshRenderState.m_useIndices && rhi.m_indexBuffer;
    const auto count = static_cast<std::uint32_t>(indexed ? meshData.m_indices.size() : meshData.m_vertices.size());
    // a vertex-less mesh drawn without indices has nothing to rasterize
    if(count == 0) return;

    PipelineStateDesc pipelineDesc;
    pipelineDesc.m_program = program;
    pipelineDesc.m_renderState = renderState;
    pipelineDesc.m_blendingState = renderState.m_globalBlendingState;
    pipelineDesc.m_meshRenderState = meshRenderState;
    pipelineDesc.m_vertexInput = rhi.m_vertexInput;
    pipelineDesc.m_debugName = "legacy_mesh";
    auto pipeline = device.getOrCreatePipeline(pipelineDesc);

    commandList.bindPipeline(pipeline);
    commandList.bindDescriptorSet(0, shader.buildDescriptorSet());
    if(rhi.m_vertexBuffer) commandList.bindVertexBuffer(0, rhi.m_vertexBuffer);
    for(std::size_t i = 0; i < rhi.m_vertexColorsBuffers.size(); ++i)
    {
        if(rhi.m_vertexColorsBuffers[i]) commandList.bindVertexBuffer(static_cast<std::uint32_t>(1 + i), rhi.m_vertexColorsBuffers[i]);
    }

    if(indexed)
    {
        commandList.bindIndexBuffer(rhi.m_indexBuffer, SGIndexType::SGG_UINT32);
        commandList.drawIndexed(count);
    }
    else
    {
        commandList.draw(count);
    }
}

void SGCore::RHILegacyDraw::drawArray(IDevice& device, ICommandList& commandList, RHILegacyShader& shader,
                                      const RenderState& renderState, const Ref<IVertexArray>& vertexArray,
                                      const MeshRenderState& meshRenderState,
                                      int verticesCount, int indicesCount, int instancesCount) noexcept
{
    if(!vertexArray) return;

    const auto& program = shader.getRHIProgram();
    if(!program || !program->isValid()) return;
    if(!commandList.isRecording()) return;

    // callers pass whatever the batch currently holds, and an empty batch means nothing to rasterize
    if(meshRenderState.m_useIndices ? indicesCount <= 0 : verticesCount <= 0) return;

    // deterministic buffer order → stable slot numbers → stable pipeline cache key (as on GL46)
    std::vector<IVertexBuffer*> buffers(vertexArray->getVertexBuffers().begin(), vertexArray->getVertexBuffers().end());
    std::sort(buffers.begin(), buffers.end(), [](const IVertexBuffer* a, const IVertexBuffer* b) {
        return a->getNativeHandle() < b->getNativeHandle();
    });
    if(buffers.empty()) return;

    VertexInputDesc vertexInput;
    std::vector<Ref<IGPUBuffer>> slotBuffers;
    for(std::size_t slot = 0; slot < buffers.size(); ++slot)
    {
        auto* buffer = dynamic_cast<RHIVertexBuffer*>(buffers[slot]);
        if(!buffer || !buffer->getGPUBuffer()) return;

        // a buffer shared between arrays (mesh buffers inside an Instancing array) contributes a
        // different layout to each one, and only the array knows which
        const auto* arrayAttributes = vertexArray->getBufferAttributes(buffer);
        const auto& attributes = arrayAttributes ? *arrayAttributes : buffer->getAttributes();
        if(attributes.empty()) return;

        std::uint32_t stride = 0;
        bool perInstance = false;
        for(const auto& attribute : attributes)
        {
            stride = static_cast<std::uint32_t>(attribute.m_stride);
            perInstance = perInstance || attribute.m_divisor > 0;
            vertexInput.m_attributes.push_back({ attribute.m_location, static_cast<std::uint32_t>(slot), attribute.m_dataType,
                                                 static_cast<std::uint32_t>(attribute.m_scalarsCount),
                                                 static_cast<std::uint32_t>(attribute.m_offsetInStruct),
                                                 attribute.m_isNormalized });
        }
        vertexInput.m_slots.push_back({ static_cast<std::uint32_t>(slot), stride, perInstance });
        slotBuffers.push_back(buffer->getGPUBuffer());
    }

    Ref<IGPUBuffer> indexBuffer;
    if(meshRenderState.m_useIndices)
    {
        auto* indices = dynamic_cast<RHIIndexBuffer*>(vertexArray->getIndexBuffer());
        if(!indices || !indices->getGPUBuffer()) return;
        indexBuffer = indices->getGPUBuffer();
    }

    PipelineStateDesc pipelineDesc;
    pipelineDesc.m_program = program;
    pipelineDesc.m_renderState = renderState;
    pipelineDesc.m_blendingState = renderState.m_globalBlendingState;
    pipelineDesc.m_meshRenderState = meshRenderState;
    pipelineDesc.m_vertexInput = std::move(vertexInput);
    pipelineDesc.m_debugName = "legacy_array";
    auto pipeline = device.getOrCreatePipeline(pipelineDesc);

    commandList.bindPipeline(pipeline);
    commandList.bindDescriptorSet(0, shader.buildDescriptorSet());
    for(std::size_t slot = 0; slot < slotBuffers.size(); ++slot)
    {
        commandList.bindVertexBuffer(static_cast<std::uint32_t>(slot), slotBuffers[slot]);
    }

    if(meshRenderState.m_useIndices && indexBuffer)
    {
        commandList.bindIndexBuffer(indexBuffer, SGIndexType::SGG_UINT32);
        commandList.drawIndexed(static_cast<std::uint32_t>(indicesCount), static_cast<std::uint32_t>(std::max(instancesCount, 1)));
    }
    else
    {
        commandList.draw(static_cast<std::uint32_t>(verticesCount), static_cast<std::uint32_t>(std::max(instancesCount, 1)));
    }
}
