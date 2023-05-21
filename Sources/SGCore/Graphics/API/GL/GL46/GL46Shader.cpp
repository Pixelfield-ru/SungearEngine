//
// Created by stuka on 13.05.2023.
//

#include "GL46Shader.h"

#include <vector>
#include <sstream>

#include "SGCore/Logging/Log.h"
#include "GL46Renderer.h"

/*
Core::Graphics::API::GL46::GL46Shader::GL46Shader(Core::Graphics::API::GL46::GL46Shader&& other) noexcept
{
   *this = std::move(other);
}
 */
Core::Graphics::API::GL::GL46::GL46Shader::~GL46Shader() noexcept
{
    destroy();
    //this->~Shader();
}

// TODO: watch SGP1
GLuint Core::Graphics::API::GL::GL46::GL46Shader::createShaderPart(const GLenum& type, const std::string_view& shaderVirtualPath, const std::string& finalShaderCode) noexcept
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

    // TODO:: glNamedStringARB не работает
    std::string finalVirtualPath = shaderVirtualPath.data() + shaderVirtualIncludeType;

    //glNamedStringARB(GL_SHADER_INCLUDE_ARB, (GLint) finalVirtualPath.size(), (const GLchar*) finalVirtualPath.c_str(), (GLint) codeToCompile.size(), (const GLchar*) codeToCompile.c_str());

    GL46Renderer::getInstance()->checkForErrors();

    return shaderPartHandler;
}

// TODO: watch SGP1
// destroys shaders and shader program in gpu side and compiles new shaders and shader program
void Core::Graphics::API::GL::GL46::GL46Shader::compile(const std::string_view& shaderVirtualPath, const std::string& code) noexcept
{
    destroy();

    m_path = shaderVirtualPath;

    std::string definesCode;

    for(auto const& shaderDefine : m_defines)
    {
        definesCode += shaderDefine.toString();
    }

    const std::string finalCode = definesCode + code;

    // parsing shaders types defines ----------------
    std::istringstream codeStream(finalCode);

    std::string line;

    while(std::getline(codeStream, line))
    {
        if(line.starts_with("#ifdef VERTEX_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_VERTEX_SHADER, shaderVirtualPath, finalCode));
        }
        else if(line.starts_with("#ifdef FRAGMENT_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_FRAGMENT_SHADER, shaderVirtualPath, finalCode));
        }
        else if(line.starts_with("#ifdef GEOMETRY_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_GEOMETRY_SHADER, shaderVirtualPath, finalCode));
        }
        else if(line.starts_with("#ifdef COMPUTE_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_COMPUTE_SHADER, shaderVirtualPath, finalCode));
        }
        else if(line.starts_with("#ifdef TESS_CONTROL_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_TESS_CONTROL_SHADER, shaderVirtualPath, finalCode));
        }
        else if(line.starts_with("#ifdef TESS_EVALUATION_SHADER"))
        {
            m_shaderPartsHandlers.push_back(createShaderPart(GL_TESS_EVALUATION_SHADER, shaderVirtualPath, finalCode));
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

        Logging::consolePrintf(Logging::MessageType::SG_ERROR, std::string(infoLog));
    }

    for(const GLuint shaderHandler : m_shaderPartsHandlers)
    {
        glDetachShader(m_programHandler, shaderHandler);
    }
}

void Core::Graphics::API::GL::GL46::GL46Shader::bind() noexcept
{
    glUseProgram(m_programHandler);
}

// TODO: watch SGP1
void Core::Graphics::API::GL::GL46::GL46Shader::destroy() noexcept
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

        std::string shaderPartVirtualPath = m_path.data() + shaderPartVirtualIncludeType;

        //glDeleteNamedStringARB((GLint) shaderPartVirtualPath.size(), (const GLchar*) shaderPartVirtualPath.c_str());

        glDeleteShader(shaderPartHandler);
    }

    glDeleteProgram(m_programHandler);

    // TODO:: SGP0
    GL46Renderer::getInstance()->checkForErrors();

    m_shaderPartsHandlers.clear();

    std::cout << "shader destroyed" << std::endl;
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