//
// Created by stuka on 13.05.2023.
//

#include "GLShader.h"

#include <vector>

#include "SGCore/Logging/Log.h"
#include "GLRenderer.h"

/*
Core::Graphics::API::GL46::GLShader::GLShader(Core::Graphics::API::GL46::GLShader&& other) noexcept
{
   *this = std::move(other);
}
 */
Core::Graphics::API::GL46::GLShader::~GLShader() noexcept
{
    destroy();
}

GLuint Core::Graphics::API::GL46::GLShader::createShaderPart(const GLenum& type, const std::string_view& shaderVirtualPath, const std::string& finalShaderCode) noexcept
{
    std::string additionalShaderInfo =
            R"(
            #version 460
            #extension GL_ARB_shading_language_include : require

            )";
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

        Logging::consolePrintf(Logging::MessageType::SG_ERROR, std::string(infoLog));

        return -1;
    }

    // TODO:: исправить GL_INVALID_VALUE. Есть вероятность, что после OpenGL 4.5 glNamedStringARB не работает
    std::string finalVirtualPath = shaderVirtualPath.data() + shaderVirtualIncludeType;
    glNamedStringARB(GL_SHADER_INCLUDE_ARB, (GLint) finalVirtualPath.size(), (const GLchar*) finalVirtualPath.c_str(), (GLint) codeToCompile.size(), (const GLchar*) codeToCompile.c_str());

    GLRenderer::getInstance()->checkForErrors();

    return shaderPartHandler;
}

void Core::Graphics::API::GL46::GLShader::compile(const std::string_view& shaderVirtualPath, const std::string& code) noexcept
{
    m_path = shaderVirtualPath;

    std::string definesCode;

    for(auto const& shaderDefine : m_defines)
    {
        definesCode += shaderDefine.toString();
    }

    const std::string finalCode = definesCode + code;

    m_shaderPartsHandlers[0] = createShaderPart(GL_FRAGMENT_SHADER, shaderVirtualPath, finalCode);
    m_shaderPartsHandlers[1] = createShaderPart(GL_VERTEX_SHADER, shaderVirtualPath, finalCode);

    // TODO:: SGP0
    //m_shaderPartsHandlers[2] = createShaderPart(GL_GEOMETRY_SHADER, shaderVirtualPath, finalCode);
    //m_shaderPartsHandlers[3] = createShaderPart(GL_COMPUTE_SHADER, shaderVirtualPath, finalCode);
    //m_shaderPartsHandlers[4] = createShaderPart(GL_TESS_CONTROL_SHADER, shaderVirtualPath, finalCode);
    //m_shaderPartsHandlers[5] = createShaderPart(GL_TESS_EVALUATION_SHADER, shaderVirtualPath, finalCode);

    // gl side -------------------------------------------
    m_programHandler = glCreateProgram();

    //GLRenderer::getInstance()->checkForErrors();

    for(const std::uint32_t shaderPartHandler : m_shaderPartsHandlers)
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

        Logging::consolePrintf(Logging::MessageType::SG_ERROR, std::string(infoLog));
    }

    for(const std::uint32_t shaderHandler : m_shaderPartsHandlers)
    {
        glDetachShader(m_programHandler, shaderHandler);
    }
}

void Core::Graphics::API::GL46::GLShader::bind() noexcept
{
    glUseProgram(m_programHandler);
}

void Core::Graphics::API::GL46::GLShader::destroy() noexcept
{
    for(const std::uint32_t shaderPartHandler : m_shaderPartsHandlers)
    {
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

        std::string shaderPartVirtualPath = m_path.data() + shaderPartVirtualIncludeType;

        glDeleteNamedStringARB((GLint) shaderPartVirtualPath.size(), (const GLchar*) shaderPartVirtualPath.c_str());

        glDeleteShader(shaderPartHandler);
    }

    glDeleteProgram(m_programHandler);

    // TODO:: SGP0
    GLRenderer::getInstance()->checkForErrors();

    std::cout << "shader destroyed" << std::endl;
}

/*
Core::Graphics::API::GL46::GLShader& Core::Graphics::API::GL46::GLShader::operator=(Core::Graphics::API::GL46::GLShader&& other) noexcept
{
    this->m_programHandler = other.m_programHandler;
    other.m_programHandler = 0;

    this->m_shadersHandlers = other.m_shadersHandlers;
    other.m_shadersHandlers = {};

    return *this;
}
 */