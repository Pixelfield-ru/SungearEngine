#include "GL46Shader.h"

#include <vector>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "SGCore/Logging/Log.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GLShadersPreprocessor.h"

Core::Graphics::GL46Shader::~GL46Shader() noexcept
{
    destroy();
}

// TODO: watch SGP1
GLuint Core::Graphics::GL46Shader::createShaderPart(const GLenum& type, const std::string& finalShaderCode) noexcept
{
    std::string additionalShaderInfo =
            "#version " + m_version + "\n";
    std::string shaderVirtualIncludeType;

    if(type == GL_VERTEX_SHADER)
    {
        additionalShaderInfo += "#define VERTEX_SHADER\n";
        shaderVirtualIncludeType = ".vs";
    }
    else if(type == GL_FRAGMENT_SHADER)
    {
        additionalShaderInfo += "#define FRAGMENT_SHADER\n";
        shaderVirtualIncludeType = ".fs";
    }
    else if(type == GL_GEOMETRY_SHADER)
    {
        additionalShaderInfo += "#define GEOMETRY_SHADER\n";
        shaderVirtualIncludeType = ".gs";
    }
    // opengl 4.3
    else if(type == GL_COMPUTE_SHADER)
    {
        additionalShaderInfo += "#define COMPUTE_SHADER\n";
        shaderVirtualIncludeType = ".cs";
    }
    // opengl 4.0
    else if(type == GL_TESS_CONTROL_SHADER)
    {
        additionalShaderInfo += "#define TESS_CONTROL_SHADER\n";
        shaderVirtualIncludeType = ".tcs";
    }
    // opengl 4.0
    else if(type == GL_TESS_EVALUATION_SHADER)
    {
        additionalShaderInfo += "#define TESS_EVALUATION_SHADER\n";
        shaderVirtualIncludeType = ".tes";
    }

    std::string codeToCompile = additionalShaderInfo + finalShaderCode;

    // gl side -------------------------------------------
    const GLuint shaderPartHandler = glCreateShader(type);

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

        SGC_ERROR("Error in shader by path: " + m_fileAsset.lock()->getPath().string() + "\n" +
        std::string(infoLog.data()));

        return -1;
    }

    // TODO:: glNamedStringARB не работает
    //std::string finalVirtualPath = shaderVirtualPath.data() + shaderVirtualIncludeType;

    //glNamedStringARB(GL_SHADER_INCLUDE_ARB, (GLint) finalVirtualPath.size(), (const GLchar*) finalVirtualPath.c_str(), (GLint) codeToCompile.size(), (const GLchar*) codeToCompile.c_str());

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shaderPartHandler;
}

// TODO: watch SGP1
// destroys shaders and shader program in gpu side and compiles new shaders and shader program
void Core::Graphics::GL46Shader::compile(std::shared_ptr<Memory::Assets::FileAsset> asset) noexcept
{
    destroy();

    auto thisWeak = weak_from_this();

    std::shared_ptr<Memory::Assets::FileAsset> fileAssetShared = m_fileAsset.lock();

    if(fileAssetShared)
    {
        fileAssetShared->removeObserver(thisWeak.lock());
    }

    m_fileAsset = asset;

    fileAssetShared = m_fileAsset.lock();

    if(!fileAssetShared)
    {
        SGCF_ERROR("Error compiling shader. Invalid file asset passed.", SG_LOG_CURRENT_SESSION_FILE);
    }

    fileAssetShared->addObserver(thisWeak.lock());

    std::string definesCode;

    for(auto const& shaderDefine : m_defines)
    {
        definesCode += shaderDefine.toString() + "\n";
    }

    std::string preprocessErrors;
    std::string finalCode = definesCode + GLShadersPreprocessor::processShader(
            fileAssetShared->getPath().string(),
            fileAssetShared->getData(),
            preprocessErrors);

    if(!preprocessErrors.empty())
    {
        SGCF_ERROR(preprocessErrors, SG_LOG_CURRENT_SESSION_FILE);
    }

    // parsing shaders types defines ----------------
    std::istringstream codeStream(finalCode);

    std::string line;

    while(std::getline(codeStream, line))
    {
        if(line.starts_with("#ifdef VERTEX_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_VERTEX_SHADER, finalCode));
        }
        else if(line.starts_with("#ifdef FRAGMENT_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_FRAGMENT_SHADER, finalCode));
        }
        else if(line.starts_with("#ifdef GEOMETRY_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_GEOMETRY_SHADER, finalCode));
        }
        else if(line.starts_with("#ifdef COMPUTE_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_COMPUTE_SHADER, finalCode));
        }
        else if(line.starts_with("#ifdef TESS_CONTROL_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_TESS_CONTROL_SHADER, finalCode));
        }
        else if(line.starts_with("#ifdef TESS_EVALUATION_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_TESS_EVALUATION_SHADER, finalCode));
        }
    }
    // -----------------------------------------------

    // gl side -------------------------------------------
    m_programHandler = glCreateProgram();

    for(const GLuint shaderPartHandler : m_shaderPartsHandlers)
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

        SGC_ERROR(std::string(infoLog.data()));
    }

    for(const GLuint shaderHandler : m_shaderPartsHandlers)
    {
        glDetachShader(m_programHandler, shaderHandler);
    }
}

void Core::Graphics::GL46Shader::bind() noexcept
{
    glUseProgram(m_programHandler);
}

// TODO: watch SGP1
void Core::Graphics::GL46Shader::destroy() noexcept
{
    for(const GLuint shaderPartHandler : m_shaderPartsHandlers)
    {
        //if(!glIsShader(shaderPartHandler)) continue;

        GLint shaderType = 0;
        glGetShaderiv(shaderPartHandler, GL_SHADER_TYPE, &shaderType);

        std::string shaderPartVirtualIncludeType;
        if(shaderType == GL_VERTEX_SHADER)
        {
            shaderPartVirtualIncludeType = ".vs";
        }
        else if(shaderType == GL_FRAGMENT_SHADER)
        {
            shaderPartVirtualIncludeType = ".fs";
        }
        else if(shaderType == GL_GEOMETRY_SHADER)
        {
            shaderPartVirtualIncludeType = ".gs";
        }
        // opengl 4.3
        else if(shaderType == GL_COMPUTE_SHADER)
        {
            shaderPartVirtualIncludeType = ".cs";
        }
        // opengl 4.0
        else if(shaderType == GL_TESS_CONTROL_SHADER)
        {
            shaderPartVirtualIncludeType = ".tcs";
        }
        // opengl 4.0
        else if(shaderType == GL_TESS_EVALUATION_SHADER)
        {
            shaderPartVirtualIncludeType = ".tes";
        }

        //std::string shaderPartVirtualPath = m_fileAsset->getPath().data() + shaderPartVirtualIncludeType;

        //glDeleteNamedStringARB((GLint) shaderPartVirtualPath.size(), (const GLchar*) shaderPartVirtualPath.c_str());

        glDeleteShader(shaderPartHandler);
    }

    glDeleteProgram(m_programHandler);

    // TODO:: SGP0
    #ifdef SUNGEAR_DEBUG
    //GL4Renderer::getInstance()->checkForErrors();
    #endif

    m_shaderPartsHandlers.clear();
}

std::int32_t Core::Graphics::GL46Shader::getShaderUniformLocation(const std::string& uniformName) const noexcept
{
    return glGetUniformLocation(m_programHandler, uniformName.c_str());
}

void Core::Graphics::GL46Shader::useMaterialTexture(const Memory::Assets::MaterialTexture& materialTexture)
{
    int texLoc = getShaderUniformLocation(materialTexture.m_nameInShader);
    glUniform1i(texLoc, materialTexture.m_textureUnit);
}

void Core::Graphics::GL46Shader::useUniformBuffer(const std::shared_ptr<IUniformBuffer>& uniformBuffer)
{
    auto uniformBufferIdx = glGetUniformBlockIndex(m_programHandler, uniformBuffer->m_blockName.c_str());
    glUniformBlockBinding(m_programHandler, uniformBufferIdx,
                          uniformBuffer->getLayoutLocation());
}

void Core::Graphics::GL46Shader::useTexture(const std::string& uniformName, const uint8_t& texBlock)
{
    int texLoc = getShaderUniformLocation(uniformName);
    glUniform1i(texLoc, texBlock);
}

void Core::Graphics::GL46Shader::useMatrix(const std::string& uniformName, const glm::mat4& matrix)
{
    int matLoc = getShaderUniformLocation(uniformName);
    glUniformMatrix4fv(matLoc, 1, false, glm::value_ptr(matrix));
}

void Core::Graphics::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, x, y);
}

void
Core::Graphics::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, x, y, z);
}

void
Core::Graphics::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z,
                                       const float& w)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, x, y, z, w);
}

void Core::Graphics::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec2& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, vec.x, vec.y);
}

void Core::Graphics::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec3& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, vec.x, vec.y, vec.z);
}

void Core::Graphics::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec4& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, vec.x, vec.y, vec.z, vec.w);
}

/*
Core::Graphics::API::GL46::GL46Shader& Core::Graphics::API::GL46::GL46Shader::operator=(Core::Graphics::API::GL46::GL46Shader&& other) noexcept
{
    this->m_programHandler = other.m_programHandler;
    other.m_programHandler = 0;

    this->m_shadersHandlers = other.m_shadersHandlers;
    other.m_shadersHandlers = {};

    return *this;
}
 */