//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKSHADER_H
#define SUNGEARENGINE_VKSHADER_H

#include "SGCore/Graphics/RHI/RHILegacyShader.h"

namespace SGCore
{
    /// The Vulkan half of the shared legacy shader facade: where the "currently bound" shader is
    /// kept, and the dummy texel buffer a declared samplerBuffer needs (see RHILegacyShader).
    class VkShader final : public RHILegacyShader
    {
        friend class VkRenderer;

    public:
        ~VkShader() noexcept override = default;

    protected:
        VkShader() noexcept = default;

        void setAsCurrentShader() const noexcept override;
        void fillBackendDescriptors(IDescriptorSet& set) noexcept override;
    };
}

#endif //SUNGEARENGINE_VKSHADER_H
