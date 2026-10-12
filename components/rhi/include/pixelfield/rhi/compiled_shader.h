//
// Created by stuka on 03.09.2026.
//

#pragma once

#include <vector>

#include "shader_stage.h"

namespace pf::rhi
{
    struct ICompiledShader
    {
        std::vector<ShaderStage> stages;

        virtual ~ICompiledShader() = default;

        virtual void getIR(ShaderStageType stage_type, const void** ir_buffer, size_t* ir_buffer_size) = 0;
    };
}
