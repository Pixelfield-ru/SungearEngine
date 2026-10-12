//
// Created by stuka on 18.09.2026.
//

#pragma once

#include <vector>
#include <optional>
#include <glm/vec4.hpp>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;
    struct IShader;
    struct IPass;
    struct IDescriptorSet;

    // todo: make vertex input and pipeline layout & maybe allow use multiple viewports
    struct PipelineStateCreateInfo final
    {
        const IContext* context {};
        const ILogicalDevice* logical_device {};

        PrimitiveTopology topology = PrimitiveTopology::kTriangleList;
        bool enable_primitive_restart = false;

        bool enable_depth_clamp = false;
        bool enable_rasterizer_discard = false;
        PolygonMode polygon_mode = PolygonMode::kFill;
        CullMode cull_mode = CullMode::kBack;
        FrontFace front_face = FrontFace::kClockwise;
        bool enable_depth_bias = false;
        float depth_bias_constant_factor {};
        float depth_bias_clamp {};
        float depth_bias_slope_factor {};

        bool enable_sample_shading = false;
        SampleCountFlag rasterization_sample_count = SampleCountFlag::k1Sample;
        float min_sample_shading = 1.0f;
        bool enable_alpha_to_coverage = false;
        bool enable_alpha_to_one = false;

        std::vector<AttachmentBlendInfo> attachments_blend_info;

        const IPass* pass {};

        bool enable_logic_op = false;
        LogicOp logic_op = LogicOp::kCopy;
        glm::vec4 blend_constants{};

        std::vector<VertexBindingDescription> vertex_bindings;
        std::vector<const IDescriptorSet*> descriptor_sets;

        IShader* shader {};
    };

    struct IPipelineState
    {
        virtual ~IPipelineState() = default;

        [[nodiscard]] std::error_code recreate(const PipelineStateCreateInfo& create_info) noexcept
        {
            destroy();
            return create(create_info);
        }

        [[nodiscard]] PipelineBindPoint bindPoint() const noexcept
        {
            return bind_point_;
        }

    private:
        PipelineBindPoint bind_point_ = PipelineBindPoint::kGraphics;

        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const PipelineStateCreateInfo& create_info) = 0;
    };
}
