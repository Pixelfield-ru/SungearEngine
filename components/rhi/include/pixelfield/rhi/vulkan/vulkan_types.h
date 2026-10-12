//
// Created by stuka on 12.09.2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include "pixelfield/rhi/types.h"

namespace pf::rhi
{
    VkFormat toVkFormat(Format format) noexcept;
    Format fromVkFormat(VkFormat format) noexcept;

    VkColorSpaceKHR toVkColorSpace(ColorSpace color_space) noexcept;
    ColorSpace fromVkColorSpace(VkColorSpaceKHR color_space) noexcept;

    // Cube/array map to the underlying VkImageType (1D/2D); use to_vk_image_view_type for views.
    VkImageType toVkImageType(ImageType image_type) noexcept;
    ImageType fromVkImageType(VkImageType image_type) noexcept;
    VkImageViewType toVkImageViewType(ImageType image_type) noexcept;

    VkImageTiling toVkImageTiling(ImageTiling tiling) noexcept;
    ImageTiling fromVkImageTiling(VkImageTiling tiling) noexcept;

    VkImageUsageFlags toVkImageUsage(ImageUsage usage) noexcept;
    ImageUsage fromVkImageUsage(VkImageUsageFlags usage) noexcept;

    // kLineLoop has no Vulkan equivalent and maps to VK_PRIMITIVE_TOPOLOGY_MAX_ENUM.
    VkPrimitiveTopology toVkPrimitiveTopology(PrimitiveTopology topology) noexcept;
    PrimitiveTopology fromVkPrimitiveTopology(VkPrimitiveTopology topology) noexcept;

    VkPolygonMode toVkPolygonMode(PolygonMode mode) noexcept;
    PolygonMode fromVkPolygonMode(VkPolygonMode mode) noexcept;

    VkCullModeFlags toVkCullMode(CullMode mode) noexcept;
    CullMode fromVkCullMode(VkCullModeFlags mode) noexcept;

    VkFrontFace toVkFrontFace(FrontFace face) noexcept;
    FrontFace fromVkFrontFace(VkFrontFace face) noexcept;

    VkSampleCountFlagBits toVkSampleCount(SampleCountFlag count) noexcept;
    SampleCountFlag fromVkSampleCount(VkSampleCountFlagBits count) noexcept;

    VkColorComponentFlags toVkColorComponents(ColorComponent components) noexcept;
    ColorComponent fromVkColorComponents(VkColorComponentFlags components) noexcept;

    VkBlendFactor toVkBlendFactor(BlendFactor factor) noexcept;
    BlendFactor fromVkBlendFactor(VkBlendFactor factor) noexcept;

    VkBlendOp toVkBlendOp(BlendOp op) noexcept;
    BlendOp fromVkBlendOp(VkBlendOp op) noexcept;

    VkLogicOp toVkLogicOp(LogicOp op) noexcept;
    LogicOp fromVkLogicOp(VkLogicOp op) noexcept;

    VkAttachmentLoadOp toVkAttachmentLoadOp(AttachmentLoadOp op) noexcept;
    AttachmentLoadOp fromVkAttachmentLoadOp(VkAttachmentLoadOp op) noexcept;

    VkAttachmentStoreOp toVkAttachmentStoreOp(AttachmentStoreOp op) noexcept;
    AttachmentStoreOp fromVkAttachmentStoreOp(VkAttachmentStoreOp op) noexcept;

    VkImageLayout toVkImageLayout(ImageLayout layout) noexcept;
    ImageLayout fromVkImageLayout(VkImageLayout layout) noexcept;

    // Typical access / pipeline stage masks for a layout (for image barriers).
    VkAccessFlags toVkAccessMask(ImageLayout layout) noexcept;
    VkPipelineStageFlags toVkPipelineStageFlags(ImageLayout layout) noexcept;

    VkImageAspectFlags toVkImageAspect(ImageAspect aspect) noexcept;
    ImageAspect fromVkImageAspect(VkImageAspectFlags aspect) noexcept;

    VkComponentSwizzle toVkComponentSwizzle(ComponentSwizzle swizzle) noexcept;
    ComponentSwizzle fromVkComponentSwizzle(VkComponentSwizzle swizzle) noexcept;
    VkComponentMapping toVkComponentMapping(const ComponentMapping& mapping) noexcept;
    ComponentMapping fromVkComponentMapping(const VkComponentMapping& mapping) noexcept;

    VkVertexInputRate toVkInputRate(InputRate rate) noexcept;
    InputRate fromVkInputRate(VkVertexInputRate rate) noexcept;

    VkIndexType toVkIndexType(IndexType type) noexcept;
    IndexType fromVkIndexType(VkIndexType type) noexcept;

    VkDescriptorType toVkDescriptorType(DescriptorType type) noexcept;
    DescriptorType fromVkDescriptorType(VkDescriptorType type) noexcept;

    VkShaderStageFlags toVkShaderStageFlags(ShaderStageType stages) noexcept;
    ShaderStageType fromVkShaderStageFlags(VkShaderStageFlags stages) noexcept;

    // kPresent has no VkQueueFlagBits (surface support is separate); maps to 0.
    VkQueueFlags toVkQueueFlags(QueueFamilyType type) noexcept;
    // Picks a single primary family from flag bits (graphics > compute > transfer > ...).
    // Never returns kPresent.
    QueueFamilyType fromVkQueueFlags(VkQueueFlags flags) noexcept;

    VkPipelineBindPoint toVkPipelineBindPoint(PipelineBindPoint bind_point) noexcept;
    PipelineBindPoint fromVkPipelineBindPoint(VkPipelineBindPoint bind_point) noexcept;

    VkBufferUsageFlags toVkBufferUsage(BufferType type) noexcept;
    BufferType fromVkBufferUsage(VkBufferUsageFlags usage) noexcept;

    VkFilter toVkFilter(FilterType filter) noexcept;
    FilterType fromVkFilter(VkFilter filter) noexcept;

    VkSamplerMipmapMode toVkMipmapMode(MipmapMode mode) noexcept;
    MipmapMode fromVkMipmapMode(VkSamplerMipmapMode mode) noexcept;

    VkSamplerAddressMode toVkAddressMode(RepeatMode mode) noexcept;
    RepeatMode fromVkAddressMode(VkSamplerAddressMode mode) noexcept;

    VkCompareOp toVkCompareOp(CompareOp op) noexcept;
    CompareOp fromVkCompareOp(VkCompareOp op) noexcept;
}
