//
// Created by stuka on 02.09.2026.
//

#include "pixelfield/rhi/slang/slang_compiler.h"

#include <array>
#include <iostream>

#include <slang.h>
#include <slang-com-helper.h>

#include "pixelfield/rhi/context.h"
#include "pixelfield/rhi/slang/slang_compiled_shader.h"
#include "pixelfield/core/errors.h"
#include "pixelfield/core/encoding.h"
#include "pixelfield/rhi/slang/slang_errors.h"

std::error_code pf::rhi::SlangShaderCompiler::compileShader(const IContext& context, ICompiledShader& compiled_shader,
                                                            const std::filesystem::path& shader_path)
{
    auto* slang_shader = dynamic_cast<SlangCompiledShader*>(&compiled_shader);
    if(!slang_shader)
    {
        const auto msg = std::format("Cannot compile shader '{}': incompatible compiler.", core::pathToUtf8(shader_path));
        context.printDebugMessage(LogLevel::kError, msg);
        return core::CoreError::kInvalidArgument;
    }

    slang_shader->stages.clear();

    slang::SessionDesc session_desc {};

    slang::TargetDesc target_desc {};
    target_desc.format = SLANG_SPIRV;
    target_desc.profile = globalSession()->findProfile("spirv_1_5");

    session_desc.targets = &target_desc;
    session_desc.targetCount = 1;

    std::array preprocessor_macro_desc {
        slang::PreprocessorMacroDesc { "SG_ENABLE_ZHOPA", "true" }
    };

    std::vector<std::string> search_path_storage;
    search_path_storage.reserve(search_paths.size());
    for (const auto& path : search_paths)
    {
        search_path_storage.push_back(path.generic_string());
    }

    std::vector<const char*> search_paths;
    search_paths.reserve(search_path_storage.size());
    for (const auto& s : search_path_storage)
    {
        search_paths.push_back(s.c_str());
    }

    session_desc.preprocessorMacros = preprocessor_macro_desc.data();
    session_desc.preprocessorMacroCount = preprocessor_macro_desc.size();
    session_desc.searchPaths = search_paths.data();
    session_desc.searchPathCount = search_paths.size();

    std::array options {
        slang::CompilerOptionEntry {
            slang::CompilerOptionName::EmitSpirvDirectly,
            { slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr }
        },
        slang::CompilerOptionEntry {
            slang::CompilerOptionName::UseUpToDateBinaryModule,
            { slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr }
        },
        slang::CompilerOptionEntry {
            slang::CompilerOptionName::VulkanUseEntryPointName,
            { slang::CompilerOptionValueKind::Int, 1, 0, nullptr, nullptr }
        }
    };

    session_desc.compilerOptionEntries = options.data();
    session_desc.compilerOptionEntryCount = options.size();

    globalSession()->createSession(session_desc, slang_shader->session.writeRef());

    {
        Slang::ComPtr<slang::IBlob> diagnostics_blob;

        const auto module_path = shader_path.string();

        // loading from file
        slang_shader->module = slang_shader->session->loadModule(module_path.c_str(), diagnostics_blob.writeRef());

        if (!slang_shader->module)
        {
            const auto msg = std::format(
                "Cannot load shader '{}': {}",
                core::pathToUtf8(shader_path),
                diagnostics_blob ? (const char*) diagnostics_blob->getBufferPointer() : "no diagnostics provided");

            context.printDebugMessage(LogLevel::kError, msg);
            return SlangError::kCannotLoadShader;
        }

        const auto cache_dir_path = cache_path / shader_path.parent_path();
        const auto cache_file_path = (cache_path / (shader_path.string() + ".slang-module")).string();
        std::filesystem::create_directories(cache_dir_path);
        slang_shader->module->writeToFile(cache_file_path.c_str());
    }

    const auto get_stage = [&](std::string entry_point_name, ShaderStageType type) -> std::pair<ShaderStage, Slang::ComPtr<slang::IEntryPoint>> {
        ShaderStage stage;
        stage.entry_point_name = std::move(entry_point_name);
        stage.type = type;

        Slang::ComPtr<slang::IEntryPoint> entry_point;

        slang_shader->module->findEntryPointByName(stage.entry_point_name.data(), entry_point.writeRef());
        if (!entry_point)
        {
            const auto msg = std::format(
                "Entry point '{}' was not found in shader '{}'.",
                stage.entry_point_name,
                core::pathToUtf8(shader_path));

            context.printDebugMessage(LogLevel::kTrace, msg);
        }

        return { stage, entry_point };
    };

    for(const auto& entry_point : standard_entry_points)
    {
        auto stage = get_stage(entry_point.second, entry_point.first);

        if(stage.second)
        {
            slang_shader->entry_points[stage.first.type].entry_point = std::move(stage.second);
            slang_shader->stages.push_back(std::move(stage.first));
        }
    }

    std::vector<slang::IComponentType*> component_types {
        slang_shader->module,
    };

   for(const auto& stage : slang_shader->stages)
   {
       component_types.push_back(slang_shader->entry_points[stage.type].entry_point);
   }

    // composing program
    Slang::ComPtr<slang::IComponentType> composed_program;
    {
        Slang::ComPtr<slang::IBlob> diagnostics_blob;
        const auto result = slang_shader->session->createCompositeComponentType(
            component_types.data(),
            component_types.size(),
            composed_program.writeRef(),
            diagnostics_blob.writeRef());
        if(SLANG_FAILED(result))
        {
            const auto msg = std::format(
                "Cannot compose shader '{}': {}",
                core::pathToUtf8(shader_path),
                diagnostics_blob ? (const char*) diagnostics_blob->getBufferPointer() : "no diagnostics provided");

            context.printDebugMessage(LogLevel::kError, msg);

            return SlangError::kCannotComposeShader;
        }
    }

    // linking program
    Slang::ComPtr<slang::IComponentType> linked_program;
    {
        Slang::ComPtr<slang::IBlob> diagnostics_blob;
        const auto result = composed_program->link(
            linked_program.writeRef(),
            diagnostics_blob.writeRef());

        if(SLANG_FAILED(result))
        {
            const auto msg = std::format(
                "Cannot link shader '{}': {}",
                core::pathToUtf8(shader_path),
                diagnostics_blob ? (const char*) diagnostics_blob->getBufferPointer() : "no diagnostics provided");

            context.printDebugMessage(LogLevel::kError, msg);

            return SlangError::kCannotLinkShader;
        }
    }

    // compiling program
    {
        for(size_t i = 0; i < slang_shader->stages.size(); ++i)
        {
            Slang::ComPtr<slang::IBlob> diagnostics_blob;
            SlangResult result = linked_program->getEntryPointCode(
                i, // entry_point_index
                0, // target_index
                slang_shader->entry_points[slang_shader->stages[i].type].spirv_code.writeRef(),
                diagnostics_blob.writeRef());

            if(SLANG_FAILED(result))
            {
                const auto msg = std::format(
                "Cannot compile shader '{}': {}",
                core::pathToUtf8(shader_path),
                diagnostics_blob ? (const char*) diagnostics_blob->getBufferPointer() : "no diagnostics provided");

                context.printDebugMessage(LogLevel::kError, msg);

                return SlangError::kCannotCompileShader;
            }
        }
    }

    return {};
}

Slang::ComPtr<slang::IGlobalSession> pf::rhi::SlangShaderCompiler::globalSession() noexcept
{
    static Slang::ComPtr<slang::IGlobalSession> globalSession;
    static volatile auto init = []() {
        createGlobalSession(globalSession.writeRef());
        return true;
    }();

    return globalSession;
}
