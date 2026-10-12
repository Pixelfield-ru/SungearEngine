//
// Created by stuka on 02.10.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_buffer.h"

#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_errors.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_physical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept :
    context_(std::exchange(other.context_, {})),
    logical_device_(std::exchange(other.logical_device_, {})),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)),
    memory_(std::exchange(other.memory_, VK_NULL_HANDLE)),
    size_(std::exchange(other.size_, 0))
{
}

pf::rhi::VulkanBuffer::~VulkanBuffer() noexcept
{
    destroy();
}

bool pf::rhi::VulkanBuffer::valid() const noexcept
{
    return handle_ != VK_NULL_HANDLE;
}

std::error_code pf::rhi::VulkanBuffer::update(void* data, size_t offset, size_t size)
{
    return logical_device_->updateBuffer(data, *this, offset, size);
}

size_t pf::rhi::VulkanBuffer::size()
{
    return size_;
}

pf::rhi::VulkanBuffer& pf::rhi::VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    context_ = std::exchange(other.context_, {});
    logical_device_ = std::exchange(other.logical_device_, {});
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
    memory_ = std::exchange(other.memory_, VK_NULL_HANDLE);
    size_ = std::exchange(other.size_, 0);

    return *this;
}

std::error_code pf::rhi::VulkanBuffer::createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                                VkBuffer& out_buffer, VkDeviceMemory& out_memory) const noexcept
{
    VkBufferCreateInfo vk_create_info{};
    vk_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vk_create_info.size = size_;
    vk_create_info.usage = usage;
    vk_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(logical_device_->handle_, &vk_create_info, context_->allocator_.get(), &out_buffer) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateBuffer;
    }

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(logical_device_->handle_, out_buffer, &mem_reqs);

    const auto memory_index = logical_device_->physical_device_->findMemoryIndex(mem_reqs, properties);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_reqs.size;
    alloc_info.memoryTypeIndex = memory_index;

    if(vkAllocateMemory(logical_device_->handle_, &alloc_info, context_->allocator_.get(), &out_memory) != VK_SUCCESS)
    {
        return VulkanError::kCannotAllocateMemory;
    }

    if(vkBindBufferMemory(logical_device_->handle_, out_buffer, out_memory, 0) != VK_SUCCESS)
    {
        return VulkanError::kCannotBindMemory;
    }

    return {};
}

void pf::rhi::VulkanBuffer::destroy()
{
    if(handle_)
    {
        vkDestroyBuffer(logical_device_->handle_, handle_, context_->allocator_.get());
        handle_ = VK_NULL_HANDLE;
    }

    if(memory_)
    {
        vkFreeMemory(logical_device_->handle_, memory_, context_->allocator_.get());
        memory_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanBuffer::create(const BufferCreateInfo& create_info)
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context_);
    auto* vulkan_logical_device = static_cast<VulkanLogicalDevice*>(create_info.logical_device_);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;
    size_ = create_info.size;

    if(size_ == 0)
    {
        return VulkanError::kInvalidArgument;
    }

    VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if(create_info.host_coherent)
    {
        props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }

    if(const auto res = createBuffer(toVkBufferUsage(create_info.type), props, handle_, memory_))
    {
        return res;
    }

    if(create_info.data)
    {
        if(const auto res = update(create_info.data, 0, size_))
        {
            return res;
        }
    }

    return {};
}
