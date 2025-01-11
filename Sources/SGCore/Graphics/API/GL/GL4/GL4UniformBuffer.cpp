//
// Created by stuka on 08.06.2023.
//

#include "GL4UniformBuffer.h"

SGCore::GL4UniformBuffer::~GL4UniformBuffer()
{
    destroy();
}

void SGCore::GL4UniformBuffer::bind() noexcept
{
    /*
    // iterating on data and on uniforms to avoid mistakes of going abroad
    for(auto [uniformsIter, dataIterator] = std::tuple {m_uniforms.begin(), m_bufferLayout.begin()};
        uniformsIter != m_uniforms.end() && dataIterator != m_bufferLayout.end(); uniformsIter++, dataIterator++)
    {
        switch(uniformsIter->m_dataType)
        {
            case SGG_NONE: break;

            case SGG_INT:
            {
                glUniform1i(uniformsIter->m_location, *reinterpret_cast<int*>(*dataIterator));

                break;
            }
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

            case SGG_FLOAT:
            {
                glUniform1f(uniformsIter->m_location, *reinterpret_cast<float*>(*dataIterator));

                break;
            }
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
     */
}

void SGCore::GL4UniformBuffer::subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_handler);
    // glBindBufferBase(GL_UNIFORM_BUFFER, m_layoutLocation, m_handler);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, m_buffer + offset);
    // glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr) m_bufferSize, m_buffer, GL_STATIC_DRAW);
}

void SGCore::GL4UniformBuffer::prepare() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);
    glBindBuffer(GL_UNIFORM_BUFFER, m_handler);
    glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, m_buffer, GL_STATIC_DRAW);
    // glBindBufferRange(GL_UNIFORM_BUFFER, m_layoutLocation, m_handler, 0, m_bufferSize);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_layoutLocation, m_handler);
}

void SGCore::GL4UniformBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);
}

void SGCore::GL4UniformBuffer::setLayoutLocation(const uint16_t& location) noexcept
{
    m_layoutLocation = location;
}
