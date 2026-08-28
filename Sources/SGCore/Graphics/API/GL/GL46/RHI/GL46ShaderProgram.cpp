//
// Created by 8bitniksis on 17.08.2026.
//

#include "GL46ShaderProgram.h"

#include <algorithm>
#include <utility>
#include <vector>

#include "SGCore/Logger/Logger.h"

namespace
{
    GLenum toGLStage(SGCore::SGSLESubShaderType type) noexcept
    {
        switch(type)
        {
            case SGCore::SST_VERTEX: return GL_VERTEX_SHADER;
            case SGCore::SST_FRAGMENT: return GL_FRAGMENT_SHADER;
            case SGCore::SST_GEOMETRY: return GL_GEOMETRY_SHADER;
            case SGCore::SST_COMPUTE: return GL_COMPUTE_SHADER;
            case SGCore::SST_TESS_CONTROL: return GL_TESS_CONTROL_SHADER;
            case SGCore::SST_TESS_EVALUATION: return GL_TESS_EVALUATION_SHADER;
            default: return 0;
        }
    }

    bool isSamplerType(GLenum type) noexcept
    {
        switch(type)
        {
            case GL_SAMPLER_1D: case GL_SAMPLER_2D: case GL_SAMPLER_3D: case GL_SAMPLER_CUBE:
            case GL_SAMPLER_1D_SHADOW: case GL_SAMPLER_2D_SHADOW: case GL_SAMPLER_1D_ARRAY:
            case GL_SAMPLER_2D_ARRAY: case GL_SAMPLER_1D_ARRAY_SHADOW: case GL_SAMPLER_2D_ARRAY_SHADOW:
            case GL_SAMPLER_2D_MULTISAMPLE: case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: case GL_SAMPLER_CUBE_SHADOW:
            case GL_SAMPLER_BUFFER: case GL_SAMPLER_2D_RECT: case GL_SAMPLER_2D_RECT_SHADOW:
            case GL_INT_SAMPLER_1D: case GL_INT_SAMPLER_2D: case GL_INT_SAMPLER_3D: case GL_INT_SAMPLER_CUBE:
            case GL_INT_SAMPLER_1D_ARRAY: case GL_INT_SAMPLER_2D_ARRAY: case GL_INT_SAMPLER_2D_MULTISAMPLE:
            case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: case GL_INT_SAMPLER_BUFFER: case GL_INT_SAMPLER_2D_RECT:
            case GL_UNSIGNED_INT_SAMPLER_1D: case GL_UNSIGNED_INT_SAMPLER_2D: case GL_UNSIGNED_INT_SAMPLER_3D:
            case GL_UNSIGNED_INT_SAMPLER_CUBE: case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
            case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
            case GL_UNSIGNED_INT_SAMPLER_BUFFER: case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
                return true;
            default:
                return false;
        }
    }

    bool isTexelBufferSampler(GLenum type) noexcept
    {
        return type == GL_SAMPLER_BUFFER || type == GL_INT_SAMPLER_BUFFER || type == GL_UNSIGNED_INT_SAMPLER_BUFFER;
    }

    std::string resourceName(GLuint program, GLenum interface, GLuint index) noexcept
    {
        GLint length = 0;
        const GLenum property = GL_NAME_LENGTH;
        glGetProgramResourceiv(program, interface, index, 1, &property, 1, nullptr, &length);
        std::string name(static_cast<std::size_t>(length > 0 ? length - 1 : 0), '\0');
        if(length > 1)
        {
            glGetProgramResourceName(program, interface, index, length, nullptr, name.data());
        }
        // arrays are reported as "name[0]"
        if(const auto bracket = name.find('['); bracket != std::string::npos) name.erase(bracket);
        return name;
    }
}

SGCore::GL46ShaderProgram::GL46ShaderProgram(const ShaderProgramDesc& desc) noexcept
{
    m_debugName = desc.m_debugName;

    std::vector<GLuint> stages;
    for(const auto& stage : desc.m_stages)
    {
        const GLuint handle = compileStage(stage);
        if(handle == 0)
        {
            for(const auto compiled : stages) glDeleteShader(compiled);
            return;
        }
        stages.push_back(handle);
    }

    m_program = glCreateProgram();
    for(const auto handle : stages) glAttachShader(m_program, handle);
    glLinkProgram(m_program);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint length = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<std::size_t>(length), '\0');
        if(length > 0) glGetProgramInfoLog(m_program, length, nullptr, log.data());
        m_log += "[" + m_debugName + "] link failed:\n" + log + "\n";
        SG_LOG_E("GL46ShaderProgram '{}': link failed:\n{}", m_debugName, log);

        for(const auto handle : stages) { glDetachShader(m_program, handle); glDeleteShader(handle); }
        glDeleteProgram(m_program);
        m_program = 0;
        return;
    }

    for(const auto handle : stages) { glDetachShader(m_program, handle); glDeleteShader(handle); }

    if(!m_debugName.empty())
    {
        glObjectLabel(GL_PROGRAM, m_program, static_cast<GLsizei>(m_debugName.size()), m_debugName.c_str());
    }

    reflect();
}

SGCore::GL46ShaderProgram::~GL46ShaderProgram()
{
    if(m_program) glDeleteProgram(m_program);
}

GLuint SGCore::GL46ShaderProgram::compileStage(const ShaderStageSource& stage) noexcept
{
    const GLenum glStage = toGLStage(stage.m_type);
    if(glStage == 0)
    {
        m_log += "[" + m_debugName + "] unsupported stage type\n";
        return 0;
    }

    std::string source = stage.m_code;
    if(source.rfind("#version", 0) != 0) source = "#version 460 core\n" + source;

    const GLuint handle = glCreateShader(glStage);
    const char* sourcePtr = source.c_str();
    glShaderSource(handle, 1, &sourcePtr, nullptr);
    glCompileShader(handle);

    GLint compiled = GL_FALSE;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLint length = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<std::size_t>(length), '\0');
        if(length > 0) glGetShaderInfoLog(handle, length, nullptr, log.data());
        m_log += "[" + m_debugName + " / stage " + std::to_string(std::to_underlying(stage.m_type)) + "] compile failed:\n" + log + "\n";
        SG_LOG_E("GL46ShaderProgram '{}': stage {} compile failed:\n{}", m_debugName, std::to_underlying(stage.m_type), log);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
}

void SGCore::GL46ShaderProgram::reflect() noexcept
{
    reflectProgram(m_program, m_reflection);
}

void SGCore::GL46ShaderProgram::reflectProgram(GLuint program, ShaderReflection& out) noexcept
{
    out = ShaderReflection { };

    // uniform blocks: name, binding point, size, members with offsets
    GLint blocksCount = 0;
    glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &blocksCount);
    for(GLint b = 0; b < blocksCount; ++b)
    {
        const GLuint blockIndex = static_cast<GLuint>(b);
        ShaderReflection::DescriptorBinding binding;
        binding.m_type = ShaderDescriptorType::UNIFORM_BUFFER;
        binding.m_name = resourceName(program, GL_UNIFORM_BLOCK, blockIndex);

        const GLenum properties[] = { GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE, GL_NUM_ACTIVE_VARIABLES,
                                      GL_REFERENCED_BY_VERTEX_SHADER, GL_REFERENCED_BY_FRAGMENT_SHADER,
                                      GL_REFERENCED_BY_GEOMETRY_SHADER, GL_REFERENCED_BY_COMPUTE_SHADER,
                                      GL_REFERENCED_BY_TESS_CONTROL_SHADER, GL_REFERENCED_BY_TESS_EVALUATION_SHADER };
        GLint values[9] = { };
        glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, blockIndex, 9, properties, 9, nullptr, values);

        binding.m_binding = static_cast<std::uint32_t>(values[0]);
        binding.m_blockSize = static_cast<std::uint32_t>(values[1]);
        if(values[3]) binding.m_stages |= 1u << SST_VERTEX;
        if(values[4]) binding.m_stages |= 1u << SST_FRAGMENT;
        if(values[5]) binding.m_stages |= 1u << SST_GEOMETRY;
        if(values[6]) binding.m_stages |= 1u << SST_COMPUTE;
        if(values[7]) binding.m_stages |= 1u << SST_TESS_CONTROL;
        if(values[8]) binding.m_stages |= 1u << SST_TESS_EVALUATION;

        const GLint variablesCount = values[2];
        if(variablesCount > 0)
        {
            std::vector<GLint> variables(static_cast<std::size_t>(variablesCount));
            const GLenum activeVariables = GL_ACTIVE_VARIABLES;
            glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, blockIndex, 1, &activeVariables, variablesCount, nullptr, variables.data());

            for(const auto variable : variables)
            {
                const GLuint uniformIndex = static_cast<GLuint>(variable);
                const GLenum memberProperties[] = { GL_OFFSET, GL_ARRAY_SIZE, GL_ARRAY_STRIDE, GL_TYPE };
                GLint memberValues[4] = { };
                glGetProgramResourceiv(program, GL_UNIFORM, uniformIndex, 4, memberProperties, 4, nullptr, memberValues);

                ShaderReflection::BlockMember member;
                member.m_name = resourceName(program, GL_UNIFORM, uniformIndex);
                // members of blocks that have an instance name are reported as "Block.member"
                if(const auto dot = member.m_name.find('.'); dot != std::string::npos)
                {
                    member.m_name.erase(0, dot + 1);
                }
                member.m_offset = static_cast<std::uint32_t>(memberValues[0]);
                member.m_arrayCount = memberValues[1] > 0 ? static_cast<std::uint32_t>(memberValues[1]) : 1;
                // GL does not report a member size directly; for arrays stride*count is exact,
                // scalars/vectors are sized by type below via the padded stride heuristic
                member.m_size = memberValues[2] > 0 ? static_cast<std::uint32_t>(memberValues[2]) * member.m_arrayCount : 0;
                // for arrays m_paddedSize is the stride of one element (what "name[i]" addressing needs)
                member.m_paddedSize = memberValues[2] > 0 ? static_cast<std::uint32_t>(memberValues[2]) : 0;
                binding.m_members.push_back(std::move(member));
            }

            // members without array stride: size = distance to the next member (or block end)
            std::sort(binding.m_members.begin(), binding.m_members.end(),
                      [](const auto& a, const auto& b) { return a.m_offset < b.m_offset; });
            for(std::size_t i = 0; i < binding.m_members.size(); ++i)
            {
                if(binding.m_members[i].m_size != 0) continue;
                const std::uint32_t next = i + 1 < binding.m_members.size() ? binding.m_members[i + 1].m_offset : binding.m_blockSize;
                binding.m_members[i].m_size = next - binding.m_members[i].m_offset;
                binding.m_members[i].m_paddedSize = binding.m_members[i].m_size;
            }
        }

        out.m_bindings.push_back(std::move(binding));
    }

    // samplers: default-block uniforms of sampler type; the texture unit is the uniform's value
    GLint uniformsCount = 0;
    glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformsCount);
    for(GLint u = 0; u < uniformsCount; ++u)
    {
        const GLuint uniformIndex = static_cast<GLuint>(u);
        const GLenum properties[] = { GL_TYPE, GL_BLOCK_INDEX, GL_LOCATION, GL_ARRAY_SIZE };
        GLint values[4] = { };
        glGetProgramResourceiv(program, GL_UNIFORM, uniformIndex, 4, properties, 4, nullptr, values);

        const auto type = static_cast<GLenum>(values[0]);
        if(values[1] != -1 || !isSamplerType(type) || values[2] < 0) continue;

        ShaderReflection::DescriptorBinding binding;
        binding.m_type = isTexelBufferSampler(type) ? ShaderDescriptorType::UNIFORM_TEXEL_BUFFER
                                                    : ShaderDescriptorType::COMBINED_IMAGE_SAMPLER;
        binding.m_name = resourceName(program, GL_UNIFORM, uniformIndex);
        binding.m_count = values[3] > 0 ? static_cast<std::uint32_t>(values[3]) : 1;

        GLint unit = 0;
        glGetUniformiv(program, values[2], &unit);
        binding.m_binding = static_cast<std::uint32_t>(unit);
        out.m_bindings.push_back(std::move(binding));
    }

    // vertex inputs
    GLint inputsCount = 0;
    glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputsCount);
    for(GLint i = 0; i < inputsCount; ++i)
    {
        const GLuint inputIndex = static_cast<GLuint>(i);
        const GLenum properties[] = { GL_LOCATION, GL_REFERENCED_BY_VERTEX_SHADER, GL_TYPE };
        GLint values[3] = { };
        glGetProgramResourceiv(program, GL_PROGRAM_INPUT, inputIndex, 3, properties, 3, nullptr, values);
        if(!values[1] || values[0] < 0) continue;

        ShaderReflection::VertexInput input;
        input.m_name = resourceName(program, GL_PROGRAM_INPUT, inputIndex);
        if(input.m_name.rfind("gl_", 0) == 0) continue;
        input.m_location = static_cast<std::uint32_t>(values[0]);
        input.m_format = static_cast<std::uint32_t>(values[2]);
        out.m_vertexInputs.push_back(std::move(input));
    }
}
