//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <string>
#include <unordered_map>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IGPUBuffer.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    /// The engine's shared uniform blocks (CameraData, ProgramDataBlock, SpotLightsBlock,
    /// AtmosphereBlock) registered by block name.
    ///
    /// On GL they live at fixed binding points chosen by `IUniformBuffer::setLayoutLocation`, but
    /// the shader sources declare them without an explicit `layout(binding)`, so on Vulkan the
    /// vulkanizer assigns their bindings itself and the layout location means nothing. The join is
    /// therefore by name: VkUniformBuffer registers here, VkShader::buildDescriptorSet() looks up
    /// every reflected uniform block that is not one of its own SGLegacyUniforms_* blocks.
    struct SGCORE_EXPORT SharedUniformBuffers
    {
        static void set(const std::string& blockName, Ref<IGPUBuffer> buffer) noexcept;
        /// nullptr when no buffer was registered under that block name.
        [[nodiscard]] static const Ref<IGPUBuffer>& get(const std::string& blockName) noexcept;
        static void remove(const std::string& blockName) noexcept;
        static void clear() noexcept;

    private:
        static inline std::unordered_map<std::string, Ref<IGPUBuffer>> s_buffers;
    };
}
