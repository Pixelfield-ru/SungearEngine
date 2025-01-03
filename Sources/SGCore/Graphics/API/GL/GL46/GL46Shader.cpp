#include <SGCore/Logger/Logger.h>
#include "GL46Shader.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/SGSL/SGSLESubShader.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

SGCore::GL46Shader::~GL46Shader() noexcept
{
    destroy();
}

// TODO: watch SGP1
// destroys shaders and shader program in gpu side and compiles new shaders and shader program
void SGCore::GL46Shader::doCompile()
{
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
        definesCode += "#define " + attributePair.first + " " + attributePair.second;
    }

    for(const auto& subShader : shaderAnalyzedFile->getSubShaders())
    {
        m_subShadersHandlers.push_back(compileSubShader(subShader.getType(), definesCode + "\n" + subShader.getCode()));
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

        if(maxLogLength > 0)
        {
            std::vector<GLchar> infoLog(maxLogLength);
            glGetProgramInfoLog(m_programHandler, maxLogLength, &maxLogLength, &infoLog[0]);

            LOG_E(SGCORE_TAG,
                  "Error in shader by path: {}\n{}",
                  fileAsset->getPath().resolved().string(),
                  infoLog.data());
        }

        destroy();
    }

    for(const GLuint shaderHandler : m_subShadersHandlers)
    {
        glDetachShader(m_programHandler, shaderHandler);
        glDeleteShader(shaderHandler);
    }

    m_cachedLocations.clear();
}

void SGCore::GL46Shader::bind() noexcept
{
    glUseProgram(m_programHandler);
}

// TODO: watch SGP1
void SGCore::GL46Shader::destroy() noexcept
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

GLuint SGCore::GL46Shader::compileSubShader(SGCore::SGSLESubShaderType shaderType, const std::string& code)
{
    auto fileAsset = getFile();

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
        LOG_E(SGCORE_TAG,
              "Error while compiling subshader! Unknown type of subshader.\n{}", SG_CURRENT_LOCATION_STR);

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

        LOG_E(SGCORE_TAG,
              "Error in shader by path: {}\n{}",
              fileAsset->getPath().resolved().string(),
             infoLog.data());

        return -1;
    }

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shaderPartHandler;
}


std::int32_t SGCore::GL46Shader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    // todo: is that faster than just glGetUniformLocation?
    // no, it is not faster with std::string. maybe if i do string that hashes its value and compares only hashes when operator== is called than it will be faster. but now it is not
    /*const auto& foundLocation = m_cachedLocations.find(uniformName);
    if(foundLocation == m_cachedLocations.cend())
    {
        GLint location = glGetUniformLocation(m_programHandler, uniformName.c_str());
        m_cachedLocations[uniformName] = location;
        return location;
    }
    return foundLocation->second;*/

    return glGetUniformLocation(m_programHandler, uniformName.c_str());
}

void SGCore::GL46Shader::useUniformBuffer(const Ref<IUniformBuffer>& uniformBuffer)
{
    auto uniformBufferIdx = glGetUniformBlockIndex(m_programHandler, uniformBuffer->m_blockName.c_str());
    glUniformBlockBinding(m_programHandler, uniformBufferIdx,
                          uniformBuffer->getLayoutLocation());
}

void SGCore::GL46Shader::useTexture(const std::string& uniformName, const uint8_t& texBlock)
{
    int texLoc = getShaderUniformLocation(uniformName);
    glUniform1i(texLoc, texBlock);
}

void SGCore::GL46Shader::useMatrix(const std::string& uniformName, const glm::mat4& matrix)
{
    int matLoc = getShaderUniformLocation(uniformName);
    glUniformMatrix4fv(matLoc, 1, false, glm::value_ptr(matrix));
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, x, y);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, x, y, z);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z,
                                    const float& w)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, x, y, z, w);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec2& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform2f(vecLoc, vec.x, vec.y);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec3& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform3f(vecLoc, vec.x, vec.y, vec.z);
}

void SGCore::GL46Shader::useVectorf(const std::string& uniformName, const glm::vec4& vec)
{
    int vecLoc = getShaderUniformLocation(uniformName);
    glUniform4f(vecLoc, vec.x, vec.y, vec.z, vec.w);
}

void SGCore::GL46Shader::useFloat(const std::string& uniformName, const float& f)
{
    int fLoc = getShaderUniformLocation(uniformName);
    glUniform1f(fLoc, f);
}

void SGCore::GL46Shader::useInteger(const std::string& uniformName, const int& i)
{
    int iLoc = getShaderUniformLocation(uniformName);
    glUniform1i(iLoc, i);
}

void SGCore::GL46Shader::useTextureBlock(const std::string& uniformName, const int& textureBlock)
{
    int iLoc = getShaderUniformLocation(uniformName);
    glUniform1i(iLoc, textureBlock);
}

bool SGCore::GL46Shader::isUniformExists(const std::string& uniformName) const noexcept
{
    return glGetUniformLocation(m_programHandler, uniformName.c_str()) != -1;
}

void SGCore::GL46Shader::useMaterialFactors(const SGCore::IMaterial* material)
{
    auto materialDiffuseColLoc = getShaderUniformLocation("u_materialDiffuseCol");
    auto materialSpecularColLoc = getShaderUniformLocation("u_materialSpecularCol");
    auto materialAmbientColLoc = getShaderUniformLocation("u_materialAmbientCol");
    auto materialEmissionColLoc = getShaderUniformLocation("u_materialEmissionCol");
    auto materialTransparentColLoc = getShaderUniformLocation("u_materialTransparentCol");
    auto materialShininessLoc = getShaderUniformLocation("u_materialShininess");
    auto materialMetallicFactorLoc = getShaderUniformLocation("u_materialMetallicFactor");
    auto materialRoughnessFactorLoc = getShaderUniformLocation("u_materialRoughnessFactor");

    auto materialDiffuseCol = material->getDiffuseColor();
    auto materialSpecularCol = material->getSpecularColor();
    auto materialAmbientCol = material->getAmbientColor();
    auto materialEmissionCol = material->getEmissionColor();
    auto materialTransparentCol = material->getTransparentColor();
    auto materialShininess = material->getShininess();
    auto materialMetallicFactor = material->getMetallicFactor();
    auto materialRoughnessFactor = material->getRoughnessFactor();

    glUniform4f(materialDiffuseColLoc, materialDiffuseCol.r, materialDiffuseCol.g, materialDiffuseCol.b, materialDiffuseCol.a);
    glUniform4f(materialSpecularColLoc, materialSpecularCol.r, materialSpecularCol.g, materialSpecularCol.b, materialSpecularCol.a);
    glUniform4f(materialAmbientColLoc, materialAmbientCol.r, materialAmbientCol.g, materialAmbientCol.b, materialAmbientCol.a);
    glUniform4f(materialEmissionColLoc, materialEmissionCol.r, materialEmissionCol.g, materialEmissionCol.b, materialEmissionCol.a);
    glUniform4f(materialTransparentColLoc, materialTransparentCol.r, materialTransparentCol.g, materialTransparentCol.b, materialTransparentCol.a);
    glUniform1f(materialShininessLoc, materialShininess);
    glUniform1f(materialMetallicFactorLoc, materialMetallicFactor);
    glUniform1f(materialRoughnessFactorLoc, materialRoughnessFactor);
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