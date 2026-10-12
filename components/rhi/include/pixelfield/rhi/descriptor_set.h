//
// Created by stuka on 06.10.2026.
//

#pragma once

#include <system_error>
#include <vector>
#include <span>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;
    struct IBuffer;
    struct IImageView;
    struct ISampler;

    struct DescriptorBuffer final
    {
        const IBuffer* buffer {};
        std::uint32_t offset {};
        std::uint32_t range {};
    };

    struct DescriptorImage final
    {
        const IImageView* image_view {};
        const ISampler* sampler {};
        ImageLayout image_layout = ImageLayout::kShaderReadOnly;
    };

    struct DescriptorInfo final
    {
        std::int16_t binding {};
        DescriptorType type = DescriptorType::kStorageBuffer;
        std::vector<DescriptorBuffer> buffers;
        std::vector<DescriptorImage> images;
        /// Offset in buffers or images.
        std::uint32_t array_offset {};
    };

    struct DescriptorSetLayoutBinding final
    {
        std::uint32_t binding {};
        DescriptorType type = DescriptorType::kStorageBuffer;
        std::uint32_t descriptor_count {};
        ShaderStageType stages = ShaderStageType::kUnknown;
    };

    struct DescriptorSetCreateInfo final
    {
        const IContext* context {};
        ILogicalDevice* logical_device {};
        std::vector<DescriptorSetLayoutBinding> bindings;
    };

    struct IDescriptorSet
    {
        virtual ~IDescriptorSet() = default;

        virtual void update(std::span<DescriptorInfo> descriptors) = 0;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const DescriptorSetCreateInfo& create_info) = 0;
    };
}