//
// Created by stuka on 20.09.2026.
//

#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;
    struct IPass;
    struct IImageView;

    struct FramebufferCreateInfo final
    {
        const IContext* context {};
        const ILogicalDevice* logical_device {};

        const IPass* pass {};
        std::vector<const IImageView*> images;

        glm::uvec2 size {};
    };

    struct IFramebuffer
    {
        virtual ~IFramebuffer() = default;

        [[nodiscard]] std::error_code recreate(const FramebufferCreateInfo& create_info) noexcept
        {
            destroy();
            return create(create_info);
        }

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const FramebufferCreateInfo& create_info) = 0;
    };
}
