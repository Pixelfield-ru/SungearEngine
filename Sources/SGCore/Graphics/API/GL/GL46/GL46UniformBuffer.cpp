//
// Created by stuka on 08.06.2023.
//

#include "GL46UniformBuffer.h"
#include <glad/glad.h>

std::shared_ptr<Core::Graphics::API::IUniformBuffer> Core::Graphics::API::GL::GL46::GL46UniformBuffer::create() noexcept
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::API::IUniformBuffer> Core::Graphics::API::GL::GL46::GL46UniformBuffer::bind() noexcept
{
    // TODO: opengl 3.1 code. must be updated to 4.6
    for(auto [uniformsIter, dataIterator] = std::tuple { m_uniforms.begin(), m_begin };
    uniformsIter != m_uniforms.end() && dataIterator.current != m_end.current;
    uniformsIter++, dataIterator++)
    {
        switch(uniformsIter->m_dataType)
        {
            case SGG_NONE: break;

            case SGG_INT: glUniform1i(uniformsIter->m_location, *reinterpret_cast<int*>(dataIterator.current)); break;
            case SGG_INT2: break;
            case SGG_INT3: break;
            case SGG_INT4: break;

            case SGG_FLOAT: break;
            case SGG_FLOAT2: break;
            case SGG_FLOAT3: break;
            case SGG_FLOAT4:
            {
                float f0 = *reinterpret_cast<float*>(dataIterator.current);
                float f1 = *(reinterpret_cast<float*>(dataIterator.current) + 1);
                float f2 = *(reinterpret_cast<float*>(dataIterator.current) + 2);
                float f3 = *(reinterpret_cast<float*>(dataIterator.current) + 3);

                glUniform4f(uniformsIter->m_location, f0, f1, f2, f3);

                break;
            }

            case SGG_MAT2: break;
            case SGG_MAT3: break;
            case SGG_MAT4: break;

            case SGG_BOOL: break;

            case SGG_STRUCT_BEGIN: break;
            case SGG_STRUCT_END: break;

            default: break;
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::API::IUniformBuffer> Core::Graphics::API::GL::GL46::GL46UniformBuffer::updateLocations(
        const Core::Graphics::API::IShader& fromShader) noexcept
{
    for(auto& uniform : m_uniforms)
    {
        uniform.m_location = fromShader.getShaderUniformLocation(uniform.m_name);
    }

    return shared_from_this();
}

void Core::Graphics::API::GL::GL46::GL46UniformBuffer::destroy() noexcept
{

}
