//
// Created by stuka on 03.09.2026.
//

#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>
#include <memory>

#include "shader_stage.h"

namespace pf::rhi
{
    struct ICompiledShader;
    struct IContext;

    struct IShaderCompiler
    {
        std::unordered_map<ShaderStageType, std::string> standard_entry_points {
            { ShaderStageType::kVertex, "vertexMain" },
            { ShaderStageType::kFragment, "fragmentMain" },
            { ShaderStageType::kCompute, "computeMain" },
            { ShaderStageType::kTessellationControl, "tesselationControlMain" },
            { ShaderStageType::kTessellationEval, "tesselationEvalMain" },
            { ShaderStageType::kGeometry, "geometryMain" }
        };

        std::vector<std::filesystem::path> search_paths;
        std::filesystem::path cache_path = "cache/shaders";

        virtual ~IShaderCompiler() = default;

        virtual std::error_code compileShader(const IContext& context, ICompiledShader& compiled_shader,
                                              const std::filesystem::path& shader_path) = 0;
    };
}