#include <SGCore/Logger/Logger.h>
#include "GL46Shader.h"

#include <charconv>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GL46/RHI/GL46ShaderProgram.h"
#include "SGCore/Graphics/API/GL/GL46/RHI/GL46ProgramBase.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/SGSL/SGSLESubShader.h"
#include "SGCore/Utils/SGSL/SGSLEVulkanizer.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

namespace
{
    // Binding points 1..4 belong to the legacy IUniformBuffer objects (camera, program data,
    // lights, atmosphere); vulkanizer-assigned bindings start above them.
    constexpr std::uint32_t legacy_first_binding = 8;

    // Evaluates the initializer expressions the shader corpus actually uses: numeric literals
    // and vecN(...) constructors with one (broadcast) or N literal arguments.
    bool evaluateInitializer(std::string_view expression, std::vector<float>& out) noexcept
    {
        out.clear();

        auto parseNumber = [](std::string_view text, float& value)
        {
            while(!text.empty() && (text.front() == ' ' || text.front() == '\t')) text.remove_prefix(1);
            while(!text.empty() && (text.back() == ' ' || text.back() == '\t' || text.back() == 'f' || text.back() == 'F')) text.remove_suffix(1);
            if(text == "true") { value = 1.0f; return true; }
            if(text == "false") { value = 0.0f; return true; }
            const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
            return result.ec == std::errc { } && result.ptr == text.data() + text.size();
        };

        const auto open = expression.find('(');
        if(open == std::string_view::npos)
        {
            float value = 0.0f;
            if(!parseNumber(expression, value)) return false;
            out.push_back(value);
            return true;
        }

        const auto typeName = expression.substr(0, open);
        std::size_t components = 0;
        if(typeName.find("vec4") != std::string_view::npos) components = 4;
        else if(typeName.find("vec3") != std::string_view::npos) components = 3;
        else if(typeName.find("vec2") != std::string_view::npos) components = 2;
        else return false;

        const auto close = expression.rfind(')');
        if(close == std::string_view::npos || close <= open) return false;
        std::string_view arguments = expression.substr(open + 1, close - open - 1);

        std::vector<float> values;
        while(!arguments.empty())
        {
            const auto comma = arguments.find(',');
            const auto piece = arguments.substr(0, comma);
            float value = 0.0f;
            if(!parseNumber(piece, value)) return false;
            values.push_back(value);
            if(comma == std::string_view::npos) break;
            arguments.remove_prefix(comma + 1);
        }

        if(values.size() == 1) out.assign(components, values[0]);
        else if(values.size() == components) out = values;
        else return false;
        return true;
    }
}

SGCore::GL46Shader::~GL46Shader() noexcept
{
    destroy();

    if(!CoreMain::getWindow().shouldClose())
    {
        static_cast<GLObjectsStorage&>(CoreMain::getRenderer()->storage()).m_shaders.erase(this);
    }
}

// TODO: watch SGP1
// destroys shaders and shader program in gpu side and compiles new shaders and shader program
void SGCore::GL46Shader::doCompile()
{
    m_cachedLocations.clear();
    destroyLegacyBlocks();
    m_reflection = ShaderReflection { };
    m_rhiProgram = nullptr;

    auto shaderAnalyzedFile = getAnalyzedFile();
    auto fileAsset = getFile();

    std::string definesCode;

    for(auto const& shaderDefinePair : getDefines())
    {
        for(const auto& shaderDefine : shaderDefinePair.second)
        {
            definesCode += shaderDefine.toString() + "\n";
        }
    }

    for(const auto& attributePair : shaderAnalyzedFile->getAttributes())
    {
        definesCode += "#define " + attributePair.first + " " + attributePair.second + "\n";
    }

    // stage sources; in RHI mode they go through the vulkanizer first, so that uniforms live
    // in blocks and bindings are explicit — the same GLSL the RHI and Vulkan consume
    std::vector<SGSLEVulkanizer::Stage> stages;
    for(const auto& subShader : shaderAnalyzedFile->getSubShaders())
    {
        stages.push_back({ subShader.getType(), definesCode + "\n" + subShader.getCode() });
    }

    SGSLEVulkanizer::Report vulkanizeReport;
    if(m_useRHIUniforms)
    {
        SGSLEVulkanizer::Config config;
        config.m_target = SGSLEVulkanizer::Target::OPENGL;
        config.m_firstBinding = legacy_first_binding;
        vulkanizeReport = SGSLEVulkanizer::vulkanize(stages, config);
    }

    for(const auto& stage : stages)
    {
        m_subShadersHandles.push_back({ stage.m_type, compileSubShader(stage.m_type, stage.m_code) });
    }

    // -----------------------------------------------

    // gl side -------------------------------------------
    m_programHandle = glCreateProgram();

    for(const auto& [shaderPartType, shaderPartHandle] : m_subShadersHandles)
    {
        if(shaderPartHandle == 0)
        {
            SG_LOG_E("Cannot create shader program: {} shader was not compiled and has handle equals to 0! Path: ",
                     sgsleSubShaderTypeToString(shaderPartType),
                     Utils::toUTF8(getPath().raw()));

            destroy();

            return;
        }
        glAttachShader(m_programHandle, shaderPartHandle);
    }

    glLinkProgram(m_programHandle);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_programHandle, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint maxLogLength = 0;
        glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &maxLogLength);

        if(maxLogLength > 0)
        {
            std::vector<GLchar> infoLog(maxLogLength);
            glGetProgramInfoLog(m_programHandle, maxLogLength, &maxLogLength, &infoLog[0]);

            SG_LOG_E(
                  "Error in shader program by path: {}\n{}",
                  Utils::toUTF8(fileAsset->getPath().resolved()),
                  infoLog.data());
        }

        destroy();

        return;
    }

    for(auto& [shaderPartType, shaderPartHandle] : m_subShadersHandles)
    {
        glDetachShader(m_programHandle, shaderPartHandle);
        glDeleteShader(shaderPartHandle);

        shaderPartHandle = 0;
    }

    if(m_useRHIUniforms)
    {
        GL46ShaderProgram::reflectProgram(m_programHandle, m_reflection);
        setupLegacyBlocks();

        // initializers of loose uniforms can not survive the move into a block: write them as defaults
        std::vector<float> values;
        for(const auto& defaultValue : vulkanizeReport.m_defaults)
        {
            if(!evaluateInitializer(defaultValue.m_expression, values))
            {
                SG_LOG_W("GL46Shader '{}': can not evaluate initializer '{}' of uniform '{}'; it stays zero.",
                         Utils::toUTF8(getPath().raw()), defaultValue.m_expression, defaultValue.m_memberName);
                continue;
            }
            writeLegacy(defaultValue.m_memberName, values.data(), static_cast<std::uint32_t>(values.size() * sizeof(float)));
        }
    }
}

void SGCore::GL46Shader::setupLegacyBlocks() noexcept
{
    for(const auto& binding : m_reflection.m_bindings)
    {
        if(binding.m_type != ShaderDescriptorType::UNIFORM_BUFFER) continue;
        if(binding.m_name.rfind("SGLegacyUniforms", 0) != 0) continue;

        LegacyBlock block;
        block.m_binding = binding.m_binding;
        block.m_size = binding.m_blockSize;
        glCreateBuffers(1, &block.m_buffer);
        // zero-initialized: uniforms nobody sets read as 0 / false, like a fresh GL program
        const std::vector<std::uint8_t> zeros(binding.m_blockSize, 0);
        glNamedBufferStorage(block.m_buffer, static_cast<GLsizeiptr>(binding.m_blockSize), zeros.data(), GL_DYNAMIC_STORAGE_BIT);

        const std::size_t blockIndex = m_legacyBlocks.size();
        m_legacyBlocks.push_back(block);

        for(const auto& member : binding.m_members)
        {
            LegacyMemberRef reference;
            reference.m_blockIndex = blockIndex;
            reference.m_offset = member.m_offset;
            reference.m_size = member.m_size;
            reference.m_arrayCount = member.m_arrayCount;
            reference.m_elementStride = member.m_arrayCount > 1 ? member.m_paddedSize : 0;
            m_legacyMembers.emplace(member.m_name, std::vector<LegacyMemberRef> { }).first->second.push_back(reference);
        }
    }
}

void SGCore::GL46Shader::destroyLegacyBlocks() noexcept
{
    for(const auto& block : m_legacyBlocks)
    {
        if(block.m_buffer) glDeleteBuffers(1, &block.m_buffer);
    }
    m_legacyBlocks.clear();
    m_legacyMembers.clear();
}

bool SGCore::GL46Shader::writeLegacy(std::string_view uniformName, const void* data, std::uint32_t size) noexcept
{
    if(!m_useRHIUniforms || m_legacyMembers.empty()) return false;

    // "name[i]" → base member + i * element stride
    std::uint32_t index = 0;
    bool indexed = false;
    std::string_view base = uniformName;
    if(const auto bracket = uniformName.find('['); bracket != std::string_view::npos)
    {
        base = uniformName.substr(0, bracket);
        const auto close = uniformName.find(']', bracket);
        const auto digits = uniformName.substr(bracket + 1, close == std::string_view::npos ? std::string_view::npos : close - bracket - 1);
        const auto result = std::from_chars(digits.data(), digits.data() + digits.size(), index);
        if(result.ec != std::errc { }) return false;
        indexed = true;
    }

    const auto it = m_legacyMembers.find(std::string(base));
    if(it == m_legacyMembers.end()) return false;

    // a uniform declared in several stages is a member of each stage's block: write all of them
    for(const auto& member : it->second)
    {
        const auto& block = m_legacyBlocks[member.m_blockIndex];
        std::uint32_t offset = member.m_offset;
        std::uint32_t capacity = member.m_size;
        if(indexed)
        {
            if(index >= member.m_arrayCount) continue;
            offset = member.m_offset + index * member.m_elementStride;
            capacity = member.m_elementStride;
        }
        // std140 padding makes capacity ≥ the value size for scalars/vectors/matrices; never overrun
        const std::uint32_t bytes = capacity == 0 || size < capacity ? size : capacity;
        glNamedBufferSubData(block.m_buffer, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(bytes), data);
    }
    return true;
}

bool SGCore::GL46Shader::isLegacyMember(std::string_view uniformName) const noexcept
{
    if(!m_useRHIUniforms) return false;
    const auto bracket = uniformName.find('[');
    return m_legacyMembers.contains(std::string(bracket == std::string_view::npos ? uniformName : uniformName.substr(0, bracket)));
}

void SGCore::GL46Shader::bind() const noexcept
{
    glUseProgram(m_programHandle);
    if(m_useRHIUniforms) GL46Renderer::getInstance()->setCurrentLegacyShader(const_cast<GL46Shader*>(this));

    for(const auto& block : m_legacyBlocks)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, block.m_binding, block.m_buffer);
    }
}

// TODO: watch SGP1
void SGCore::GL46Shader::destroy() noexcept
{
    SG_LOG_I("Destroying shader program with handle '{}'", m_programHandle);

    for(const auto& [shaderPartType, shaderPartHandle] : m_subShadersHandles)
    {
        if(shaderPartHandle == 0) continue;

        glDeleteShader(shaderPartHandle);
    }

    if(m_programHandle != 0)
    {
        glDeleteProgram(m_programHandle);
    }

    m_programHandle = 0;

    destroyLegacyBlocks();

    // TODO:: SGP0
    #ifdef SUNGEAR_DEBUG
    //GL4Renderer::getInstance()->checkForErrors();
    #endif

    m_subShadersHandles.clear();
}

GLuint SGCore::GL46Shader::compileSubShader(SGCore::SGSLESubShaderType shaderType, const std::string& code)
{
    auto fileAsset = getFile();

    std::string additionalShaderInfo =
            "#version " + m_version + "\n";

#if SG_PLATFORM_OS_ANDROID
    additionalShaderInfo += "precision highp float;\n";
#endif

    std::string codeToCompile = additionalShaderInfo + code;

    GLint glShaderType = -1;
    switch(shaderType)
    {
        case SST_NONE: break;
        case SST_VERTEX: glShaderType = GL_VERTEX_SHADER; break;
        case SST_FRAGMENT: glShaderType = GL_FRAGMENT_SHADER; break;
        case SST_GEOMETRY: glShaderType = GL_GEOMETRY_SHADER; break;
        case SST_COMPUTE: glShaderType = GL_COMPUTE_SHADER; break;
        case SST_TESS_CONTROL: glShaderType = GL_TESS_CONTROL_SHADER; break;
        case SST_TESS_EVALUATION: glShaderType = GL_TESS_EVALUATION_SHADER; break;
    }

    if(glShaderType == -1)
    {
        SG_LOG_E(
              "Error while compiling subshader! Unknown type of subshader.\n{}", SG_CURRENT_LOCATION_STR);

        return 0;
    }

    // gl side -------------------------------------------
    const GLuint shaderPartHandle = glCreateShader(glShaderType);

    const auto* castedCode = (const GLchar*) codeToCompile.c_str();
    glShaderSource(shaderPartHandle, 1, &castedCode, nullptr);

    glCompileShader(shaderPartHandle);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shaderPartHandle, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        GLint maxLogLength = 0;
        glGetShaderiv(shaderPartHandle, GL_INFO_LOG_LENGTH, &maxLogLength);

        std::vector<GLchar> infoLog(maxLogLength);
        glGetShaderInfoLog(shaderPartHandle, maxLogLength, &maxLogLength, &infoLog[0]);

        glDeleteShader(shaderPartHandle);

        SG_LOG_E(
              "Error in shader by path: {}\n{}",
              Utils::toUTF8(fileAsset->getPath().resolved()),
              infoLog.data());

        return 0;
    }

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shaderPartHandle;
}


std::int32_t SGCore::GL46Shader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    return glGetUniformLocation(m_programHandle, uniformName.c_str());
}

void SGCore::GL46Shader::useUniformBuffer(const Ref<IUniformBuffer>& uniformBuffer)
{
    auto uniformBufferIdx = glGetUniformBlockIndex(m_programHandle, uniformBuffer->m_blockName.c_str());
    glUniformBlockBinding(m_programHandle, uniformBufferIdx,
                          uniformBuffer->getLayoutLocation());
}

void SGCore::GL46Shader::useTexture(const std::string& uniformName, const uint8_t& texBlock)
{
    int texLoc = getShaderUniformLocation(uniformName);
    glUniform1i(texLoc, texBlock);
}

void SGCore::GL46Shader::useMatrix(const std::string& uniformName, const glm::mat4& matrix)
{
    if(writeLegacy(uniformName, glm::value_ptr(matrix), sizeof(glm::mat4))) return;
    int matLoc = getShaderUniformLocation(uniformName);
    glUniformMatrix4fv(matLoc, 1, false, glm::value_ptr(matrix));
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y)
{
    const float values[2] = { x, y };
    if(writeLegacy(uniformName, values, sizeof(values))) return;
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, x, y);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z)
{
    const float values[3] = { x, y, z };
    if(writeLegacy(uniformName, values, sizeof(values))) return;
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, x, y, z);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z,
                                    const float& w)
{
    const float values[4] = { x, y, z, w };
    if(writeLegacy(uniformName, values, sizeof(values))) return;
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, x, y, z, w);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec2& vec)
{
    useVectorf(uniformName, vec.x, vec.y);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec3& vec)
{
    useVectorf(uniformName, vec.x, vec.y, vec.z);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec4& vec)
{
    useVectorf(uniformName, vec.x, vec.y, vec.z, vec.w);
}

void SGCore::GL46Shader::useFloat(const std::string& uniformName, const float& f)
{
    if(writeLegacy(uniformName, &f, sizeof(float))) return;
    int fLoc = getShaderUniformLocation(uniformName);
    glUniform1f(fLoc, f);
}

void SGCore::GL46Shader::useInteger(const std::string& uniformName, const int& i)
{
    if(writeLegacy(uniformName, &i, sizeof(int))) return;
    int iLoc = getShaderUniformLocation(uniformName);
    glUniform1i(iLoc, i);
}

void SGCore::GL46Shader::useTextureBlock(const std::string& uniformName, const int& textureBlock)
{
    // samplers are never block members: plain uniform
    int iLoc = getShaderUniformLocation(uniformName);
    glUniform1i(iLoc, textureBlock);
}

bool SGCore::GL46Shader::isUniformExists(const std::string& uniformName) const noexcept
{
    if(isLegacyMember(uniformName)) return true;
    return glGetUniformLocation(m_programHandle, uniformName.c_str()) != -1;
}

void SGCore::GL46Shader::useMaterialFactors(const SGCore::IMaterial* material)
{
    useVectorf("u_materialDiffuseCol", material->getDiffuseColor());
    useVectorf("u_materialSpecularCol", material->getSpecularColor());
    useVectorf("u_materialAmbientCol", material->getAmbientColor());
    useVectorf("u_materialEmissionCol", material->getEmissionColor());
    useVectorf("u_materialTransparentCol", material->getTransparentColor());
    useFloat("u_materialShininess", material->getShininess());
    useFloat("u_materialMetallicFactor", material->getMetallicFactor());
    useFloat("u_materialRoughnessFactor", material->getRoughnessFactor());
}

SGCore::Ref<SGCore::IShaderProgram> SGCore::GL46Shader::getRHIProgram() noexcept
{
    if(m_programHandle == 0) return nullptr;
    if(!m_rhiProgram)
    {
        m_rhiProgram = MakeRef<GL46LegacyProgram>(m_programHandle, m_reflection);
    }
    return m_rhiProgram;
}
