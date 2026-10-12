//
// Created by stuka on 14.09.2026.
//

#pragma once

#include "types.h"

#include <system_error>

namespace pf::rhi
{
    struct IContext;
    struct IImage;
    struct ILogicalDevice;

    struct ImageViewCreateInfo final
    {
        const IContext* context {};
        const ILogicalDevice* logical_device {};

        const IImage* image {};

        ImageType type = ImageType::k2D;
        Format format = Format::kUnknown;
        ComponentMapping component_mapping {};
        ImageAspect aspect_mask = ImageAspect::kColor;
        std::uint16_t base_mip_level = 0;
        std::uint16_t levels_count = 1;
        std::uint16_t base_array_layer = 0;
        std::uint16_t layers_count = 1;
    };

    struct IImageView
    {
        virtual ~IImageView() = default;

        [[nodiscard]] std::error_code recreate(const ImageViewCreateInfo& create_info) noexcept;

        [[nodiscard]] virtual bool valid() const noexcept = 0;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const ImageViewCreateInfo& create_info) = 0;
    };
}