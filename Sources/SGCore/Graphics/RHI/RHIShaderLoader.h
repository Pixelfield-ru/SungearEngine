//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <string>
#include <vector>
#include <sgcore_export.h>

#include "IShaderProgram.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"

namespace SGCore
{
    class IDevice;

    /**
     * Turns an .sgshader into an IShaderProgram of a device: SGSL translation (via the
     * ShaderAnalyzedFile asset), defines of the backend and of the shader's #attribute's,
     * SGSLEVulkanizer in the dialect the device speaks, then IDevice::createShaderProgram.
     * This is the path every RHI render pass takes; IShader will delegate to it during
     * the migration.
     */
    struct SGCORE_EXPORT RHIShaderLoader
    {
        struct Result
        {
            Ref<IShaderProgram> m_program;
            std::string m_log;
            [[nodiscard]] bool ok() const noexcept { return m_program && m_program->isValid(); }
        };

        /**
         * @param defines Extra defines prepended to every stage (backend defines such as SG_GLSL4
         *                are added by the loader from the device's API type).
         */
        [[nodiscard]] static Result load(IDevice& device, const InterpolatedPath& shaderPath,
                                         const std::vector<ShaderDefine>& defines = { }) noexcept;
    };
}
