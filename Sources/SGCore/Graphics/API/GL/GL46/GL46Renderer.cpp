#include <SGCore/Logger/Logger.h>
#include "GL46Renderer.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"

#include "RHI/GL46Device.h"
#include "SGCore/Graphics/RHI/ScreenBlit.h"
#include "SGCore/Graphics/RHI/ICommandList.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "RHI/GL46GPUBuffer.h"
#include "GL46FrameBuffer.h"

#include <algorithm>

SGCore::GL46Renderer::~GL46Renderer() = default;

void SGCore::GL46Renderer::init() noexcept
{
    GL4Renderer::init();
    m_device = std::make_unique<GL46Device>(*this);
}

SGCore::IDevice* SGCore::GL46Renderer::getDevice() noexcept
{
    return m_device.get();
}

void SGCore::GL46Renderer::renderTextureOnScreen(const ITexture2D* texture, bool flipOutput,
                                                 int x, int y, int width, int height) noexcept
{
    // lazy: the screen shader asset is loadable only after the asset manager is up, which is
    // later than init()
    if(!m_screenBlitInitTried && m_device)
    {
        m_screenBlitInitTried = true;
        m_screenBlit = std::make_unique<ScreenBlit>();
        if(!m_screenBlit->init(*m_device))
        {
            SG_LOG_E("GL46Renderer: RHI ScreenBlit failed to initialize, falling back to the legacy screen quad.");
            m_screenBlit.reset();
        }
    }

    if(m_screenBlit)
    {
        m_screenBlit->blit(texture, flipOutput, x, y, width, height);
        return;
    }

    GL4Renderer::renderTextureOnScreen(texture, flipOutput, x, y, width, height);
}


bool SGCore::GL46Renderer::confirmSupport() noexcept
{
    const char* versionString = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const std::string glVersion = versionString ? versionString : "";

    // "4.6.0 NVIDIA ...", "4.6 (Core Profile) Mesa ..." — only the major.minor prefix matters
    if(!glVersion.starts_with("4.6"))
    {
        SG_LOG_E("OpenGL 4.6 is not supported! Reported version: '{}'\n{}", glVersion, SG_CURRENT_LOCATION_STR);
        return false;
    }

    return true;
}

SGCore::GL46Shader* SGCore::GL46Renderer::createShader()
{
    // same shader pipeline as GL4, only the GLSL version differs: the SG_GLSL4 define selects the
    // shader variant and is mandatory — without it every stage is preprocessed away
    auto* shader = new GL46Shader;
    shader->m_version = "460 core";
    shader->m_useRHIUniforms = true;
    shader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_GLSL4", ""));

    m_storage.m_shaders.insert(shader);

    return shader;
}

const std::shared_ptr<SGCore::GL46Renderer>& SGCore::GL46Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL46Renderer> s_instancePointer(new GL46Renderer);
    s_instancePointer->m_apiType = SG_API_TYPE_GL46;

    return s_instancePointer;
}

bool SGCore::GL46Renderer::prepareMeshRHI(IMeshData& meshData) noexcept
{
    auto& rhi = meshData.m_rhi;
    if(rhi.m_prepared) return true;
    if(!m_device || meshData.m_vertices.empty()) return false;

    const auto& verticesBuffer = meshData.getVerticesBuffer();
    if(!verticesBuffer || verticesBuffer->getAttributes().empty()) return false;

    // vertex layout: slot 0 = interleaved Vertex, slots 1.. = per-vertex color sets
    rhi.m_vertexInput = VertexInputDesc { };
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
        const std::uint32_t slot = static_cast<std::uint32_t>(1 + i);
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

    // buffers
    GPUBufferDesc bufferDesc;
    bufferDesc.m_access = GPUMemoryAccess::SGG_DEVICE_LOCAL;

    bufferDesc.m_usage = GPUBufferUsage::SGG_VERTEX_BUFFER;
    bufferDesc.m_size = meshData.m_vertices.size() * sizeof(Vertex);
    bufferDesc.m_debugName = "mesh_vertices";
    rhi.m_vertexBuffer = m_device->createBuffer(bufferDesc);

    rhi.m_vertexColorsBuffers.clear();
    for(std::size_t i = 0; i < meshData.m_verticesColors.size(); ++i)
    {
        const auto& colors = meshData.m_verticesColors[i].m_colors;
        bufferDesc.m_size = colors.size() * sizeof(colors[0]);
        bufferDesc.m_debugName = "mesh_colors";
        rhi.m_vertexColorsBuffers.push_back(bufferDesc.m_size > 0 ? m_device->createBuffer(bufferDesc) : nullptr);
    }

    if(!meshData.m_indices.empty())
    {
        bufferDesc.m_usage = GPUBufferUsage::SGG_INDEX_BUFFER;
        bufferDesc.m_size = meshData.m_indices.size() * sizeof(std::uint32_t);
        bufferDesc.m_debugName = "mesh_indices";
        rhi.m_indexBuffer = m_device->createBuffer(bufferDesc);
    }

    if(!m_meshCommandList) m_meshCommandList = m_device->createCommandList();
    m_meshCommandList->begin();
    m_meshCommandList->uploadData(rhi.m_vertexBuffer, meshData.m_vertices.data(), meshData.m_vertices.size() * sizeof(Vertex));
    for(std::size_t i = 0; i < rhi.m_vertexColorsBuffers.size(); ++i)
    {
        if(!rhi.m_vertexColorsBuffers[i]) continue;
        const auto& colors = meshData.m_verticesColors[i].m_colors;
        m_meshCommandList->uploadData(rhi.m_vertexColorsBuffers[i], colors.data(), colors.size() * sizeof(colors[0]));
    }
    if(rhi.m_indexBuffer)
    {
        m_meshCommandList->uploadData(rhi.m_indexBuffer, meshData.m_indices.data(), meshData.m_indices.size() * sizeof(std::uint32_t));
    }
    m_meshCommandList->end();
    m_device->submit(m_meshCommandList);

    rhi.m_prepared = true;
    return true;
}

void SGCore::GL46Renderer::renderMeshData(const IMeshData* meshData, const MeshRenderState& meshRenderState)
{
    if(!meshData) return;

    // RHI path needs the program of the shader the pass has bound; anything else → legacy
    Ref<IShaderProgram> program = m_currentLegacyShader ? m_currentLegacyShader->getRHIProgram() : nullptr;
    auto* mutableMesh = const_cast<IMeshData*>(meshData);
    if(!m_device || !program || !prepareMeshRHI(*mutableMesh))
    {
        GL4Renderer::renderMeshData(meshData, meshRenderState);
        return;
    }

    const auto& rhi = meshData->m_rhi;

    PipelineStateDesc pipelineDesc;
    pipelineDesc.m_program = program;
    pipelineDesc.m_renderState = m_cachedRenderState;
    pipelineDesc.m_blendingState = m_cachedRenderState.m_globalBlendingState;
    pipelineDesc.m_meshRenderState = meshRenderState;
    pipelineDesc.m_vertexInput = rhi.m_vertexInput;
    auto pipeline = m_device->getOrCreatePipeline(pipelineDesc);

    m_meshCommandList->begin();
    m_meshCommandList->bindPipeline(pipeline);
    m_meshCommandList->bindVertexBuffer(0, rhi.m_vertexBuffer);
    for(std::size_t i = 0; i < rhi.m_vertexColorsBuffers.size(); ++i)
    {
        if(rhi.m_vertexColorsBuffers[i]) m_meshCommandList->bindVertexBuffer(static_cast<std::uint32_t>(1 + i), rhi.m_vertexColorsBuffers[i]);
    }

    if(meshRenderState.m_useIndices && rhi.m_indexBuffer)
    {
        m_meshCommandList->bindIndexBuffer(rhi.m_indexBuffer, SGIndexType::SGG_UINT32);
        m_meshCommandList->drawIndexed(static_cast<std::uint32_t>(meshData->m_indices.size()));
    }
    else
    {
        m_meshCommandList->draw(static_cast<std::uint32_t>(meshData->m_vertices.size()));
    }
    m_meshCommandList->end();
    m_device->submit(m_meshCommandList);
}

bool SGCore::GL46Renderer::drawLegacyArrayThroughRHI(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                                     int verticesCount, int indicesCount, int instancesCount) noexcept
{
    Ref<IShaderProgram> program = m_currentLegacyShader ? m_currentLegacyShader->getRHIProgram() : nullptr;
    if(!m_device || !program || !vertexArray) return false;

    // deterministic buffer order → stable slot numbers → stable pipeline cache key
    std::vector<IVertexBuffer*> buffers(vertexArray->getVertexBuffers().begin(), vertexArray->getVertexBuffers().end());
    std::sort(buffers.begin(), buffers.end(), [](const IVertexBuffer* a, const IVertexBuffer* b) {
        return a->getNativeHandle() < b->getNativeHandle();
    });
    if(buffers.empty()) return false;

    auto wrap = [this](std::uintptr_t handle, std::uint64_t size, const char* name) -> Ref<IGPUBuffer>
    {
        auto it = m_wrappedBuffers.find(handle);
        if(it != m_wrappedBuffers.end()) return it->second;
        auto wrapped = MakeRef<GL46GPUBuffer>(static_cast<GLuint>(handle), size, name);
        m_wrappedBuffers.emplace(handle, wrapped);
        return wrapped;
    };

    VertexInputDesc vertexInput;
    std::vector<Ref<IGPUBuffer>> slotBuffers;
    for(std::size_t slot = 0; slot < buffers.size(); ++slot)
    {
        const auto* buffer = buffers[slot];
        if(buffer->getNativeHandle() == 0) return false;

        // a buffer shared between vertex arrays contributes a different layout to each (the mesh
        // buffers inside an Instancing array sit behind the per-instance transform); only the array
        // knows which one applies here
        const auto* arrayAttributes = vertexArray->getBufferAttributes(buffer);
        const auto& bufferAttributes = arrayAttributes ? *arrayAttributes : buffer->getAttributes();
        if(bufferAttributes.empty()) return false;

        std::uint32_t stride = 0;
        bool perInstance = false;
        for(const auto& attribute : bufferAttributes)
        {
            stride = static_cast<std::uint32_t>(attribute.m_stride);
            perInstance = perInstance || attribute.m_divisor > 0;
            vertexInput.m_attributes.push_back({ attribute.m_location, static_cast<std::uint32_t>(slot), attribute.m_dataType,
                                                 static_cast<std::uint32_t>(attribute.m_scalarsCount),
                                                 static_cast<std::uint32_t>(attribute.m_offsetInStruct),
                                                 attribute.m_isNormalized });
        }
        vertexInput.m_slots.push_back({ static_cast<std::uint32_t>(slot), stride, perInstance });
        slotBuffers.push_back(wrap(buffer->getNativeHandle(), buffer->getData().size(), "legacy_vbo"));
    }

    Ref<IGPUBuffer> indexBuffer;
    if(meshRenderState.m_useIndices)
    {
        const auto* indices = vertexArray->getIndexBuffer();
        if(!indices || indices->getNativeHandle() == 0) return false;
        indexBuffer = wrap(indices->getNativeHandle(), std::uint64_t(indicesCount) * sizeof(std::uint32_t), "legacy_ibo");
    }

    PipelineStateDesc pipelineDesc;
    pipelineDesc.m_program = program;
    pipelineDesc.m_renderState = m_cachedRenderState;
    pipelineDesc.m_blendingState = m_cachedRenderState.m_globalBlendingState;
    pipelineDesc.m_meshRenderState = meshRenderState;
    pipelineDesc.m_vertexInput = std::move(vertexInput);
    auto pipeline = m_device->getOrCreatePipeline(pipelineDesc);

    if(!m_meshCommandList) m_meshCommandList = m_device->createCommandList();
    m_meshCommandList->begin();
    m_meshCommandList->bindPipeline(pipeline);
    for(std::size_t slot = 0; slot < slotBuffers.size(); ++slot)
    {
        m_meshCommandList->bindVertexBuffer(static_cast<std::uint32_t>(slot), slotBuffers[slot]);
    }
    if(indexBuffer)
    {
        m_meshCommandList->bindIndexBuffer(indexBuffer, SGIndexType::SGG_UINT32);
        m_meshCommandList->drawIndexed(static_cast<std::uint32_t>(indicesCount), static_cast<std::uint32_t>(instancesCount));
    }
    else
    {
        m_meshCommandList->draw(static_cast<std::uint32_t>(verticesCount), static_cast<std::uint32_t>(instancesCount));
    }
    m_meshCommandList->end();
    m_device->submit(m_meshCommandList);
    return true;
}

void SGCore::GL46Renderer::renderArray(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                       const int& verticesCount, const int& indicesCount)
{
    if(drawLegacyArrayThroughRHI(vertexArray, meshRenderState, verticesCount, indicesCount, 1)) return;
    GL4Renderer::renderArray(vertexArray, meshRenderState, verticesCount, indicesCount);
}

void SGCore::GL46Renderer::renderArrayInstanced(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                                const int& verticesCount, const int& indicesCount, const int& instancesCount)
{
    if(drawLegacyArrayThroughRHI(vertexArray, meshRenderState, verticesCount, indicesCount, instancesCount)) return;
    GL4Renderer::renderArrayInstanced(vertexArray, meshRenderState, verticesCount, indicesCount, instancesCount);
}

SGCore::GL4FrameBuffer* SGCore::GL46Renderer::createFrameBuffer()
{
    auto* frameBuffer = new GL46FrameBuffer;
    m_storage.m_frameBuffers.insert(frameBuffer);
    return frameBuffer;
}

SGCore::ICommandList* SGCore::GL46Renderer::getFrameBufferCommandList() noexcept
{
    if(!m_device) return nullptr;
    if(!m_frameBufferCommandList) m_frameBufferCommandList = m_device->createCommandList();
    return m_frameBufferCommandList.get();
}
