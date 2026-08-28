//
// Created by 8bitniksis on 17.08.2026.
//

#include "RHIShaderLoader.h"

#include "IDevice.h"
#include "SGCore/Graphics/SPIRV/SPIRVCompiler.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Utils/SGSL/SGSLEVulkanizer.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Utils/Utils.h"

SGCore::RHIShaderLoader::Result SGCore::RHIShaderLoader::load(IDevice& device, const InterpolatedPath& shaderPath,
                                                              const std::vector<ShaderDefine>& defines) noexcept
{
    Result result;

    const auto analyzed = AssetManager::getInstance()->loadAsset<ShaderAnalyzedFile>(shaderPath);
    if(!analyzed || analyzed->getSubShaders().empty())
    {
        result.m_log = "shader '" + Utils::toUTF8(shaderPath.resolved()) + "' has no stages after translation";
        SG_LOG_E("RHIShaderLoader: {}", result.m_log);
        return result;
    }

    const auto& properties = device.getProperties();
    const bool openGL = isOpenGLAPI(properties.m_apiType);

    // the same variant selection the legacy GL backend does in createShader()
    std::string definesCode;
    if(openGL)
    {
        definesCode += properties.m_apiType == SG_API_TYPE_GLES3 || properties.m_apiType == SG_API_TYPE_GLES2
                       ? "#define SG_GLES32 \n" : "#define SG_GLSL4 \n";
    }
    else
    {
        // explicit APIs consume the GLSL4 variant through the vulkanizer
        definesCode += "#define SG_GLSL4 \n";
    }
    for(const auto& define : defines) definesCode += define.toString() + "\n";
    for(const auto& [name, value] : analyzed->getAttributes()) definesCode += "#define " + name + " " + value + "\n";

    std::vector<SGSLEVulkanizer::Stage> stages;
    for(const auto& subShader : analyzed->getSubShaders())
    {
        stages.push_back({ subShader.getType(), definesCode + subShader.getCode() });
    }

    SGSLEVulkanizer::Config vulkanizeConfig;
    vulkanizeConfig.m_target = openGL ? SGSLEVulkanizer::Target::OPENGL : SGSLEVulkanizer::Target::VULKAN;
    const auto report = SGSLEVulkanizer::vulkanize(stages, vulkanizeConfig);
    for(const auto& warning : report.m_warnings)
    {
        SG_LOG_W("RHIShaderLoader '{}': {}", Utils::toUTF8(shaderPath.resolved()), warning);
    }

    ShaderProgramDesc programDesc;
    programDesc.m_debugName = Utils::toUTF8(shaderPath.resolved().filename());
    for(const auto& stage : stages)
    {
        programDesc.m_stages.push_back({ stage.m_type, stage.m_code, { } });
    }

    if(!openGL)
    {
        std::vector<SPIRVCompiler::StageSource> sources;
        for(const auto& stage : stages) sources.push_back({ stage.m_type, stage.m_code });
        SPIRVCompiler::Options options;
        options.m_programName = programDesc.m_debugName;
        const auto compiled = SPIRVCompiler::compile(sources, options);
        if(!compiled.m_success)
        {
            result.m_log = compiled.m_log;
            SG_LOG_E("RHIShaderLoader: SPIR-V compilation of '{}' failed:\n{}", programDesc.m_debugName, compiled.m_log);
            return result;
        }
        for(std::size_t i = 0; i < compiled.m_stages.size() && i < programDesc.m_stages.size(); ++i)
        {
            programDesc.m_stages[i].m_spirv = compiled.m_stages[i].m_spirv;
        }
    }

    result.m_program = device.createShaderProgram(programDesc);
    if(!result.ok())
    {
        result.m_log = result.m_program ? result.m_program->getLog() : "createShaderProgram returned null";
    }
    return result;
}
