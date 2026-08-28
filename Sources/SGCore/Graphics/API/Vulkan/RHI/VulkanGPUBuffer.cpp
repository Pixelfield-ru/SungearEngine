//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanGPUBuffer.h"

#include <cstring>

#include "SGCore/Logger/Logger.h"
#include "VulkanDevice.h"

VkBufferUsageFlags SGCore::VulkanGPUBuffer::toVkUsage(GPUBufferUsage usage) noexcept
{
    VkBufferUsageFlags flags = 0;
    if(hasUsage(usage, GPUBufferUsage::SGG_VERTEX_BUFFER)) flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if(hasUsage(usage, GPUBufferUsage::SGG_INDEX_BUFFER)) flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if(hasUsage(usage, GPUBufferUsage::SGG_UNIFORM_BUFFER)) flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    // the engine's storage-ish buffers are read by shaders as samplerBuffer (TBO), which on Vulkan is
    // a uniform texel buffer, so the bit rides along with the storage usage
    if(hasUsage(usage, GPUBufferUsage::SGG_STORAGE_BUFFER))
    {
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
    }
    if(hasUsage(usage, GPUBufferUsage::SGG_TRANSFER_SRC)) flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if(hasUsage(usage, GPUBufferUsage::SGG_TRANSFER_DST)) flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    return flags;
}

SGCore::VulkanGPUBuffer::VulkanGPUBuffer(VulkanDevice& device, const GPUBufferDesc& desc) noexcept : m_device(device)
{
    m_desc = desc;
    m_debugName = desc.m_debugName;

    auto& context = device.getContext();
    m_context = context.shared_from_this();

    VkBufferCreateInfo bufferInfo { };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = desc.m_size == 0 ? 1 : desc.m_size;
    // every buffer can be a copy destination (uploadData) and source (readbacks)
    bufferInfo.usage = toVkUsage(desc.m_usage) | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocationInfo { };
    allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
    if(desc.m_access == GPUMemoryAccess::SGG_HOST_VISIBLE)
    {
        allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
        // uniform buffers written every frame want to live in host-visible memory that the GPU reads directly
        allocationInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }

    VmaAllocationInfo resultInfo { };
    if(!SG_VK_CHECK(vmaCreateBuffer(context.m_allocator, &bufferInfo, &allocationInfo, &m_buffer, &m_allocation, &resultInfo)))
    {
        m_buffer = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
        return;
    }
    m_mapped = resultInfo.pMappedData;

    context.setObjectName(reinterpret_cast<std::uint64_t>(m_buffer), VK_OBJECT_TYPE_BUFFER, desc.m_debugName);
    context.registerResource(this, [this] { releaseGPU(); });
}

SGCore::VulkanGPUBuffer::~VulkanGPUBuffer()
{
    if(m_context) m_context->unregisterResource(this);
    releaseGPU();
}

bool SGCore::VulkanGPUBuffer::createTexelView(VkFormat format) noexcept
{
    if(m_buffer == VK_NULL_HANDLE || !m_context) return false;

    if(m_texelView != VK_NULL_HANDLE)
    {
        vkDestroyBufferView(m_context->m_device, m_texelView, nullptr);
        m_texelView = VK_NULL_HANDLE;
    }

    VkBufferViewCreateInfo viewInfo { VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO };
    viewInfo.buffer = m_buffer;
    viewInfo.format = format;
    viewInfo.offset = 0;
    viewInfo.range = VK_WHOLE_SIZE;

    return SG_VK_CHECK(vkCreateBufferView(m_context->m_device, &viewInfo, nullptr, &m_texelView));
}

void SGCore::VulkanGPUBuffer::releaseGPU() noexcept
{
    if(m_texelView != VK_NULL_HANDLE && m_context)
    {
        vkDestroyBufferView(m_context->m_device, m_texelView, nullptr);
        m_texelView = VK_NULL_HANDLE;
    }
    if(m_buffer != VK_NULL_HANDLE && m_context && m_context->m_allocator != VK_NULL_HANDLE)
    {
        vmaDestroyBuffer(m_context->m_allocator, m_buffer, m_allocation);
    }
    m_buffer = VK_NULL_HANDLE;
    m_allocation = VK_NULL_HANDLE;
    m_mapped = nullptr;
}

void* SGCore::VulkanGPUBuffer::map(std::uint64_t offset, std::uint64_t size) noexcept
{
    if(!m_mapped)
    {
        SG_LOG_E("VulkanGPUBuffer '{}': map() on a device-local buffer is not supported; use write() or ICommandList::uploadData().", m_debugName);
        return nullptr;
    }
    if(offset + size > m_desc.m_size) return nullptr;
    return static_cast<std::uint8_t*>(m_mapped) + offset;
}

void SGCore::VulkanGPUBuffer::unmap() noexcept
{
    // persistently mapped, coherent memory: nothing to flush
}

bool SGCore::VulkanGPUBuffer::write(const void* data, std::uint64_t size, std::uint64_t offset) noexcept
{
    if(!data || size == 0 || !isValid()) return false;
    if(offset + size > m_desc.m_size)
    {
        SG_LOG_E("VulkanGPUBuffer '{}': write of {} bytes at offset {} exceeds the buffer size {}.", m_debugName, size, offset, m_desc.m_size);
        return false;
    }

    if(m_mapped)
    {
        std::memcpy(static_cast<std::uint8_t*>(m_mapped) + offset, data, size);
        return true;
    }

    // device-local: staging + synchronous copy. Fine for setup-time uploads, use the command list
    // path for per-frame data.
    auto staging = m_device.createStagingBuffer(size, "write_staging");
    if(!staging || !staging->isValid()) return false;
    std::memcpy(staging->getMappedPointer(), data, size);

    m_device.immediateSubmit([&](VkCommandBuffer commandBuffer) {
        VkBufferCopy region { };
        region.srcOffset = 0;
        region.dstOffset = offset;
        region.size = size;
        vkCmdCopyBuffer(commandBuffer, staging->getHandle(), m_buffer, 1, &region);
    });
    return true;
}
