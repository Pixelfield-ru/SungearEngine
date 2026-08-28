//
// Created by 8bitniksis on 17.08.2026.
//

#include "SPIRVCompiler.h"

#include <memory>
#include <mutex>
#include <unordered_map>

#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/SPIRV/Logger.h>
#include <spirv_reflect.h>

namespace
{
    // glslang requires one InitializeProcess() per process; FinalizeProcess() at exit is optional
    // and left out on purpose: static destruction order against other subsystems is not worth it.
    void ensureGlslangInitialized() noexcept
    {
        static std::once_flag once;
        std::call_once(once, [] { glslang::InitializeProcess(); });
    }

    EShLanguage toGlslangStage(SGCore::SGSLESubShaderType type) noexcept
    {
        switch(type)
        {
            case SGCore::SST_VERTEX: return EShLangVertex;
            case SGCore::SST_FRAGMENT: return EShLangFragment;
            case SGCore::SST_GEOMETRY: return EShLangGeometry;
            case SGCore::SST_COMPUTE: return EShLangCompute;
            case SGCore::SST_TESS_CONTROL: return EShLangTessControl;
            case SGCore::SST_TESS_EVALUATION: return EShLangTessEvaluation;
            default: return EShLangCount;
        }
    }

    const char* stageName(SGCore::SGSLESubShaderType type) noexcept
    {
        switch(type)
        {
            case SGCore::SST_VERTEX: return "vertex";
            case SGCore::SST_FRAGMENT: return "fragment";
            case SGCore::SST_GEOMETRY: return "geometry";
            case SGCore::SST_COMPUTE: return "compute";
            case SGCore::SST_TESS_CONTROL: return "tess_control";
            case SGCore::SST_TESS_EVALUATION: return "tess_eval";
            default: return "unknown";
        }
    }

    SGCore::ShaderDescriptorType toDescriptorType(SpvReflectDescriptorType type) noexcept
    {
        switch(type)
        {
            case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return SGCore::ShaderDescriptorType::UNIFORM_BUFFER;
            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return SGCore::ShaderDescriptorType::STORAGE_BUFFER;
            case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return SGCore::ShaderDescriptorType::COMBINED_IMAGE_SAMPLER;
            case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return SGCore::ShaderDescriptorType::SAMPLED_IMAGE;
            case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: return SGCore::ShaderDescriptorType::SAMPLER;
            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE: return SGCore::ShaderDescriptorType::STORAGE_IMAGE;
            case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return SGCore::ShaderDescriptorType::UNIFORM_TEXEL_BUFFER;
            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return SGCore::ShaderDescriptorType::STORAGE_TEXEL_BUFFER;
            case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return SGCore::ShaderDescriptorType::INPUT_ATTACHMENT;
            default: return SGCore::ShaderDescriptorType::UNKNOWN;
        }
    }

    std::uint32_t arrayCount(const SpvReflectArrayTraits& array) noexcept
    {
        std::uint32_t count = 1;
        for(std::uint32_t i = 0; i < array.dims_count; ++i)
        {
            count *= array.dims[i] == 0 ? 1 : array.dims[i];
        }
        return count;
    }

    std::uint32_t arrayCount(const SpvReflectBindingArrayTraits& array) noexcept
    {
        std::uint32_t count = 1;
        for(std::uint32_t i = 0; i < array.dims_count; ++i)
        {
            count *= array.dims[i] == 0 ? 1 : array.dims[i];
        }
        return count;
    }

    /// Appends the leaves of one block member, flattening nested structs into dotted names.
    ///
    /// The engine addresses uniforms the way GL reports them — leaf paths like
    /// "objectTransform.modelMatrix" (GL enumerates GL_UNIFORM resources, which are always leaves).
    /// SPIRV-Reflect instead reports the top-level member "objectTransform" with its own nested
    /// members, so without this flattening every write to a nested uniform silently misses and the
    /// shader reads zeros (observed as an empty frame on Vulkan, 2026-08-17).
    void appendMemberLeaves(const SpvReflectBlockVariable& source, const std::string& prefix,
                            std::vector<SGCore::ShaderReflection::BlockMember>& out) noexcept
    {
        const std::string name = prefix + (source.name ? source.name : "");
        const bool isStruct = source.member_count > 0 && source.members;
        const bool isArray = source.array.dims_count > 0;

        // an array of structs would need per-element paths; the engine has none in these blocks, so
        // it is reported as a single member rather than guessed at
        if(isStruct && !isArray)
        {
            for(std::uint32_t i = 0; i < source.member_count; ++i)
            {
                appendMemberLeaves(source.members[i], name + ".", out);
            }
            return;
        }

        SGCore::ShaderReflection::BlockMember member;
        member.m_name = name;
        member.m_typeName = source.type_description && source.type_description->type_name ? source.type_description->type_name : "";
        // offsets of nested members are relative to their parent; the engine needs them relative to
        // the block, which is what absolute_offset carries
        member.m_offset = source.absolute_offset;
        member.m_size = source.size;
        member.m_arrayCount = arrayCount(source.array);
        // m_paddedSize means "stride of one element" for arrays (see ShaderReflection):
        // SPIRV-Reflect's padded_size is the padded size of the whole member instead, so an
        // array takes its stride from the array traits — otherwise "name[i]" addressing
        // walks past the end of the block
        member.m_paddedSize = isArray && source.array.stride > 0 ? source.array.stride : source.padded_size;
        out.push_back(std::move(member));
    }

    std::vector<SGCore::ShaderReflection::BlockMember> collectMembers(const SpvReflectBlockVariable& block) noexcept
    {
        std::vector<SGCore::ShaderReflection::BlockMember> members;
        members.reserve(block.member_count);
        for(std::uint32_t i = 0; i < block.member_count; ++i)
        {
            appendMemberLeaves(block.members[i], "", members);
        }
        return members;
    }

    struct ReflectModule
    {
        SpvReflectShaderModule m_module { };
        bool m_created { };

        ~ReflectModule()
        {
            if(m_created) spvReflectDestroyShaderModule(&m_module);
        }
    };
}

SGCore::SPIRVCompiler::Result SGCore::SPIRVCompiler::compile(const std::vector<StageSource>& stages, const Options& options) noexcept
{
    ensureGlslangInitialized();

    Result result;
    result.m_success = true;

    // shaders must outlive the program that links them
    std::vector<std::unique_ptr<glslang::TShader>> shaders;
    std::vector<std::string> sources;
    glslang::TProgram program;

    const auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

    for(const auto& stage : stages)
    {
        const EShLanguage language = toGlslangStage(stage.m_type);
        if(language == EShLangCount)
        {
            result.m_log += std::string("[") + options.m_programName + "] unsupported stage type\n";
            result.m_success = false;
            continue;
        }

        std::string source = stage.m_code;
        if(source.rfind("#version", 0) != 0)
        {
            source = "#version " + std::to_string(options.m_glslVersion) + "\n" + source;
        }
        sources.push_back(std::move(source));
        const char* sourcePtr = sources.back().c_str();

        auto shader = std::make_unique<glslang::TShader>(language);
        shader->setStrings(&sourcePtr, 1);
        shader->setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
        shader->setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
        shader->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_6);
        shader->setEntryPoint(options.m_entryPoint.c_str());
        // bindings come from SGSLEVulkanizer; locations of user in/out varyings are assigned by
        // glslang consistently across the linked stages (mapIO below)
        shader->setAutoMapLocations(true);
        shader->setAutoMapBindings(true);

        StageResult stageResult;
        stageResult.m_type = stage.m_type;

        if(!shader->parse(GetDefaultResources(), static_cast<int>(options.m_glslVersion), ECoreProfile, false, false, messages))
        {
            stageResult.m_log = shader->getInfoLog();
            stageResult.m_success = false;
            result.m_log += std::string("[") + options.m_programName + " / " + stageName(stage.m_type) + "] parse failed:\n" + stageResult.m_log + "\n";
            result.m_success = false;
            result.m_stages.push_back(std::move(stageResult));
            continue;
        }

        program.addShader(shader.get());
        shaders.push_back(std::move(shader));
        result.m_stages.push_back(std::move(stageResult));
    }

    if(!result.m_success) return result;

    if(!program.link(messages) || !program.mapIO())
    {
        result.m_log += std::string("[") + options.m_programName + "] link failed:\n" + program.getInfoLog() + "\n";
        result.m_success = false;
        return result;
    }

    for(auto& stageResult : result.m_stages)
    {
        const EShLanguage language = toGlslangStage(stageResult.m_type);
        const glslang::TIntermediate* intermediate = program.getIntermediate(language);
        if(!intermediate)
        {
            stageResult.m_log = "no intermediate after link";
            stageResult.m_success = false;
            result.m_success = false;
            continue;
        }

        glslang::SpvOptions spvOptions;
        spvOptions.generateDebugInfo = options.m_generateDebugInfo;
        spvOptions.disableOptimizer = true;
        spvOptions.validate = true;

        spv::SpvBuildLogger logger;
        glslang::GlslangToSpv(*intermediate, stageResult.m_spirv, &logger, &spvOptions);

        const std::string messagesText = logger.getAllMessages();
        if(!messagesText.empty())
        {
            stageResult.m_log = messagesText;
            result.m_log += std::string("[") + options.m_programName + " / " + stageName(stageResult.m_type) + "] spirv:\n" + messagesText + "\n";
        }
        stageResult.m_success = !stageResult.m_spirv.empty();
        if(!stageResult.m_success) result.m_success = false;
    }

    if(!result.m_success) return result;

    std::string reflectLog;
    if(!reflect(result.m_stages, result.m_reflection, reflectLog))
    {
        result.m_log += std::string("[") + options.m_programName + "] reflection failed:\n" + reflectLog + "\n";
        result.m_success = false;
    }

    return result;
}

bool SGCore::SPIRVCompiler::reflect(const std::vector<StageResult>& stages, ShaderReflection& outReflection, std::string& outLog) noexcept
{
    outReflection = ShaderReflection { };

    // (set, binding) → index in outReflection.m_bindings
    std::unordered_map<std::uint64_t, std::size_t> bindingIndex;
    auto key = [](std::uint32_t set, std::uint32_t binding) { return (std::uint64_t(set) << 32) | binding; };

    for(const auto& stage : stages)
    {
        if(stage.m_spirv.empty()) continue;

        ReflectModule module;
        if(spvReflectCreateShaderModule(stage.m_spirv.size() * sizeof(std::uint32_t), stage.m_spirv.data(), &module.m_module) != SPV_REFLECT_RESULT_SUCCESS)
        {
            outLog += std::string("spvReflectCreateShaderModule failed for ") + stageName(stage.m_type) + "\n";
            return false;
        }
        module.m_created = true;

        const shader_stages_mask_t stageBit = 1u << static_cast<std::uint32_t>(stage.m_type);

        // descriptor bindings
        {
            std::uint32_t count = 0;
            spvReflectEnumerateDescriptorBindings(&module.m_module, &count, nullptr);
            std::vector<SpvReflectDescriptorBinding*> bindings(count);
            spvReflectEnumerateDescriptorBindings(&module.m_module, &count, bindings.data());

            for(const auto* source : bindings)
            {
                if(!source) continue;

                const auto k = key(source->set, source->binding);
                if(auto it = bindingIndex.find(k); it != bindingIndex.end())
                {
                    auto& existing = outReflection.m_bindings[it->second];
                    existing.m_stages |= stageBit;
                    if(existing.m_type != toDescriptorType(source->descriptor_type))
                    {
                        outLog += "binding (" + std::to_string(source->set) + ", " + std::to_string(source->binding) +
                                  ") has different descriptor types across stages\n";
                    }
                    continue;
                }

                ShaderReflection::DescriptorBinding binding;
                binding.m_set = source->set;
                binding.m_binding = source->binding;
                binding.m_type = toDescriptorType(source->descriptor_type);
                binding.m_count = arrayCount(source->array);
                binding.m_stages = stageBit;

                const bool isBlock = binding.m_type == ShaderDescriptorType::UNIFORM_BUFFER || binding.m_type == ShaderDescriptorType::STORAGE_BUFFER;
                const char* variableName = source->name;
                const char* typeName = source->type_description ? source->type_description->type_name : nullptr;

                if(isBlock)
                {
                    // blocks are addressed by their block (type) name — the instance name is an
                    // implementation detail (SGSLEVulkanizer generates `sg_<block>`) and anonymous
                    // blocks have none at all
                    binding.m_name = typeName && *typeName ? typeName : (variableName ? variableName : "");
                    binding.m_blockSize = source->block.size;
                    binding.m_members = collectMembers(source->block);
                    if(binding.m_name.empty() && source->block.name) binding.m_name = source->block.name;
                }
                else
                {
                    binding.m_name = variableName ? variableName : "";
                }

                bindingIndex.emplace(k, outReflection.m_bindings.size());
                outReflection.m_bindings.push_back(std::move(binding));
            }
        }

        // vertex inputs
        if(stage.m_type == SST_VERTEX)
        {
            std::uint32_t count = 0;
            spvReflectEnumerateInputVariables(&module.m_module, &count, nullptr);
            std::vector<SpvReflectInterfaceVariable*> inputs(count);
            spvReflectEnumerateInputVariables(&module.m_module, &count, inputs.data());

            for(const auto* input : inputs)
            {
                if(!input || (input->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN)) continue;
                ShaderReflection::VertexInput vertexInput;
                vertexInput.m_name = input->name ? input->name : "";
                vertexInput.m_location = input->location;
                vertexInput.m_format = static_cast<std::uint32_t>(input->format);
                // A matrix input takes one location per column, and the reflector reports only the
                // first of them. A pipeline that took the report literally dropped columns 1..3 of
                // the per-instance model matrix: Vulkan rasterized garbage transforms, DX12 refused
                // to create the pipeline at all (TEXCOORD1..3 in the signature, not in the layout).
                if(input->type_description &&
                   (input->type_description->type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX) &&
                   input->numeric.matrix.column_count > 0)
                {
                    vertexInput.m_locationsCount = input->numeric.matrix.column_count;
                }
                outReflection.m_vertexInputs.push_back(std::move(vertexInput));
            }
        }

        // push constants
        {
            std::uint32_t count = 0;
            spvReflectEnumeratePushConstantBlocks(&module.m_module, &count, nullptr);
            std::vector<SpvReflectBlockVariable*> blocks(count);
            spvReflectEnumeratePushConstantBlocks(&module.m_module, &count, blocks.data());

            for(const auto* block : blocks)
            {
                if(!block) continue;

                bool merged = false;
                for(auto& existing : outReflection.m_pushConstants)
                {
                    if(existing.m_offset == block->offset && existing.m_size == block->size)
                    {
                        existing.m_stages |= stageBit;
                        merged = true;
                        break;
                    }
                }
                if(merged) continue;

                ShaderReflection::PushConstantRange range;
                range.m_name = block->name ? block->name : "";
                range.m_offset = block->offset;
                range.m_size = block->size;
                range.m_members = collectMembers(*block);
                range.m_stages = stageBit;
                outReflection.m_pushConstants.push_back(std::move(range));
            }
        }
    }

    return true;
}
