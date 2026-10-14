//
// Created by stuka on 12.10.2026.
//

#include "pixelfield/rhi/slang/slang_compiled_shader.h"

void pf::rhi::SlangCompiledShader::getIR(ShaderStageType stage_type, const void** ir_buffer, size_t* ir_size)
{
    *ir_buffer = entry_points[stage_type].spirv_code->getBufferPointer();
    *ir_size = entry_points[stage_type].spirv_code->getBufferSize();
}