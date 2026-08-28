//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <sgcore_export.h>

#include "IGPUObject.h"
#include "RHITypes.h"
#include "SGCore/Graphics/SPIRV/ShaderReflection.h"

namespace SGCore
{
    /// A linked shader program (all stages) as the device sees it: created from
    /// SGSLEVulkanizer output for the device's dialect (GLSL for GL, SPIR-V for Vulkan/DX12).
    /// The reflection describes its binding interface; consumers use it to build descriptor
    /// sets and to address legacy uniform block members by name.
    class SGCORE_EXPORT IShaderProgram : public IGPUObject
    {
    public:
        [[nodiscard]] const ShaderReflection& getReflection() const noexcept { return m_reflection; }
        [[nodiscard]] virtual bool isValid() const noexcept = 0;
        /// Compile / link log; empty when clean.
        [[nodiscard]] const std::string& getLog() const noexcept { return m_log; }

    protected:
        ShaderReflection m_reflection;
        std::string m_log;
    };
}
