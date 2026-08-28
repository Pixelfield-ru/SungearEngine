//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12DescriptorHeap.h"

#if defined(_WIN32)

#include "SGCore/Logger/Logger.h"

bool SGCore::DX12DescriptorHeap::create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type,
                                        std::uint32_t capacity, bool shaderVisible) noexcept
{
    destroy();
    if(!device || capacity == 0) return false;

    D3D12_DESCRIPTOR_HEAP_DESC desc { };
    desc.Type = type;
    desc.NumDescriptors = capacity;
    desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    if(!SG_DX_CHECK(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap)))) return false;

    m_capacity = capacity;
    m_used = 0;
    m_shaderVisible = shaderVisible;
    m_incrementSize = device->GetDescriptorHandleIncrementSize(type);
    return true;
}

void SGCore::DX12DescriptorHeap::destroy() noexcept
{
    m_heap.Reset();
    m_freeList.clear();
    m_capacity = 0;
    m_used = 0;
    m_incrementSize = 0;
    m_shaderVisible = false;
}

std::uint32_t SGCore::DX12DescriptorHeap::allocate() noexcept
{
    if(!m_freeList.empty())
    {
        const std::uint32_t index = m_freeList.back();
        m_freeList.pop_back();
        return index;
    }
    if(m_used >= m_capacity)
    {
        SG_LOG_E("DX12DescriptorHeap: out of descriptors ({} of {} used).", m_used, m_capacity);
        return invalid_index;
    }
    return m_used++;
}

void SGCore::DX12DescriptorHeap::free(std::uint32_t index) noexcept
{
    if(index == invalid_index || index >= m_capacity) return;
    m_freeList.push_back(index);
}

std::uint32_t SGCore::DX12DescriptorHeap::allocateRange(std::uint32_t count) noexcept
{
    if(count == 0 || count > m_capacity) return invalid_index;
    // a range must be contiguous: wrap to the start rather than split it
    if(m_used + count > m_capacity) m_used = 0;
    const std::uint32_t start = m_used;
    m_used += count;
    return start;
}

D3D12_CPU_DESCRIPTOR_HANDLE SGCore::DX12DescriptorHeap::cpuHandle(std::uint32_t index) const noexcept
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle { };
    if(!m_heap || index == invalid_index) return handle;
    handle = m_heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += static_cast<SIZE_T>(index) * m_incrementSize;
    return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SGCore::DX12DescriptorHeap::gpuHandle(std::uint32_t index) const noexcept
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle { };
    if(!m_heap || !m_shaderVisible || index == invalid_index) return handle;
    handle = m_heap->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += static_cast<UINT64>(index) * m_incrementSize;
    return handle;
}

#endif
