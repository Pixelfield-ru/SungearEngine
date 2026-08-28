//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12GPUBuffer.h"

#if defined(_WIN32)

#include <cstring>

#include "SGCore/Logger/Logger.h"
#include "DX12Device.h"

SGCore::DX12GPUBuffer::DX12GPUBuffer(DX12Device& device, const GPUBufferDesc& desc) noexcept : m_device(device)
{
    m_desc = desc;
    m_debugName = desc.m_debugName;
    m_context = device.getContextRef();

    const bool hostVisible = desc.m_access == GPUMemoryAccess::SGG_HOST_VISIBLE;

    // a constant buffer view addresses 256-byte multiples, so the resource is rounded up to keep
    // the last block addressable
    m_allocatedSize = desc.m_size == 0 ? 1 : desc.m_size;
    if(hasUsage(desc.m_usage, GPUBufferUsage::SGG_UNIFORM_BUFFER))
    {
        m_allocatedSize = (m_allocatedSize + 255) & ~static_cast<std::uint64_t>(255);
    }

    D3D12_HEAP_PROPERTIES heapProperties { };
    heapProperties.Type = hostVisible ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC resourceDesc { };
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = m_allocatedSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // an UPLOAD resource must be created in GENERIC_READ and stays there; a DEFAULT one starts in
    // COMMON and is promoted per command
    const D3D12_RESOURCE_STATES initialState = hostVisible ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COMMON;

    if(!SG_DX_CHECK(m_context->m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
                                                                 initialState, nullptr, IID_PPV_ARGS(&m_resource))))
    {
        m_resource.Reset();
        return;
    }
    m_context->setObjectName(m_resource.Get(), desc.m_debugName);

    if(hostVisible)
    {
        const D3D12_RANGE readRange { 0, 0 }; // written by the CPU, never read back through this pointer
        if(!SG_DX_CHECK(m_resource->Map(0, &readRange, &m_mapped)))
        {
            m_mapped = nullptr;
        }
    }
}

SGCore::DX12GPUBuffer::~DX12GPUBuffer()
{
    // the resource keeps the device alive through COM, so unmapping here is safe even in static
    // destruction, when the renderer is long gone
    if(m_mapped && m_resource)
    {
        m_resource->Unmap(0, nullptr);
        m_mapped = nullptr;
    }
}

D3D12_GPU_VIRTUAL_ADDRESS SGCore::DX12GPUBuffer::getGPUAddress() const noexcept
{
    return m_resource ? m_resource->GetGPUVirtualAddress() : 0;
}

void* SGCore::DX12GPUBuffer::map(std::uint64_t offset, std::uint64_t size) noexcept
{
    if(!m_mapped)
    {
        SG_LOG_E("DX12GPUBuffer '{}': map() on a device-local buffer is not supported; use write() or ICommandList::uploadData().", m_debugName);
        return nullptr;
    }
    if(offset + size > m_allocatedSize) return nullptr;
    return static_cast<std::uint8_t*>(m_mapped) + offset;
}

void SGCore::DX12GPUBuffer::unmap() noexcept
{
    // persistently mapped: nothing to flush
}

bool SGCore::DX12GPUBuffer::write(const void* data, std::uint64_t size, std::uint64_t offset) noexcept
{
    if(!data || size == 0 || !isValid()) return false;
    if(offset + size > m_allocatedSize)
    {
        SG_LOG_E("DX12GPUBuffer '{}': write of {} bytes at offset {} exceeds the buffer size {}.", m_debugName, size, offset, m_allocatedSize);
        return false;
    }

    if(m_mapped)
    {
        std::memcpy(static_cast<std::uint8_t*>(m_mapped) + offset, data, size);
        return true;
    }

    // device-local: staging + synchronous copy, as on Vulkan. Fine at setup time; per-frame data
    // goes through ICommandList::uploadData.
    auto staging = m_device.createStagingBuffer(size, "write_staging");
    if(!staging || !staging->isValid()) return false;
    std::memcpy(staging->getMappedPointer(), data, size);

    m_device.immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        commandList->CopyBufferRegion(m_resource.Get(), offset, staging->getResource(), 0, size);
    });
    return true;
}


D3D12_SHADER_RESOURCE_VIEW_DESC SGCore::DX12GPUBuffer::getTexelSRVDesc() const noexcept
{
    D3D12_SHADER_RESOURCE_VIEW_DESC desc { };
    desc.Format = m_texelFormat;
    desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    desc.Buffer.FirstElement = 0;
    desc.Buffer.NumElements = m_texelElements;
    desc.Buffer.StructureByteStride = 0;
    desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    return desc;
}


#endif