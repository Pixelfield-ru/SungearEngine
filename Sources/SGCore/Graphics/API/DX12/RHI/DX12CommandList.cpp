//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12CommandList.h"

#if defined(_WIN32)

#include <algorithm>
#include <cstring>

#include "SGCore/Graphics/API/DX12/DX12Texture2D.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Logger/Logger.h"
#include "DX12DescriptorSet.h"
#include "DX12GPUBuffer.h"
#include "DX12ShaderProgram.h"

SGCore::DX12CommandList::DX12CommandList(DX12Device& device) noexcept : m_device(device)
{
    m_debugName = "command_list";
}

SGCore::DX12CommandList::~DX12CommandList()
{
    // command contexts of an un-submitted recording go back to the device through an empty submit,
    // the same way VulkanCommandList returns its buffers
    if(!m_submitted && m_context.isValid())
    {
        if(m_recording)
        {
            m_context.m_list->Close();
            if(m_uploadContext.isValid()) m_uploadContext.m_list->Close();
        }
        DX12Submission drop;
        drop.m_contexts.push_back(std::move(m_context));
        if(m_uploadContext.isValid()) drop.m_contexts.push_back(std::move(m_uploadContext));
        m_device.submitRaw(std::move(drop));
    }
}

void SGCore::DX12CommandList::resetState() noexcept
{
    m_inPass = false;
    m_targetIsSwapchain = false;
    m_touchedSwapchain = false;
    m_passFormats = { };
    m_passWidth = 0;
    m_passHeight = 0;
    m_passColorTextures.clear();
    m_passColorViews.clear();
    m_passDepthTexture.reset();
    m_pipeline.reset();
    m_boundPipeline = nullptr;
    m_boundRootSignature = nullptr;
    for(auto& slot : m_sets) slot = { };
    m_viewportExplicit = false;
    m_scissorExplicit = false;
    m_viewportDirty = true;
}

void SGCore::DX12CommandList::begin() noexcept
{
    // Someone else is still recording into this list: finish and SUBMIT that work rather than drop
    // it. Resetting a command list discards everything recorded so far, and on Vulkan exactly this
    // silently emptied the geometry framebuffer of the smoke scene.
    if(m_recording) end();
    if(m_ended && !m_submitted) flushRecordedWork();

    m_context = m_device.acquireCommandContext();
    m_uploadContext = m_device.acquireCommandContext();
    m_submission = { };
    if(!m_context.isValid()) return;

    m_submitted = false;
    m_ended = false;
    resetState();

    // the heaps the descriptor tables are written into have to be set once per command list, and
    // only one heap of each type can be set at a time
    if(auto* views = m_device.getViewRing().getHeap())
    {
        ID3D12DescriptorHeap* heaps[] = { views, m_device.getSamplerRing().getHeap() };
        m_context.m_list->SetDescriptorHeaps(heaps[1] ? 2 : 1, heaps);
    }
    m_recording = true;
}

void SGCore::DX12CommandList::end() noexcept
{
    if(!m_recording) return;
    if(m_inPass) endRenderPass();

    if(m_uploadContext.isValid()) SG_DX_CHECK(m_uploadContext.m_list->Close());
    SG_DX_CHECK(m_context.m_list->Close());
    m_recording = false;
    m_ended = true;
}

void SGCore::DX12CommandList::flushRecordedWork() noexcept
{
    auto submission = takeSubmission();
    if(submission.m_contexts.empty()) return;
    m_device.submitRaw(std::move(submission));
}

SGCore::DX12Submission SGCore::DX12CommandList::takeSubmission() noexcept
{
    DX12Submission submission = std::move(m_submission);
    m_submission = { };
    if(!m_ended || m_submitted) return { };

    submission.m_contexts.clear();
    // uploads run before anything the main list reads, so they are executed first; an unused upload
    // list still has to be recycled, and an empty one costs nothing
    if(m_uploadContext.isValid()) submission.m_contexts.push_back(std::move(m_uploadContext));
    submission.m_contexts.push_back(std::move(m_context));
    m_submitted = true;
    return submission;
}

// ---- render pass -------------------------------------------------------------------------------

void SGCore::DX12CommandList::beginRenderPass(const RenderPassBeginDesc& desc) noexcept
{
    if(!m_recording) begin();
    if(!m_recording) return;
    if(m_inPass) endRenderPass();

    m_passColorTextures.clear();
    m_passColorViews.clear();
    m_passDepthTexture.reset();
    m_passFormats = { };
    m_targetIsSwapchain = desc.m_frameBuffer == nullptr;

    if(m_targetIsSwapchain)
    {
        auto& swapchain = m_device.getDX12Swapchain();
        if(!swapchain.ensureAcquired())
        {
            SG_LOG_E("DX12CommandList: can not begin a render pass on the window, no back buffer is available.");
            return;
        }
        auto texture = swapchain.getCurrentTexture();
        m_passColorTextures.push_back(texture);
        m_passWidth = texture->getWidth();
        m_passHeight = texture->getHeight();
        m_touchedSwapchain = true;
    }
    else
    {
        std::vector<SGFrameBufferAttachmentType> colorTypes = desc.m_colorAttachments;
        if(colorTypes.empty())
        {
            for(const auto& [type, texture] : desc.m_frameBuffer->getAttachments())
            {
                if(isColorAttachment(type)) colorTypes.push_back(type);
            }
            std::sort(colorTypes.begin(), colorTypes.end());
        }
        for(const auto type : colorTypes)
        {
            const auto attachment = desc.m_frameBuffer->getAttachment(type);
            const auto* dxAttachment = dynamic_cast<DX12Texture2D*>(attachment.get());
            if(!dxAttachment || !dxAttachment->getDX12Texture())
            {
                SG_LOG_E("DX12CommandList: framebuffer attachment {} is not a DX12 texture.", static_cast<int>(type));
                continue;
            }
            m_passColorTextures.push_back(dxAttachment->getDX12Texture());
        }
        for(const auto& [type, texture] : desc.m_frameBuffer->getAttachments())
        {
            if(!isDepthAttachment(type) && !isDepthStencilAttachment(type)) continue;
            const auto* dxAttachment = dynamic_cast<DX12Texture2D*>(texture.get());
            if(dxAttachment && dxAttachment->getDX12Texture())
            {
                m_passDepthTexture = dxAttachment->getDX12Texture();
                break;
            }
        }

        m_passWidth = static_cast<std::uint32_t>(std::max(0, desc.m_frameBuffer->getWidth()));
        m_passHeight = static_cast<std::uint32_t>(std::max(0, desc.m_frameBuffer->getHeight()));
        if(!m_passColorTextures.empty())
        {
            m_passWidth = m_passColorTextures.front()->getWidth();
            m_passHeight = m_passColorTextures.front()->getHeight();
        }
        else if(m_passDepthTexture)
        {
            m_passWidth = m_passDepthTexture->getWidth();
            m_passHeight = m_passDepthTexture->getHeight();
        }
    }

    if(m_passColorTextures.empty() && !m_passDepthTexture)
    {
        SG_LOG_E("DX12CommandList: render pass without attachments.");
        return;
    }

    for(const auto& texture : m_passColorTextures)
    {
        texture->recordTransition(m_context.m_list.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_passFormats.m_colorFormats.push_back(texture->getFormat());
        m_passColorViews.push_back(texture->getRTV());
    }
    if(m_passDepthTexture)
    {
        m_passDepthTexture->recordTransition(m_context.m_list.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
        m_passFormats.m_depthFormat = m_passDepthTexture->getFormat();
    }
    m_passFormats.m_viewportFlipped = !m_targetIsSwapchain;

    const D3D12_CPU_DESCRIPTOR_HANDLE depthView = m_passDepthTexture ? m_passDepthTexture->getDSV() : D3D12_CPU_DESCRIPTOR_HANDLE { };
    m_context.m_list->OMSetRenderTargets(static_cast<UINT>(m_passColorViews.size()), m_passColorViews.data(), FALSE,
                                         m_passDepthTexture ? &depthView : nullptr);

    if(desc.m_colorLoadOp == LoadOp::SGG_CLEAR)
    {
        const float clearColor[4] = { desc.m_clearColor.r, desc.m_clearColor.g, desc.m_clearColor.b, desc.m_clearColor.a };
        for(const auto view : m_passColorViews)
        {
            m_context.m_list->ClearRenderTargetView(view, clearColor, 0, nullptr);
        }
    }
    if(m_passDepthTexture && desc.m_depthLoadOp == LoadOp::SGG_CLEAR)
    {
        m_context.m_list->ClearDepthStencilView(depthView, D3D12_CLEAR_FLAG_DEPTH, desc.m_clearDepth, 0, 0, nullptr);
    }

    m_inPass = true;

    // pipeline variants depend on the pass formats: rebind at the next draw
    m_boundPipeline = nullptr;
    m_boundRootSignature = nullptr;
    m_viewportExplicit = false;
    m_scissorExplicit = false;
    m_viewportDirty = true;
    for(auto& slot : m_sets) slot.m_materialized = false;
    for(const auto& texture : m_passColorTextures) m_submission.m_keepAlive.push_back(texture);
    if(m_passDepthTexture) m_submission.m_keepAlive.push_back(m_passDepthTexture);
}

void SGCore::DX12CommandList::endRenderPass() noexcept
{
    if(!m_inPass) return;
    m_inPass = false;

    // offscreen targets rest in the shader-read state so a later pass can sample them without a
    // barrier of its own; the back buffer is transitioned by present()
    if(!m_targetIsSwapchain)
    {
        for(const auto& texture : m_passColorTextures)
        {
            texture->recordTransition(m_context.m_list.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        }
        if(m_passDepthTexture)
        {
            m_passDepthTexture->recordTransition(m_context.m_list.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        }
    }
    m_passColorTextures.clear();
    m_passColorViews.clear();
    m_passDepthTexture.reset();
}

// ---- state ------------------------------------------------------------------------------------

void SGCore::DX12CommandList::bindPipeline(const Ref<IPipelineState>& pipeline) noexcept
{
    m_pipeline = std::static_pointer_cast<DX12PipelineState>(pipeline);
    m_boundPipeline = nullptr;
    if(m_pipeline) m_submission.m_keepAlive.push_back(m_pipeline);
}

void SGCore::DX12CommandList::bindDescriptorSet(std::uint32_t setIndex, const Ref<IDescriptorSet>& set) noexcept
{
    if(setIndex >= max_descriptor_sets)
    {
        SG_LOG_E("DX12CommandList: descriptor set index {} exceeds the supported {} sets.", setIndex, max_descriptor_sets);
        return;
    }
    auto& slot = m_sets[setIndex];
    slot.m_set = std::static_pointer_cast<DX12DescriptorSet>(set);
    slot.m_materializedVersion = ~0ull;
    slot.m_materialized = false;
    if(slot.m_set) m_submission.m_keepAlive.push_back(slot.m_set);
}

void SGCore::DX12CommandList::pushConstants(const void* /*data*/, std::uint32_t /*size*/, std::uint32_t /*offset*/) noexcept
{
    // root constants are mapped together with the HLSL translation (task 3.4); no engine pass uses
    // push constants, which is why the GL backend left them unimplemented as well
    static bool s_reported = false;
    if(s_reported) return;
    s_reported = true;
    SG_LOG_W("DX12CommandList: push constants are not implemented yet (stage 3, task 3.4).");
}

void SGCore::DX12CommandList::bindVertexBuffer(std::uint32_t slot, const Ref<IGPUBuffer>& buffer, std::uint64_t offset) noexcept
{
    const auto* dxBuffer = static_cast<const DX12GPUBuffer*>(buffer.get());
    if(!dxBuffer || !dxBuffer->isValid() || !m_recording) return;

    // the stride belongs to the vertex layout, which lives in the pipeline: the view can only be
    // filled once the pipeline of this draw is known
    if(!m_pipeline)
    {
        SG_LOG_E("DX12CommandList: bindVertexBuffer before bindPipeline; the stride comes from the pipeline layout.");
        return;
    }

    std::uint32_t stride = 0;
    for(const auto& slotDesc : m_pipeline->getDesc().m_vertexInput.m_slots)
    {
        if(slotDesc.m_slot != slot) continue;
        stride = slotDesc.m_stride;
        break;
    }

    D3D12_VERTEX_BUFFER_VIEW view { };
    view.BufferLocation = dxBuffer->getGPUAddress() + offset;
    view.SizeInBytes = static_cast<UINT>(dxBuffer->getAllocatedSize() - offset);
    view.StrideInBytes = stride;
    m_context.m_list->IASetVertexBuffers(slot, 1, &view);
    m_submission.m_keepAlive.push_back(buffer);
}

void SGCore::DX12CommandList::bindIndexBuffer(const Ref<IGPUBuffer>& buffer, SGIndexType indexType, std::uint64_t offset) noexcept
{
    const auto* dxBuffer = static_cast<const DX12GPUBuffer*>(buffer.get());
    if(!dxBuffer || !dxBuffer->isValid() || !m_recording) return;

    D3D12_INDEX_BUFFER_VIEW view { };
    view.BufferLocation = dxBuffer->getGPUAddress() + offset;
    view.SizeInBytes = static_cast<UINT>(dxBuffer->getAllocatedSize() - offset);
    view.Format = indexType == SGIndexType::SGG_UINT16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_context.m_list->IASetIndexBuffer(&view);
    m_submission.m_keepAlive.push_back(buffer);
}

void SGCore::DX12CommandList::setViewport(const Viewport& viewport) noexcept
{
    m_viewport = viewport;
    m_viewportExplicit = true;
    m_viewportDirty = true;
}

void SGCore::DX12CommandList::setScissor(const Scissor& scissor) noexcept
{
    m_scissor = scissor;
    m_scissorExplicit = true;
    m_viewportDirty = true;
}

void SGCore::DX12CommandList::applyViewportScissor() noexcept
{
    Viewport viewport = m_viewport;
    if(!m_viewportExplicit)
    {
        viewport = { 0.0f, 0.0f, static_cast<float>(m_passWidth), static_cast<float>(m_passHeight), 0.0f, 1.0f };
    }
    Scissor scissor = m_scissor;
    if(!m_scissorExplicit)
    {
        // GL default: no scissor -> the viewport rectangle
        scissor = { static_cast<std::int32_t>(viewport.m_x), static_cast<std::int32_t>(viewport.m_y),
                    static_cast<std::int32_t>(viewport.m_width), static_cast<std::int32_t>(viewport.m_height) };
    }

    // Offscreen passes are flipped, window ones are not — the MIRROR of the Vulkan rule, and copying
    // Vulkan's here was a real bug (every offscreen buffer came out upside down; measured 2026-08-22).
    // Vulkan clips with +y down, so its NDC y = -1 lands in row 0 exactly as GL's does and offscreen
    // needs no flip. D3D clips with +y UP but keeps row 0 at the top, so NDC y = +1 lands in row 0 —
    // the opposite of GL. Offscreen targets therefore need the flip to keep the GL memory layout
    // (readback and every uv the engine computes by hand assume it), and the window needs none: the
    // swapchain already presents row 0 at the top of the screen.
    const bool flipped = !m_targetIsSwapchain;
    const float targetHeight = static_cast<float>(m_passHeight);

    D3D12_VIEWPORT dxViewport { };
    dxViewport.TopLeftX = viewport.m_x;
    dxViewport.Width = viewport.m_width;
    dxViewport.MinDepth = viewport.m_minDepth;
    dxViewport.MaxDepth = viewport.m_maxDepth;

    D3D12_RECT rect { };
    rect.left = std::max(scissor.m_x, 0);
    rect.right = std::min<std::int32_t>(scissor.m_x + scissor.m_width, static_cast<std::int32_t>(m_passWidth));

    // Two separate conversions, and mixing them up costs a whole render pass: (1) the rectangle is
    // given GL-style from the bottom edge and D3D wants it from the top — that applies to EVERY pass,
    // window included (without it the screen blit lands at the top of the window and reads back as
    // zeros); (2) the mirroring, which is the negative height and only the offscreen passes want it.
    const float topEdge = targetHeight - viewport.m_y - viewport.m_height;
    dxViewport.TopLeftY = flipped ? topEdge + viewport.m_height : topEdge;
    dxViewport.Height = flipped ? -viewport.m_height : viewport.m_height;

    rect.top = std::max<std::int32_t>(static_cast<std::int32_t>(targetHeight) - scissor.m_y - scissor.m_height, 0);
    rect.bottom = std::min<std::int32_t>(static_cast<std::int32_t>(targetHeight) - scissor.m_y, static_cast<std::int32_t>(m_passHeight));
    if(rect.right < rect.left) rect.right = rect.left;
    if(rect.bottom < rect.top) rect.bottom = rect.top;

    m_context.m_list->RSSetViewports(1, &dxViewport);
    m_context.m_list->RSSetScissorRects(1, &rect);
    m_viewportDirty = false;
}

bool SGCore::DX12CommandList::materializeSet(SetSlot& slot, std::uint32_t setIndex) noexcept
{
    const auto* program = m_pipeline->getProgram();
    if(!program) return false;

    bool bound = false;
    auto* device = m_device.getContext().m_device.Get();

    for(const bool sampler : { false, true })
    {
        const auto* table = program->findRootTable(setIndex, sampler);
        if(!table || table->m_descriptorsCount == 0) continue;

        auto& ring = sampler ? m_device.getSamplerRing() : m_device.getViewRing();
        const std::uint32_t start = ring.allocateRange(table->m_descriptorsCount);
        if(start == DX12DescriptorHeap::invalid_index)
        {
            SG_LOG_E("DX12CommandList: the shader-visible {} heap can not fit a table of {} descriptors.",
                     sampler ? "sampler" : "view", table->m_descriptorsCount);
            continue;
        }

        for(const auto& entry : table->m_entries)
        {
            for(std::uint32_t element = 0; element < entry.m_count; ++element)
            {
                const auto handle = ring.cpuHandle(start + entry.m_offsetInTable + element);
                const auto found = slot.m_set->getEntries().find({ entry.m_binding, element });
                const DX12DescriptorSet::Entry* bindingEntry = found != slot.m_set->getEntries().end() ? &found->second : nullptr;

                auto texture = bindingEntry ? resolveTexture(*bindingEntry) : nullptr;

                if(sampler)
                {
                    // a sampler comes from the texture bound to the same binding: the engine has no
                    // sampler objects of its own (GL and the SGSL shaders use combined samplers)
                    const auto samplerDesc = texture ? texture->getSamplerDesc() : DX12Texture::defaultSamplerDesc();
                    device->CreateSampler(&samplerDesc, handle);
                    continue;
                }

                const auto* buffer = bindingEntry ? static_cast<const DX12GPUBuffer*>(bindingEntry->m_buffer.get()) : nullptr;

                if(entry.m_type == ShaderDescriptorType::UNIFORM_BUFFER)
                {
                    if(buffer && buffer->isValid())
                    {
                        D3D12_CONSTANT_BUFFER_VIEW_DESC view { };
                        view.BufferLocation = buffer->getGPUAddress() + bindingEntry->m_offset;
                        // a constant buffer view is addressed in 256-byte multiples
                        const std::uint64_t range = bindingEntry->m_range == 0 ? buffer->getAllocatedSize() - bindingEntry->m_offset
                                                                               : bindingEntry->m_range;
                        view.SizeInBytes = static_cast<UINT>((range + 255) & ~static_cast<std::uint64_t>(255));
                        device->CreateConstantBufferView(&view, handle);
                        m_submission.m_keepAlive.push_back(bindingEntry->m_buffer);
                    }
                    else
                    {
                        // a table descriptor that is never written is undefined memory; a null view
                        // is the defined stand-in until the binding is filled
                        device->CreateConstantBufferView(nullptr, handle);
                    }
                    continue;
                }

                // a typed buffer takes a buffer SRV; it is never a render target, so none of the
                // attachment aliasing below applies to it
                if(bindingEntry && bindingEntry->m_texelBuffer && bindingEntry->m_texelBuffer->isTexelBuffer())
                {
                    const auto bufferView = bindingEntry->m_texelBuffer->getTexelSRVDesc();
                    device->CreateShaderResourceView(bindingEntry->m_texelBuffer->getResource(), &bufferView, handle);
                    m_submission.m_keepAlive.push_back(bindingEntry->m_texelBuffer);
                    continue;
                }

                // The passes bind every attachment of a framebuffer as a texture up front and only
                // then pick which one to draw into, so a descriptor can end up pointing at the very
                // image this pass renders to. Reading it is undefined and the debug layer complains,
                // and the descriptor still has to be written — so it gets the dummy, exactly as on
                // Vulkan. No engine shader reads what it writes; if one ever does, it reads white.
                if(!texture || isPassAttachment(texture)) texture = m_device.getDummyTexture();

                if(!texture)
                {
                    device->CreateShaderResourceView(nullptr, &DX12Texture::nullSRVDesc(), handle);
                    continue;
                }

                if(texture->getState() != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE && !m_inPass)
                {
                    texture->recordTransition(m_context.m_list.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                }
                const auto view = texture->getSRVDesc();
                device->CreateShaderResourceView(texture->getResource(), &view, handle);
                m_submission.m_keepAlive.push_back(texture);
            }
        }

        m_context.m_list->SetGraphicsRootDescriptorTable(table->m_rootParameterIndex, ring.gpuHandle(start));
        bound = true;
    }
    return bound;
}

SGCore::Ref<SGCore::DX12Texture> SGCore::DX12CommandList::resolveTexture(const DX12DescriptorSet::Entry& entry) const noexcept
{
    if(entry.m_dx12Texture) return entry.m_dx12Texture;
    if(const auto* facade = dynamic_cast<const DX12Texture2D*>(entry.m_texture.get())) return facade->getDX12Texture();
    return nullptr;
}

bool SGCore::DX12CommandList::isPassAttachment(const Ref<DX12Texture>& texture) const noexcept
{
    if(!m_inPass || !texture) return false;
    if(m_passDepthTexture == texture) return true;
    return std::find(m_passColorTextures.begin(), m_passColorTextures.end(), texture) != m_passColorTextures.end();
}

bool SGCore::DX12CommandList::flushState() noexcept
{
    if(!m_inPass)
    {
        SG_LOG_E("DX12CommandList: draw outside of a render pass.");
        return false;
    }
    if(!m_pipeline)
    {
        SG_LOG_E("DX12CommandList: draw without a bound pipeline.");
        return false;
    }

    if(!m_boundPipeline)
    {
        auto* pipeline = m_pipeline->getOrCreate(m_passFormats);
        if(!pipeline) return false;

        auto* rootSignature = m_pipeline->getRootSignature();
        if(rootSignature != m_boundRootSignature)
        {
            m_context.m_list->SetGraphicsRootSignature(rootSignature);
            m_boundRootSignature = rootSignature;
            // the tables belong to the previous root signature
            for(auto& slot : m_sets) slot.m_materialized = false;
        }
        m_context.m_list->SetPipelineState(pipeline);
        m_context.m_list->IASetPrimitiveTopology(m_pipeline->getTopology());
        // the stencil reference is command list state on D3D12, not part of the pipeline
        m_context.m_list->OMSetStencilRef(m_pipeline->getStencilRef());
        m_boundPipeline = pipeline;
    }

    for(std::uint32_t i = 0; i < max_descriptor_sets; ++i)
    {
        auto& slot = m_sets[i];
        if(!slot.m_set) continue;
        const bool stale = !slot.m_materialized ||
                           slot.m_materializedVersion != slot.m_set->getVersion() ||
                           slot.m_materializedRootSignature != m_boundRootSignature;
        if(!stale) continue;

        slot.m_materialized = materializeSet(slot, i);
        slot.m_materializedVersion = slot.m_set->getVersion();
        slot.m_materializedRootSignature = m_boundRootSignature;
    }

    if(m_viewportDirty) applyViewportScissor();
    return true;
}

// ---- draws ------------------------------------------------------------------------------------

void SGCore::DX12CommandList::draw(std::uint32_t vertexCount, std::uint32_t instanceCount,
                                   std::uint32_t firstVertex, std::uint32_t firstInstance) noexcept
{
    if(!flushState()) return;
    m_context.m_list->DrawInstanced(vertexCount, instanceCount, firstVertex, firstInstance);
}

void SGCore::DX12CommandList::drawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount,
                                          std::uint32_t firstIndex, std::int32_t vertexOffset,
                                          std::uint32_t firstInstance) noexcept
{
    if(!flushState()) return;
    m_context.m_list->DrawIndexedInstanced(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

// ---- clears / transitions / uploads --------------------------------------------------------------

void SGCore::DX12CommandList::clearColorAttachment(std::uint32_t colorIndex, const glm::vec4& color) noexcept
{
    if(!m_inPass || colorIndex >= m_passColorViews.size()) return;

    const float clearColor[4] = { color.r, color.g, color.b, color.a };
    m_context.m_list->ClearRenderTargetView(m_passColorViews[colorIndex], clearColor, 0, nullptr);
}

void SGCore::DX12CommandList::clearDepthStencil(float depth, bool clearStencil, std::uint32_t stencil) noexcept
{
    if(!m_inPass || !m_passDepthTexture) return;

    D3D12_CLEAR_FLAGS flags = D3D12_CLEAR_FLAG_DEPTH;
    if(clearStencil) flags |= D3D12_CLEAR_FLAG_STENCIL;
    m_context.m_list->ClearDepthStencilView(m_passDepthTexture->getDSV(), flags, depth,
                                            static_cast<UINT8>(stencil), 0, nullptr);
}

void SGCore::DX12CommandList::transition(const Ref<IGPUObject>& resource, GPUResourceState newState) noexcept
{
    auto texture = std::dynamic_pointer_cast<DX12Texture>(resource);
    // buffers need no transition here: D3D12 promotes them out of COMMON per command and decays back
    if(!texture || !m_recording || m_inPass) return;

    D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
    switch(newState)
    {
        case GPUResourceState::SGG_STATE_RENDER_TARGET: state = D3D12_RESOURCE_STATE_RENDER_TARGET; break;
        case GPUResourceState::SGG_STATE_DEPTH_WRITE: state = D3D12_RESOURCE_STATE_DEPTH_WRITE; break;
        case GPUResourceState::SGG_STATE_SHADER_READ: state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; break;
        case GPUResourceState::SGG_STATE_TRANSFER_SRC: state = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
        case GPUResourceState::SGG_STATE_TRANSFER_DST: state = D3D12_RESOURCE_STATE_COPY_DEST; break;
        case GPUResourceState::SGG_STATE_PRESENT: state = D3D12_RESOURCE_STATE_PRESENT; break;
        case GPUResourceState::SGG_STATE_UNDEFINED: return;
    }
    texture->recordTransition(m_context.m_list.Get(), state);
    m_submission.m_keepAlive.push_back(texture);
}

void SGCore::DX12CommandList::uploadData(const Ref<IGPUBuffer>& dst, const void* data, std::uint64_t size, std::uint64_t offset) noexcept
{
    auto* buffer = static_cast<DX12GPUBuffer*>(dst.get());
    if(!buffer || !buffer->isValid() || !data || size == 0) return;

    if(buffer->isHostVisible() || !m_recording || !m_uploadContext.isValid())
    {
        buffer->write(data, size, offset);
        return;
    }

    auto staging = m_device.createStagingBuffer(size, "upload_staging");
    if(!staging || !staging->isValid()) return;
    std::memcpy(staging->getMappedPointer(), data, size);

    m_uploadContext.m_list->CopyBufferRegion(buffer->getResource(), offset, staging->getResource(), 0, size);

    // A buffer is promoted out of COMMON per command and decays back — but only when the submission
    // ends. The upload list and the list that reads the buffer are submitted together, so within
    // that batch the buffer is still in COPY_DEST and using it as a vertex buffer is a validation
    // error. GENERIC_READ covers every way a buffer can be read afterwards.
    D3D12_RESOURCE_BARRIER barrier { };
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = buffer->getResource();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
    m_uploadContext.m_list->ResourceBarrier(1, &barrier);

    m_submission.m_keepAlive.push_back(staging);
    m_submission.m_keepAlive.push_back(dst);
}

#endif
