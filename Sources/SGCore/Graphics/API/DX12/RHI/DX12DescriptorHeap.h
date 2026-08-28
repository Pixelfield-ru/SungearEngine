//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/API/DX12/DX12Common.h"

namespace SGCore
{
    /**
     * One ID3D12DescriptorHeap plus the bookkeeping D3D12 does not do for us.
     *
     * Two use modes, both needed by the backend and both trivial, so they share one type:
     * - allocate()/free(): a free list over fixed slots. Used for the CPU-only RTV / DSV / SRV
     *   heaps, whose slots belong to a resource for as long as it lives.
     * - allocateRange(): a bump allocator that wraps around. Used for the shader-visible heap the
     *   command lists copy descriptor tables into every frame; the ring is sized so a region is
     *   overwritten only frames after the submission that read it has finished.
     */
    struct SGCORE_EXPORT DX12DescriptorHeap
    {
        static constexpr std::uint32_t invalid_index = ~0u;

        bool create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, std::uint32_t capacity, bool shaderVisible) noexcept;
        void destroy() noexcept;

        [[nodiscard]] std::uint32_t allocate() noexcept;
        void free(std::uint32_t index) noexcept;

        /// Start index of a contiguous range, or invalid_index when the range does not fit at all.
        [[nodiscard]] std::uint32_t allocateRange(std::uint32_t count) noexcept;
        void resetRing() noexcept { m_used = 0; }

        [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle(std::uint32_t index) const noexcept;
        [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle(std::uint32_t index) const noexcept;

        [[nodiscard]] ID3D12DescriptorHeap* getHeap() const noexcept { return m_heap.Get(); }
        [[nodiscard]] std::uint32_t getCapacity() const noexcept { return m_capacity; }
        [[nodiscard]] bool isValid() const noexcept { return m_heap != nullptr; }

    private:
        DX12Ptr<ID3D12DescriptorHeap> m_heap;
        std::vector<std::uint32_t> m_freeList;
        std::uint32_t m_capacity { };
        std::uint32_t m_used { };
        std::uint32_t m_incrementSize { };
        bool m_shaderVisible { };
    };
}

#endif
