#include "GL46SubPassShader.h"

#include <vector>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Main/CoreMain.h"

SGCore::GL46SubPassShader::~GL46SubPassShader() noexcept
{
    destroy();
}

// TODO: watch SGP1
// destroys shaders and shader program in gpu side and compiles new shaders and shader program
void SGCore::GL46SubPassShader::compile(Ref<FileAsset> fileAsset)
{
    if(!fileAsset)
    {
        spdlog::error("Can not compile subpass shader! File asset is nullptr.\n{0}", SG_CURRENT_LOCATION_STR);
        return;
    }

    if(m_subShadersCodes.empty())
    {
        spdlog::error("No sub shaders to compile! Shader path: {0}\n{1}", fileAsset->getPath().string(), SG_CURRENT_LOCATION_STR);
        return;
    }

    m_fileAsset = fileAsset;

    std::string definesCode;

    for(auto const& shaderDefinePair : m_defines)
    {
        for(const auto& shaderDefine : shaderDefinePair.second)
        {
            definesCode += shaderDefine.toString() + "\n";
        }
    }

    for(const auto& codesIter : m_subShadersCodes)
    {
        m_subShadersHandlers.push_back(compileSubShader(codesIter.first, definesCode + "\n" + codesIter.second));
    }

    // -----------------------------------------------

    // gl side -------------------------------------------
    m_programHandler = glCreateProgram();

    for(const GLuint shaderPartHandler : m_subShadersHandlers)
    {
        glAttachShader(m_programHandler, shaderPartHandler);
    }

    glLinkProgram(m_programHandler);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_programHandler, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint maxLogLength = 0;
        glGetProgramiv(m_programHandler, GL_INFO_LOG_LENGTH, &maxLogLength);

        std::vector<GLchar> infoLog(maxLogLength);
        glGetProgramInfoLog(m_programHandler, maxLogLength, &maxLogLength, &infoLog[0]);

        destroy();

        spdlog::error("Error in shader by path: {0}\n{1}",
                      m_fileAsset.lock()->getPath().string(),
                      infoLog.data());
    }

    for(const GLuint shaderHandler : m_subShadersHandlers)
    {
        glDetachShader(m_programHandler, shaderHandler);
    }

    m_cachedLocations.clear();

    onTexturesCountChanged();
}

void SGCore::GL46SubPassShader::bind() noexcept
{
    glUseProgram(m_programHandler);

    bindTexturesBlocks();
}

// TODO: watch SGP1
void SGCore::GL46SubPassShader::destroy() noexcept
{
    for(const GLuint shaderPartHandler : m_subShadersHandlers)
    {
        glDeleteShader(shaderPartHandler);
    }

    glDeleteProgram(m_programHandler);

    // TODO:: SGP0
    #ifdef SUNGEAR_DEBUG
    //GL4Renderer::getInstance()->checkForErrors();
    #endif

    m_subShadersHandlers.clear();
}

GLuint SGCore::GL46SubPassShader::compileSubShader(SGCore::SGSLSubShaderType shaderType, const std::string& code)
{
    std::string additionalShaderInfo =
            "#version " + m_version + "\n";

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
        spdlog::error("Error while compiling subshader! Unknown type of subshader.\n{0}", SG_CURRENT_LOCATION_STR);

        return -1;
    }

    // gl side -------------------------------------------
    const GLuint shaderPartHandler = glCreateShader(glShaderType);

    const auto* castedCode = (const GLchar*) codeToCompile.c_str();
    glShaderSource(shaderPartHandler, 1, &castedCode, nullptr);

    glCompileShader(shaderPartHandler);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shaderPartHandler, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        GLint maxLogLength = 0;
        glGetShaderiv(shaderPartHandler, GL_INFO_LOG_LENGTH, &maxLogLength);

        std::vector<GLchar> infoLog(maxLogLength);
        glGetShaderInfoLog(shaderPartHandler, maxLogLength, &maxLogLength, &infoLog[0]);

        glDeleteShader(shaderPartHandler);

        spdlog::error("Error in shader by path: {0}\n{1}",
                      m_fileAsset.lock()->getPath().string(),
                      infoLog.data());

        return -1;
    }

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shaderPartHandler;
}


std::int32_t SGCore::GL46SubPassShader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    const auto& foundLocation = m_cachedLocations.find(uniformName);
    if(foundLocation == m_cachedLocations.cend())
    {
        GLint location = glGetUniformLocation(m_programHandler, uniformName.c_str());
        m_cachedLocations[uniformName] = location;
        return location;
    }
    return foundLocation->second;
}

void SGCore::GL46SubPassShader::useUniformBuffer(const Ref<IUniformBuffer>& uniformBuffer)
{
    auto uniformBufferIdx = glGetUniformBlockIndex(m_programHandler, uniformBuffer->m_blockName.c_str());
    glUniformBlockBinding(m_programHandler, uniformBufferIdx,
                          uniformBuffer->getLayoutLocation());
}

void SGCore::GL46SubPassShader::useTexture(const std::string& uniformName, const uint8_t& texBlock)
{
    int texLoc = getShaderUniformLocation(uniformName);
    glUniform1i(texLoc, texBlock);
}

void SGCore::GL46SubPassShader::useMatrix(const std::string& uniformName, const glm::mat4& matrix)
{
    int matLoc = getShaderUniformLocation(uniformName);
    glUniformMatrix4fv(matLoc, 1, false, glm::value_ptr(matrix));
}

void SGCore::GL46SubPassShader::useVectorf(const std::string& uniformName, const float& x, const float& y)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, x, y);
}

void SGCore::GL46SubPassShader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, x, y, z);
}

void SGCore::GL46SubPassShader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z,
                                           const float& w)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, x, y, z, w);
}

void SGCore::GL46SubPassShader::useVectorf(const std::string& uniformName, const glm::vec2& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, vec.x, vec.y);
}

void SGCore::GL46SubPassShader::useVectorf(const std::string& uniformName, const glm::vec3& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, vec.x, vec.y, vec.z);
}

void SGCore::GL46SubPassShader::useVectorf(const std::string& uniformName, const glm::vec4& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, vec.x, vec.y, vec.z, vec.w);
}

void SGCore::GL46SubPassShader::useFloat(const std::string& uniformName, const float& f)
{
    int fLoc = getShaderUniformLocation(uniformName);
    glUniform1f(fLoc, f);
}

void SGCore::GL46SubPassShader::useInteger(const std::string& uniformName, const size_t& i)
{
    int iLoc = getShaderUniformLocation(uniformName);
    glUniform1i(iLoc, i);
}

void SGCore::GL46SubPassShader::useTextureBlock(const std::string& uniformName, const size_t& textureBlock)
{
    int iLoc = getShaderUniformLocation(uniformName);
    glUniform1i(iLoc, textureBlock);
}

/*
SGCore::API::GL46::GL46Shader& SGCore::API::GL46::GL46Shader::operator=(SGCore::API::GL46::GL46Shader&& other) noexcept
{
    this->m_programHandler = other.m_programHandler;
    other.m_programHandler = 0;

    this->m_shadersHandlers = other.m_shadersHandlers;
    other.m_shadersHandlers = {};

    return *this;
}
 */