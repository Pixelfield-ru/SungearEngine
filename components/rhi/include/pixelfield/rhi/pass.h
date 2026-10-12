//
// Created by stuka on 29.09.2026.
//

#pragma once

#include <vector>
#include <optional>
#include <system_error>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;

    struct PassCreateInfo final
    {
        const IContext* context {};
        const ILogicalDevice* logical_device {};

        // targets
        std::vector<ColorAttachmentCreateInfo> color_attachments;
        std::optional<DepthStencilAttachmentCreateInfo> depth_stencil_attachment;
    };

    struct IPass
    {
        virtual ~IPass() = default;

        [[nodiscard]] virtual bool valid() const noexcept = 0;

    private:
        virtual void destroy() = 0;
        virtual std::error_code create(const PassCreateInfo& create_info) = 0;
    };
}
