//
// Created by stuka on 14.09.2026.
//

#pragma once

#include <system_error>
#include <glm/vec3.hpp>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;

    struct ImageCreateInfo final
    {
        const IContext* context {};
        ILogicalDevice* logical_device {};

        ImageType type = ImageType::k2D;
        Format format = Format::kUnknown;
        glm::uvec3 extent {};
        std::int16_t mip_levels = 1;
        std::int16_t array_layers = 1;
        SampleCountFlag sample_count = SampleCountFlag::k1Sample;
        ImageTiling tiling = ImageTiling::kOptimal;
        ImageUsage usage = ImageUsage::kTransferDst;

        size_t size {};
        void* data {};
    };

    struct ImageUpdateInfo final
    {
        void* data {};

        std::uint32_t mip_level = 0;
        std::uint32_t base_array_layer = 0;
        std::uint32_t layer_count = 1;
        /// In texels.
        glm::ivec3 offset { 0, 0, 0 };
        /// In texels.
        glm::uvec3 extent { 0, 0, 1 };
        /// Layout after copy.
        ImageLayout final_layout = ImageLayout::kShaderReadOnly;
        ImageAspect aspect = ImageAspect::kColor;

        std::uint32_t buffer_row_length = 0;
        std::uint32_t buffer_image_height = 0;
    };

    struct IImage
    {
        virtual ~IImage() = default;

        [[nodiscard]] std::error_code recreate(const ImageCreateInfo& create_info) noexcept
        {
            destroy();
            return create(create_info);
        }

        [[nodiscard]] virtual std::error_code update(const ImageUpdateInfo& update_info) = 0;

        [[nodiscard]] Format format() const noexcept
        {
            return format_;
        }

        [[nodiscard]] ImageLayout layout() const noexcept
        {
            return layout_;
        }

        [[nodiscard]] virtual IImageView const *const view() const noexcept = 0;
        [[nodiscard]] virtual IImageView *const view() noexcept = 0;

        [[nodiscard]] virtual bool valid() const noexcept = 0;

    protected:
        Format format_ = Format::kUnknown;
        ImageLayout layout_ = ImageLayout::kUndefined;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const ImageCreateInfo& create_info) = 0;
    };
}
