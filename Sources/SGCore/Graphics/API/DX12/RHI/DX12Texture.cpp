//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12Texture.h"

#if defined(_WIN32)

#include <algorithm>

#include "SGCore/Graphics/API/DX12/DX12TypesCaster.h"
#include "SGCore/Logger/Logger.h"

SGCore::Ref<SGCore::DX12Texture> SGCore::DX12Texture::wrap(const std::shared_ptr<DX12Context>& context, ID3D12Resource* resource,
                                                           DXGI_FORMAT format, std::uint32_t width, std::uint32_t height,
                                                           D3D12_RESOURCE_STATES state, const std::string& debugName) noexcept
{
    if(!context || !resource) return nullptr;

    Ref<DX12Texture> texture(new DX12Texture);
    texture->m_context = context;
    texture->m_resource = resource;
    texture->m_format = format;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_state = state;
    texture->m_debugName = debugName;
    context->setObjectName(resource, debugName);
    return texture;
}

SGCore::Ref<SGCore::DX12Texture> SGCore::DX12Texture::create(const std::shared_ptr<DX12Context>& context, const DX12TextureDesc& desc) noexcept
{
    if(!context || !context->isReady()) return nullptr;

    const bool depth = desc.m_depthStencil || DX12TypesCaster::isDepthFormat(desc.m_format);

    D3D12_HEAP_PROPERTIES heapProperties { };
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC resourceDesc { };
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Width = desc.m_width == 0 ? 1 : desc.m_width;
    resourceDesc.Height = desc.m_height == 0 ? 1 : desc.m_height;
    resourceDesc.DepthOrArraySize = static_cast<UINT16>(desc.m_cube ? 6 : std::max<std::uint32_t>(1, desc.m_arrayLayers));
    resourceDesc.MipLevels = static_cast<UINT16>(std::max<std::uint32_t>(1, desc.m_mipLevels));
    // a depth resource that is also sampled has to be typeless: the DSV and the SRV then pick their
    // own view formats out of it
    resourceDesc.Format = depth ? DX12TypesCaster::depthToTypeless(desc.m_format) : desc.m_format;
    resourceDesc.SampleDesc.Count = std::max<std::uint32_t>(1, desc.m_samples);
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    if(depth) resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    else if(desc.m_renderTarget) resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    // an optimized clear value is required for render targets, and using a different colour later
    // costs performance but stays correct
    D3D12_CLEAR_VALUE clearValue { };
    clearValue.Format = desc.m_format;
    if(depth)
    {
        clearValue.DepthStencil.Depth = 1.0f;
    }
    // No optimized clear value for colour targets: the framebuffer facade creates an attachment
    // before the pass tells it which colour it will be cleared to, and a value that does not match
    // the actual clear only costs a warning per clear ("the clear values do not match those passed
    // to resource creation" — 664 of them on the smoke scene). Depth keeps its 1.0, which the passes
    // really do clear to.
    const bool hasClearValue = depth;

    DX12Ptr<ID3D12Resource> resource;
    if(!SG_DX_CHECK(context->m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
                                                               D3D12_RESOURCE_STATE_COMMON,
                                                               hasClearValue ? &clearValue : nullptr, IID_PPV_ARGS(&resource))))
    {
        return nullptr;
    }

    Ref<DX12Texture> texture(new DX12Texture);
    texture->m_context = context;
    texture->m_resource = resource;
    texture->m_format = desc.m_format;
    texture->m_width = static_cast<std::uint32_t>(resourceDesc.Width);
    texture->m_height = resourceDesc.Height;
    texture->m_state = D3D12_RESOURCE_STATE_COMMON;
    texture->m_mipLevels = resourceDesc.MipLevels;
    texture->m_arrayLayers = resourceDesc.DepthOrArraySize;
    texture->m_cube = desc.m_cube;
    texture->m_filter = desc.m_filter;
    texture->m_addressMode = desc.m_addressMode;
    texture->m_debugName = desc.m_debugName;
    context->setObjectName(resource.Get(), desc.m_debugName);
    return texture;
}

SGCore::DX12Texture::~DX12Texture()
{
    if(!m_context) return;
    m_context->m_rtvHeap.free(m_rtvIndex);
    m_context->m_dsvHeap.free(m_dsvIndex);
}

bool SGCore::DX12Texture::isDepth() const noexcept
{
    return DX12TypesCaster::isDepthFormat(m_format);
}

void SGCore::DX12Texture::recordTransition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState) noexcept
{
    if(!commandList || !m_resource || newState == m_state) return;

    D3D12_RESOURCE_BARRIER barrier { };
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_resource.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = m_state;
    barrier.Transition.StateAfter = newState;
    commandList->ResourceBarrier(1, &barrier);

    m_state = newState;
}

D3D12_SAMPLER_DESC SGCore::DX12Texture::defaultSamplerDesc() noexcept
{
    D3D12_SAMPLER_DESC desc { };
    desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    desc.MaxLOD = D3D12_FLOAT32_MAX;
    return desc;
}

const D3D12_SHADER_RESOURCE_VIEW_DESC& SGCore::DX12Texture::nullSRVDesc() noexcept
{
    static const D3D12_SHADER_RESOURCE_VIEW_DESC desc = [] {
        D3D12_SHADER_RESOURCE_VIEW_DESC view { };
        view.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        view.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        view.Texture2D.MipLevels = 1;
        return view;
    }();
    return desc;
}

D3D12_SHADER_RESOURCE_VIEW_DESC SGCore::DX12Texture::getSRVDesc() const noexcept
{
    D3D12_SHADER_RESOURCE_VIEW_DESC desc { };
    // a depth resource is typeless in memory: sampling it needs the colour-shaped view format
    desc.Format = isDepth() ? DX12TypesCaster::depthToShaderView(m_format) : m_format;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    if(m_cube)
    {
        desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
        desc.TextureCube.MipLevels = m_mipLevels;
    }
    else
    {
        desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = m_mipLevels;
    }
    return desc;
}

D3D12_SAMPLER_DESC SGCore::DX12Texture::getSamplerDesc() const noexcept
{
    D3D12_SAMPLER_DESC desc { };
    desc.Filter = m_filter;
    desc.AddressU = m_addressMode;
    desc.AddressV = m_addressMode;
    desc.AddressW = m_addressMode;
    desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    desc.MaxLOD = D3D12_FLOAT32_MAX;
    return desc;
}

D3D12_CPU_DESCRIPTOR_HANDLE SGCore::DX12Texture::getRTV() noexcept
{
    if(!m_context || !m_resource || isDepth()) return { };

    if(m_rtvIndex == DX12DescriptorHeap::invalid_index)
    {
        m_rtvIndex = m_context->m_rtvHeap.allocate();
        if(m_rtvIndex == DX12DescriptorHeap::invalid_index) return { };

        D3D12_RENDER_TARGET_VIEW_DESC desc { };
        desc.Format = m_format;
        desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        m_context->m_device->CreateRenderTargetView(m_resource.Get(), &desc, m_context->m_rtvHeap.cpuHandle(m_rtvIndex));
    }
    return m_context->m_rtvHeap.cpuHandle(m_rtvIndex);
}

D3D12_CPU_DESCRIPTOR_HANDLE SGCore::DX12Texture::getDSV() noexcept
{
    if(!m_context || !m_resource || !isDepth()) return { };

    if(m_dsvIndex == DX12DescriptorHeap::invalid_index)
    {
        m_dsvIndex = m_context->m_dsvHeap.allocate();
        if(m_dsvIndex == DX12DescriptorHeap::invalid_index) return { };

        D3D12_DEPTH_STENCIL_VIEW_DESC desc { };
        desc.Format = m_format;
        desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        m_context->m_device->CreateDepthStencilView(m_resource.Get(), &desc, m_context->m_dsvHeap.cpuHandle(m_dsvIndex));
    }
    return m_context->m_dsvHeap.cpuHandle(m_dsvIndex);
}

#endif
