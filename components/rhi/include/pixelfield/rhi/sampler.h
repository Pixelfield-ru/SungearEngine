//
// Created by stuka on 11.10.2026.
//

#pragma once

#include <system_error>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;

    struct SamplerCreateInfo final
    {
        const IContext* context {};
        const ILogicalDevice* logical_device {};

        FilterType mag_filter = FilterType::kLinear;
        FilterType min_filter = FilterType::kLinear;
        MipmapMode mipmap_mode = MipmapMode::kLinear;

        RepeatMode repeat_mode_u = RepeatMode::kRepeat;
        RepeatMode repeat_mode_v = RepeatMode::kRepeat;
        RepeatMode repeat_mode_w = RepeatMode::kRepeat;

        bool enable_anisotropy = true;
        float max_anisotropy = 16.0f;

        bool enable_compare {};
        CompareOp compare_op = CompareOp::kAlways;

        float min_lod {};
        float max_lod {};
        float mip_lod_bias {};

        bool unnormalized_coordinates {};
    };

    struct ISampler
    {
        virtual ~ISampler() = default;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const SamplerCreateInfo& create_info) = 0;
    };
}