//
// Created by 8bitniksis on 17.08.2026.
//

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "SGCore/Graphics/SPIRV/SPIRVCompiler.h"
#include "SGCore/Utils/SGSL/SGSLETranslator.h"
#include "SGCore/Utils/SGSL/SGSLEVulkanizer.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"

namespace
{
    using SGCore::SGSLEVulkanizer;

    int g_failures = 0;

    void check(bool condition, const char* what) noexcept
    {
        if(!condition)
        {
            ++g_failures;
            std::printf("  FAIL: %s\n", what);
        }
    }

    bool contains(const std::string& text, std::string_view needle) noexcept
    {
        return text.find(needle) != std::string::npos;
    }

    // ---------------------------------------------------------------- unit checks on snippets

    void testLooseUniformsBecomeBlock()
    {
        std::printf("[loose uniforms -> block]\n");

        std::vector<SGSLEVulkanizer::Stage> stages = {
            { SGCore::SST_VERTEX,
              "uniform mat4 u_model;\n"
              "layout(std140) uniform CameraData { mat4 view; };\n"
              "void main() { gl_Position = view * u_model * vec4(1.0); }\n" },
            { SGCore::SST_FRAGMENT,
              "uniform vec4 u_color;\n"
              "uniform mat4 u_model;\n"
              "uniform sampler2D u_tex;\n"
              "void main() { gl_FragColor = u_color * texture(u_tex, vec2(0.0)); }\n" },
        };

        const auto report = SGSLEVulkanizer::vulkanize(stages, { });

        check(report.m_looseUniformsMoved == 3, "3 loose uniforms moved (u_model x2, u_color)");
        check(report.m_uniformBlocksBound == 1, "CameraData bound");
        check(report.m_opaqueUniformsBound == 1, "u_tex bound");
        check(report.m_fragColorReplaced == 1, "gl_FragColor replaced");

        const auto& vs = stages[0].m_code;
        const auto& fs = stages[1].m_code;

        check(!contains(vs, "uniform mat4 u_model;"), "loose uniform removed from VS");
        check(contains(vs, "uniform SGLegacyUniforms_vertex"), "per-stage legacy block present in VS");
        check(contains(vs, "mat4 u_model;"), "u_model is a block member in VS");
        check(!contains(vs, "vec4 u_color;"), "VS block holds only VS uniforms (no u_color)");
        check(contains(vs, "layout(std140, set = 0, binding = "), "legacy block has set/binding");
        check(contains(vs, "layout(set = 0, binding = ") && contains(vs, "uniform CameraData"), "CameraData got set/binding merged into layout");

        check(contains(fs, "uniform SGLegacyUniforms_fragment"), "per-stage legacy block present in FS");
        check(contains(fs, "vec4 u_color;") && contains(fs, "mat4 u_model;"), "FS block holds u_color and its own u_model copy");
        check(contains(fs, "} sg_SGLegacyUniforms_fragment;"), "FS block has an instance name");
        check(contains(vs, "sg_SGLegacyUniforms_vertex.u_model * vec4"), "VS reference rewritten to instance.member");
        check(contains(fs, "layout(set = 0, binding = ") && contains(fs, "uniform sampler2D u_tex;"), "sampler got layout");
        check(contains(fs, "layout(location = 0) out vec4 sgFragColor;"), "fragment out declared");
        check(contains(fs, "sgFragColor = sg_SGLegacyUniforms_fragment.u_color"), "gl_FragColor usage rewritten, member qualified");
        check(!contains(fs, "gl_FragColor"), "no gl_FragColor left");

        // every resource of the program gets its own binding number
        std::uint32_t vsLegacy = 99, fsLegacy = 99, cameraBinding = 99, texBinding = 99;
        for(const auto& binding : report.m_bindings)
        {
            if(binding.m_name == "SGLegacyUniforms_vertex") vsLegacy = binding.m_binding;
            if(binding.m_name == "SGLegacyUniforms_fragment") fsLegacy = binding.m_binding;
            if(binding.m_name == "CameraData") cameraBinding = binding.m_binding;
            if(binding.m_name == "u_tex") texBinding = binding.m_binding;
        }
        check(vsLegacy != fsLegacy && vsLegacy != cameraBinding && fsLegacy != texBinding && cameraBinding != texBinding, "bindings are distinct");
        check(contains(vs, "binding = " + std::to_string(vsLegacy) + ") uniform SGLegacyUniforms_vertex"), "VS uses table binding for its block");
        check(contains(fs, "binding = " + std::to_string(fsLegacy) + ") uniform SGLegacyUniforms_fragment"), "FS uses table binding for its block");

        // line count preserved where uniforms were removed (only the block adds lines)
        check(report.m_warnings.empty(), "no warnings");
        for(const auto& warning : report.m_warnings) std::printf("  warning: %s\n", warning.c_str());
    }

    void testConditionalUniformsAndExplicitBindings()
    {
        std::printf("[conditional uniforms, explicit bindings, arrays, initializers]\n");

        std::vector<SGSLEVulkanizer::Stage> stages = {
            { SGCore::SST_FRAGMENT,
              "#define SG_HAS_SHADOWS 1\n"
              "uniform float u_always;\n"
              "#ifdef SG_HAS_SHADOWS\n"
              "uniform float u_shadowBias;\n"
              "#else\n"
              "uniform float u_noShadows;\n"
              "#endif\n"
              "#if SG_LEVEL > 2\n"
              "uniform int u_level;\n"
              "#endif\n"
              "uniform vec4 u_init = vec4(1.0);\n"
              "layout(binding = 5) uniform sampler2D u_explicit;\n"
              "uniform sampler2D u_array[3];\n"
              "layout(std140) uniform Data { float d; };\n"
              "void main() { }\n" },
        };

        SGSLEVulkanizer::Config config;
        config.m_descriptorSet = 2;
        const auto report = SGSLEVulkanizer::vulkanize(stages, config);
        const auto& code = stages[0].m_code;

        check(report.m_looseUniformsMoved == 5, "5 loose uniforms moved");
        check(report.m_initializersDropped == 1, "1 initializer dropped");
        check(contains(code, "#if (defined(SG_HAS_SHADOWS))\n    float u_shadowBias;\n#endif"), "ifdef preserved as block member guard");
        check(contains(code, "#if (!(defined(SG_HAS_SHADOWS)))\n    float u_noShadows;\n#endif"), "else preserved as negated guard");
        check(contains(code, "#if (SG_LEVEL > 2)\n    int u_level;\n#endif"), "#if expr preserved");
        check(contains(code, "    vec4 u_init;\n"), "initializer stripped from block member");
        check(contains(code, "layout(set = 2, binding = 5) uniform sampler2D u_explicit;"), "explicit binding kept, set added");
        check(contains(code, "uniform sampler2D u_array[3];"), "array sampler kept");
        check(contains(code, "layout(std140, set = 2, binding = "), "legacy block in set 2");

        bool arrayCountOk = false;
        for(const auto& binding : report.m_bindings)
        {
            if(binding.m_name == "u_array") arrayCountOk = binding.m_count == 3;
            if(binding.m_name == "u_explicit") check(binding.m_binding == 5, "u_explicit reported as binding 5");
        }
        check(arrayCountOk, "u_array count = 3");

        // nothing else may have taken binding 5
        for(const auto& binding : report.m_bindings)
        {
            if(binding.m_name != "u_explicit") check(binding.m_binding != 5, "binding 5 reserved for u_explicit");
        }

        for(const auto& warning : report.m_warnings) std::printf("  warning: %s\n", warning.c_str());
    }

    void testBuiltinsAndFunctionsUntouched()
    {
        std::printf("[builtins, function bodies untouched]\n");

        std::vector<SGSLEVulkanizer::Stage> stages = {
            { SGCore::SST_VERTEX,
              "struct S { float a; };\n"
              "float helper(float x) { float local = x; return local; }\n"
              "void main() { int id = gl_VertexID + gl_InstanceID; }\n" },
        };

        const auto report = SGSLEVulkanizer::vulkanize(stages, { });
        const auto& code = stages[0].m_code;

        check(report.m_looseUniformsMoved == 0, "no uniforms, nothing moved");
        check(!contains(code, "SGLegacyUniforms"), "no legacy block emitted");
        check(contains(code, "gl_VertexIndex + gl_InstanceIndex"), "builtins renamed");
        check(report.m_builtinsReplaced == 2, "2 builtins replaced");
        check(contains(code, "float helper(float x) { float local = x; return local; }"), "function untouched");
    }

    // ---------------------------------------------------------------- corpus run

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

    std::string readFile(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // ---------------------------------------------------------------- SPIR-V compile of a snippet program

    void testSpirvCompileAndReflect()
    {
        std::printf("[spirv compile + reflection]\n");

        std::vector<SGSLEVulkanizer::Stage> stages = {
            { SGCore::SST_VERTEX,
              "layout(location = 0) in vec3 positionsAttribute;\n"
              "uniform mat4 u_model;\n"
              "layout(std140) uniform CameraData { mat4 view; mat4 projection; };\n"
              "out vec2 vs_uv;\n"
              "void main() { vs_uv = positionsAttribute.xy; gl_Position = projection * view * u_model * vec4(positionsAttribute, 1.0); }\n" },
            { SGCore::SST_FRAGMENT,
              "uniform vec4 u_color;\n"
              "uniform float u_factors[4];\n"
              "uniform sampler2D u_tex[2];\n"
              "in vec2 vs_uv;\n"
              "void main() { gl_FragColor = u_color * texture(u_tex[1], vs_uv) * u_factors[2]; }\n" },
        };

        std::ignore = SGSLEVulkanizer::vulkanize(stages, { });

        std::vector<SGCore::SPIRVCompiler::StageSource> sources;
        for(const auto& stage : stages) sources.push_back({ stage.m_type, stage.m_code });

        SGCore::SPIRVCompiler::Options options;
        options.m_programName = "snippet";
        const auto result = SGCore::SPIRVCompiler::compile(sources, options);

        check(result.m_success, "snippet program compiles to SPIR-V");
        if(!result.m_success) { std::printf("%s\n", result.m_log.c_str()); return; }

        check(result.m_stages.size() == 2 && !result.m_stages[0].m_spirv.empty() && !result.m_stages[1].m_spirv.empty(), "two SPIR-V modules");

        const auto& reflection = result.m_reflection;
        const auto* vsLegacy = reflection.findBinding("SGLegacyUniforms_vertex");
        const auto* fsLegacy = reflection.findBinding("SGLegacyUniforms_fragment");
        check(vsLegacy != nullptr && fsLegacy != nullptr, "per-stage legacy blocks reflected");
        if(vsLegacy && fsLegacy)
        {
            check(vsLegacy->m_type == SGCore::ShaderDescriptorType::UNIFORM_BUFFER, "legacy block is a uniform buffer");
            check(vsLegacy->m_stages == (1u << SGCore::SST_VERTEX), "vertex block used in vertex stage only");
            check(fsLegacy->m_stages == (1u << SGCore::SST_FRAGMENT), "fragment block used in fragment stage only");
            const auto* model = reflection.findMember("SGLegacyUniforms_vertex", "u_model");
            const auto* color = reflection.findMember("SGLegacyUniforms_fragment", "u_color");
            const auto* factors = reflection.findMember("SGLegacyUniforms_fragment", "u_factors");
            check(model && model->m_offset == 0 && model->m_size == 64, "u_model at offset 0, 64 bytes");
            check(color && color->m_offset == 0 && color->m_size == 16, "u_color at offset 0 of the fragment block");
            check(factors && factors->m_arrayCount == 4 && factors->m_offset == 16, "u_factors[4] at 16 (std140 array stride 16)");
            check(fsLegacy->m_blockSize >= 80, "fragment block size covers all members");
        }

        const auto* camera = reflection.findBinding("CameraData");
        check(camera && camera->m_members.size() == 2 && camera->m_blockSize == 128, "CameraData: 2 mat4, 128 bytes");

        const auto* tex = reflection.findBinding("u_tex");
        check(tex && tex->m_type == SGCore::ShaderDescriptorType::COMBINED_IMAGE_SAMPLER && tex->m_count == 2, "u_tex[2] is a combined image sampler array");

        check(reflection.m_vertexInputs.size() == 1 && reflection.m_vertexInputs[0].m_name == "positionsAttribute" &&
              reflection.m_vertexInputs[0].m_location == 0, "vertex input reflected");

        if(!result.m_log.empty()) std::printf("  log: %s\n", result.m_log.c_str());
    }

    int runCorpus(const std::filesystem::path& root, const std::filesystem::path& outDir, bool compileSpirv)
    {
        const auto shadersDir = root / "Resources" / "sg_shaders";
        if(!std::filesystem::exists(shadersDir))
        {
            std::printf("corpus: '%s' does not exist\n", shadersDir.string().c_str());
            return 2;
        }

        SGCore::SGSLETranslator::includeDirectory(root / "Resources");
        std::filesystem::create_directories(outDir);

        std::size_t files = 0, stages = 0, moved = 0, opaque = 0, blocks = 0, warnings = 0;
        std::size_t spirvOk = 0, spirvFailed = 0;
        std::vector<std::string> spirvFailures;

        // the same defines the GL4 backend prepends on PC (GL4Renderer::createShader) plus the
        // shader's own #attribute defines, so #if branches match a real compilation
        const std::string baseDefines = "#define SG_GLSL4 \n";

        for(const auto& entry : std::filesystem::recursive_directory_iterator(shadersDir))
        {
            if(!entry.is_regular_file() || entry.path().extension() != ".sgshader") continue;
            ++files;

            SGCore::SGSLETranslator translator;
            translator.m_config.m_useOutputDebug = false;
            SGCore::ShaderAnalyzedFile analyzed;
            translator.processCode(entry.path(), readFile(entry.path()), &analyzed);

            std::string defines = baseDefines;
            for(const auto& [name, value] : analyzed.getAttributes()) defines += "#define " + name + " " + value + "\n";

            std::vector<SGSLEVulkanizer::Stage> vkStages;
            for(const auto& subShader : analyzed.getSubShaders())
            {
                vkStages.push_back({ subShader.getType(), defines + subShader.getCode() });
            }

            const auto report = SGSLEVulkanizer::vulkanize(vkStages, { });

            const auto relative = std::filesystem::relative(entry.path(), shadersDir).string();
            std::string baseName = relative;
            for(auto& c : baseName) if(c == '/' || c == '\\') c = '_';

            std::printf("%-60s stages=%zu moved=%u opaque=%u blocks=%u fragColor=%u warnings=%zu\n",
                        relative.c_str(), vkStages.size(), report.m_looseUniformsMoved, report.m_opaqueUniformsBound,
                        report.m_uniformBlocksBound, report.m_fragColorReplaced, report.m_warnings.size());
            for(const auto& warning : report.m_warnings) std::printf("    warning: %s\n", warning.c_str());

            for(const auto& stage : vkStages)
            {
                std::ofstream out(outDir / (baseName + "." + stageName(stage.m_type) + ".glsl"), std::ios::binary);
                out << stage.m_code;
            }

            stages += vkStages.size();
            moved += report.m_looseUniformsMoved;
            opaque += report.m_opaqueUniformsBound;
            blocks += report.m_uniformBlocksBound;
            warnings += report.m_warnings.size();

            if(compileSpirv && !vkStages.empty())
            {
                std::vector<SGCore::SPIRVCompiler::StageSource> sources;
                for(const auto& stage : vkStages) sources.push_back({ stage.m_type, stage.m_code });

                SGCore::SPIRVCompiler::Options options;
                options.m_programName = relative;
                const auto result = SGCore::SPIRVCompiler::compile(sources, options);

                if(result.m_success)
                {
                    ++spirvOk;
                    std::printf("    spirv: OK — %zu bindings, %zu vertex inputs, %zu push ranges\n",
                                result.m_reflection.m_bindings.size(), result.m_reflection.m_vertexInputs.size(),
                                result.m_reflection.m_pushConstants.size());
                    for(const auto& stage : result.m_stages)
                    {
                        std::ofstream out(outDir / (baseName + "." + stageName(stage.m_type) + ".spv"), std::ios::binary);
                        out.write(reinterpret_cast<const char*>(stage.m_spirv.data()),
                                  static_cast<std::streamsize>(stage.m_spirv.size() * sizeof(std::uint32_t)));
                    }
                }
                else
                {
                    ++spirvFailed;
                    spirvFailures.push_back(relative);
                    std::printf("    spirv: FAILED\n%s\n", result.m_log.c_str());
                }
            }
        }

        std::printf("\ncorpus: %zu files, %zu stages, %zu loose uniforms moved, %zu opaque bound, %zu blocks bound, %zu warnings\n"
                    "vulkanized GLSL written to '%s'\n",
                    files, stages, moved, opaque, blocks, warnings, outDir.string().c_str());
        if(compileSpirv)
        {
            std::printf("spirv: %zu programs OK, %zu failed\n", spirvOk, spirvFailed);
            for(const auto& name : spirvFailures) std::printf("  failed: %s\n", name.c_str());
        }

        return files == 0 ? 2 : 0;
    }
}

int main(int argc, char** argv)
{
    std::filesystem::path corpusRoot;
    std::filesystem::path outDir = "vulkanized";
    bool compileSpirv = false;

    for(int i = 1; i < argc; ++i)
    {
        const std::string_view arg = argv[i];
        if(arg == "--corpus" && i + 1 < argc) corpusRoot = argv[++i];
        else if(arg == "--out" && i + 1 < argc) outDir = argv[++i];
        else if(arg == "--spirv") compileSpirv = true;
        else if(arg == "--help")
        {
            std::puts("SGShadersTest [--corpus <engine root>] [--out <dir>] [--spirv]\n"
                      "  without --corpus: unit checks of the SGSL vulkanizer and SPIR-V compiler on built-in snippets\n"
                      "  with --corpus: additionally translates every Resources/sg_shaders/**/*.sgshader,\n"
                      "  vulkanizes it and dumps the result to <dir> (default ./vulkanized)\n"
                      "  --spirv: also compiles every corpus program to SPIR-V (glslang) and reflects it");
            return 0;
        }
    }

    testLooseUniformsBecomeBlock();
    testConditionalUniformsAndExplicitBindings();
    testBuiltinsAndFunctionsUntouched();
    testSpirvCompileAndReflect();

    std::printf("\nunit checks: %s (%d failure(s))\n", g_failures == 0 ? "PASS" : "FAIL", g_failures);

    int corpusResult = 0;
    if(!corpusRoot.empty())
    {
        corpusResult = runCorpus(corpusRoot, outDir, compileSpirv);
    }

    return g_failures == 0 && corpusResult == 0 ? 0 : 1;
}
