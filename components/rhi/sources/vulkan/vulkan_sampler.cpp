//
// Created by stuka on 11.10.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_sampler.h"

#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_errors.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanSampler::VulkanSampler(VulkanSampler&& other) noexcept :
    context_(std::exchange(other.context_, {})),
    logical_device_(std::exchange(other.logical_device_, {})),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE))
{

}

pf::rhi::VulkanSampler::~VulkanSampler() noexcept
{
    destroy();
}

pf::rhi::VulkanSampler& pf::rhi::VulkanSampler::operator=(VulkanSampler&& other) noexcept
{
    if(std::addressof(other) == this) return *this;

    destroy();

    context_ = std::exchange(other.context_, {});
    logical_device_ = std::exchange(other.logical_device_, {});
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);

    return *this;
}

void pf::rhi::VulkanSampler::destroy() noexcept
{
    if(handle_)
    {
        vkDestroySampler(logical_device_->handle_, handle_, context_->allocator_.get());
        handle_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanSampler::create(const SamplerCreateInfo& create_info)
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    const auto* vulkan_logical_device = static_cast<const VulkanLogicalDevice*>(create_info.logical_device);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;

    VkSamplerCreateInfo sampler_create_info {};
    sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_create_info.addressModeU = toVkAddressMode(create_info.repeat_mode_u);
    sampler_create_info.addressModeV = toVkAddressMode(create_info.repeat_mode_v);
    sampler_create_info.addressModeW = toVkAddressMode(create_info.repeat_mode_w);

    sampler_create_info.anisotropyEnable = create_info.enable_anisotropy;
    sampler_create_info.maxAnisotropy = create_info.max_anisotropy;

    sampler_create_info.compareEnable = create_info.enable_compare;
    sampler_create_info.compareOp = toVkCompareOp(create_info.compare_op);

    sampler_create_info.minLod = create_info.min_lod;
    sampler_create_info.maxLod = create_info.max_lod;
    sampler_create_info.mipLodBias = create_info.mip_lod_bias;

    // todo:
    // sampler_create_info.borderColor;

    sampler_create_info.unnormalizedCoordinates = create_info.unnormalized_coordinates;

    if(vkCreateSampler(vulkan_logical_device->handle_, &sampler_create_info, vulkan_context->allocator_.get(), &handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateSampler;
    }

    return {};
}
