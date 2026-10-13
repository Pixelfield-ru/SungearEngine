//
// Created by stuka on 06.10.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_descriptor_set.h"

#include "pixelfield/rhi/vulkan/vulkan_buffer.h"
#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_errors.h"
#include "pixelfield/rhi/vulkan/vulkan_image.h"
#include "pixelfield/rhi/vulkan/vulkan_image_view.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_sampler.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanDescriptorSet::VulkanDescriptorSet(VulkanDescriptorSet&& other) noexcept :
    context_(std::exchange(other.context_, {})),
    logical_device_(std::exchange(other.logical_device_, {})),
    pool_(std::exchange(other.pool_, VK_NULL_HANDLE)),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)),
    layout_(std::exchange(other.layout_, VK_NULL_HANDLE))
{
}

pf::rhi::VulkanDescriptorSet::~VulkanDescriptorSet() noexcept
{
    destroy();
}

void pf::rhi::VulkanDescriptorSet::update(std::span<DescriptorInfo> descriptors) noexcept
{
    std::vector<VkWriteDescriptorSet> vulkan_descriptors(descriptors.size());
    // Must outlive vkUpdateDescriptorSets — WriteDescriptorSet only stores pointers.
    std::vector<std::vector<VkDescriptorBufferInfo>> buffer_infos(descriptors.size());
    std::vector<std::vector<VkDescriptorImageInfo>> image_infos(descriptors.size());

    for(size_t i = 0; i < descriptors.size(); ++i)
    {
        const auto& descriptor = descriptors[i];
        auto& vulkan_descriptor = vulkan_descriptors[i];

        vulkan_descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vulkan_descriptor.dstSet = handle_;
        vulkan_descriptor.dstBinding = descriptor.binding;
        vulkan_descriptor.dstArrayElement = descriptor.array_offset;
        vulkan_descriptor.descriptorType = toVkDescriptorType(descriptor.type);
        if(descriptor.type == DescriptorType::kUniformBuffer ||
           descriptor.type == DescriptorType::kUniformBufferDynamic ||
           descriptor.type == DescriptorType::kStorageBuffer ||
           descriptor.type == DescriptorType::kStorageBufferDynamic)
        {
            auto& buffers = buffer_infos[i];
            buffers.resize(descriptor.buffers.size());
            for(size_t k = 0; k < descriptor.buffers.size(); ++k)
            {
                const auto& buffer = descriptor.buffers[k];
                auto& buffer_info = buffers[k];
                buffer_info.buffer = static_cast<const VulkanBuffer*>(buffer.buffer)->handle_;
                buffer_info.offset = buffer.offset;
                buffer_info.range = buffer.range;
            }

            vulkan_descriptor.pBufferInfo = buffers.data();
            vulkan_descriptor.descriptorCount = static_cast<uint32_t>(descriptor.buffers.size());
        }
        else if(descriptor.type == DescriptorType::kCombinedImageSampler ||
                descriptor.type == DescriptorType::kSampledImage ||
                descriptor.type == DescriptorType::kStorageImage)
        {
            auto& images = image_infos[i];
            images.resize(descriptor.images.size());
            for(size_t k = 0; k < descriptor.images.size(); ++k)
            {
                const auto& image = descriptor.images[k];
                auto& image_info = images[k];
                image_info.imageView = static_cast<const VulkanImageView*>(image.image_view)->handle_;
                image_info.imageLayout = toVkImageLayout(image.image_layout);
                image_info.sampler = static_cast<const VulkanSampler*>(image.sampler)->handle_;
            }

            vulkan_descriptor.pImageInfo = images.data();
            vulkan_descriptor.descriptorCount = static_cast<uint32_t>(descriptor.images.size());
        }
        // todo: use texel buffers
        // else if texels
    }

    vkUpdateDescriptorSets(logical_device_->handle_, static_cast<uint32_t>(vulkan_descriptors.size()),
                           vulkan_descriptors.data(), 0, nullptr);
}

pf::rhi::VulkanDescriptorSet& pf::rhi::VulkanDescriptorSet::operator=(VulkanDescriptorSet&& other) noexcept
{
    if(std::addressof(other) == this) return *this;

    destroy();

    context_ = std::exchange(other.context_, {});
    logical_device_ = std::exchange(other.logical_device_, {});
    pool_ = std::exchange(other.pool_, VK_NULL_HANDLE);
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
    layout_ = std::exchange(other.layout_, VK_NULL_HANDLE);

    return *this;
}

void pf::rhi::VulkanDescriptorSet::destroy()
{
    if(handle_)
    {
        vkFreeDescriptorSets(logical_device_->handle_, pool_, 1, &handle_);
        handle_ = VK_NULL_HANDLE;
    }

    if(layout_)
    {
        vkDestroyDescriptorSetLayout(logical_device_->handle_, layout_, context_->allocator_.get());
        layout_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanDescriptorSet::create(const DescriptorSetCreateInfo& create_info)
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    auto* vulkan_logical_device = static_cast<VulkanLogicalDevice*>(create_info.logical_device);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;

    if(const auto res = logical_device_->descriptorPool())
    {
        pool_ = *res;
    }
    else
    {
        return res.error();
    }

    std::vector<VkDescriptorSetLayoutBinding> vulkan_bindings(create_info.bindings.size());
    for(size_t i = 0; i < create_info.bindings.size(); ++i)
    {
        const auto& binding = create_info.bindings[i];
        auto& vulkan_binding = vulkan_bindings[i];
        vulkan_binding.binding = binding.binding;
        vulkan_binding.stageFlags = toVkShaderStageFlags(binding.stages);
        vulkan_binding.descriptorCount = binding.descriptor_count;
        vulkan_binding.descriptorType = toVkDescriptorType(binding.type);
    }

    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = vulkan_bindings.size();
    layout_info.pBindings = vulkan_bindings.data();

    if(vkCreateDescriptorSetLayout(vulkan_logical_device->handle_, &layout_info, vulkan_context->allocator_.get(), &layout_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateDescriptorSetLayout;
    }

    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool_;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &layout_;
    if(vkAllocateDescriptorSets(vulkan_logical_device->handle_, &alloc_info, &handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateDescriptorSet;
    }

    return {};
}
