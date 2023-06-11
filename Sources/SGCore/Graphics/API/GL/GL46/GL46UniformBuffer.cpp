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
    for(auto [uniformsIter, dataIterator] = std::tuple {m_uniforms.begin(), m_bufferLayout.begin() };
    uniformsIter != m_uniforms.end() && dataIterator != m_bufferLayout.end();
    uniformsIter++, dataIterator++)
    {
        switch(uniformsIter->m_dataType)
        {
            case SGG_NONE: break;

            case SGG_INT: glUniform1i(uniformsIter->m_location, *reinterpret_cast<int*>(*dataIterator)); break;
            case SGG_INT2:
            {
                int s0 = *reinterpret_cast<int*>(*dataIterator);
                int s1 = *(reinterpret_cast<int*>(*dataIterator) + 1);

                glUniform2i(uniformsIter->m_location, s0, s1);

                break;
            }
            case SGG_INT3:
            {
                int s0 = *reinterpret_cast<int*>(*dataIterator);
                int s1 = *(reinterpret_cast<int*>(*dataIterator) + 1);
                int s2 = *(reinterpret_cast<int*>(*dataIterator) + 2);

                glUniform3i(uniformsIter->m_location, s0, s1, s2);

                break;
            }
            case SGG_INT4:
            {
                int s0 = *reinterpret_cast<int*>(*dataIterator);
                int s1 = *(reinterpret_cast<int*>(*dataIterator) + 1);
                int s2 = *(reinterpret_cast<int*>(*dataIterator) + 2);
                int s3 = *(reinterpret_cast<int*>(*dataIterator) + 3);

                glUniform4i(uniformsIter->m_location, s0, s1, s2, s3);

                break;
            }

            case SGG_FLOAT: glUniform1f(uniformsIter->m_location, *reinterpret_cast<float*>(*dataIterator)); break;
            case SGG_FLOAT2:
            {
                float s0 = *reinterpret_cast<float*>(*dataIterator);
                float s1 = *(reinterpret_cast<float*>(*dataIterator) + 1);

                glUniform2f(uniformsIter->m_location, s0, s1);

                break;
            }
            case SGG_FLOAT3:
            {
                float s0 = *reinterpret_cast<float*>(*dataIterator);
                float s1 = *(reinterpret_cast<float*>(*dataIterator) + 1);
                float s2 = *(reinterpret_cast<float*>(*dataIterator) + 2);

                glUniform3f(uniformsIter->m_location, s0, s1, s2);

                break;
            }
            case SGG_FLOAT4:
            {
                float s0 = *reinterpret_cast<float*>(*dataIterator);
                float s1 = *(reinterpret_cast<float*>(*dataIterator) + 1);
                float s2 = *(reinterpret_cast<float*>(*dataIterator) + 2);
                float s3 = *(reinterpret_cast<float*>(*dataIterator) + 3);

                glUniform4f(uniformsIter->m_location, s0, s1, s2, s3);

                break;
            }

            case SGG_MAT2:
            {
                glUniformMatrix2fv(uniformsIter->m_location, 1, false, reinterpret_cast<float*>(*dataIterator));

                break;
            }
            case SGG_MAT3:
            {
                glUniformMatrix3fv(uniformsIter->m_location, 1, false, reinterpret_cast<float*>(*dataIterator));

                break;
            }
            case SGG_MAT4:
            {
                glUniformMatrix4fv(uniformsIter->m_location, 1, false, reinterpret_cast<float*>(*dataIterator));

                break;
            }

            // TODO: impl
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
