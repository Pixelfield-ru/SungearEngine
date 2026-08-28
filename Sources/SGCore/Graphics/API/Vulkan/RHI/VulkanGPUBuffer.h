//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IGPUBuffer.h"
#include "VulkanContext.h"

namespace SGCore
{
    class VulkanDevice;

    /// VkBuffer + VMA allocation. Host-visible buffers are persistently mapped; device-local ones
    /// are written through a staging copy on the device's immediate queue.
    class SGCORE_EXPORT VulkanGPUBuffer final : public IGPUBuffer
    {
    public:
        VulkanGPUBuffer(VulkanDevice& device, const GPUBufferDesc& desc) noexcept;
        ~VulkanGPUBuffer() override;

        [[nodiscard]] void* map(std::uint64_t offset, std::uint64_t size) noexcept override;
        void unmap() noexcept override;
        bool write(const void* data, std::uint64_t size, std::uint64_t offset = 0) noexcept override;

        [[nodiscard]] VkBuffer getHandle() const noexcept { return m_buffer; }
        [[nodiscard]] bool isHostVisible() const noexcept { return m_mapped != nullptr; }
        [[nodiscard]] bool isValid() const noexcept { return m_buffer != VK_NULL_HANDLE; }
        /// Persistently mapped pointer of a host-visible buffer (nullptr for device-local ones).
        [[nodiscard]] void* getMappedPointer() const noexcept { return m_mapped; }

        static VkBufferUsageFlags toVkUsage(GPUBufferUsage usage) noexcept;

        /// Creates a uniform texel buffer view over the whole buffer — this is what a GLSL samplerBuffer
        /// reads. The buffer must carry SGG_STORAGE_BUFFER usage, which is what brings the texel bit
        /// along (see toVkUsage). Returns false and leaves the buffer usable as a plain buffer if the
        /// view can not be created.
        bool createTexelView(VkFormat format) noexcept;
        [[nodiscard]] VkBufferView getTexelView() const noexcept { return m_texelView; }

        /// Destroys the VkBuffer; the object becomes an empty shell (isValid() == false).
        void releaseGPU() noexcept;

    private:
        VulkanDevice& m_device;
        std::shared_ptr<VulkanContext> m_context;
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        void* m_mapped { };
        VkBufferView m_texelView = VK_NULL_HANDLE;
    };
}
