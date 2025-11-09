//
// Created by stuka on 23.05.2023.
//

#include "GLVertexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

SGCore::GLVertexBuffer::~GLVertexBuffer() noexcept
{
    GLVertexBuffer::destroy();
}

void SGCore::GLVertexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);
}

void SGCore::GLVertexBuffer::create(const size_t& byteSize) noexcept
{
    destroy();

    m_data.resize(byteSize);

    glGenBuffers(1, &m_handler);
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLuint64>(byteSize), nullptr,
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
}

void SGCore::GLVertexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);
}

void SGCore::GLVertexBuffer::subDataOnGAPISide(const void* data, const size_t& bytesCount,
                                               const size_t& bytesOffset,
                                               bool isPutData) noexcept
{
    if(bytesCount == 0) return;

    if(isPutData)
    {
        glBufferData(GL_ARRAY_BUFFER, (GLuint64) bytesCount, data,
                     GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, (GLuint64) bytesOffset, (GLuint64) bytesCount, data);
    }
}

void SGCore::GLVertexBuffer::bind() noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handler);
}

void SGCore::GLVertexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;
}

void SGCore::GLVertexBuffer::addAttribute(std::uint32_t location,
                                          std::int32_t scalarsCount,
                                          SGGDataType dataType,
                                          bool isNormalized,
                                          std::int32_t stride,
                                          std::uint64_t offsetInStruct,
                                          std::int32_t divisor) noexcept
{
    GLVertexAttribute d;
    d.location = location;
    d.size = scalarsCount;
    d.dataType = GLGraphicsTypesCaster::sggDataTypeToGL(dataType);
    d.isNormalized = isNormalized;
    d.stride = stride;
    d.offset = offsetInStruct;
    d.divisor = divisor;

    m_attributes.push_back(d);
}

void SGCore::GLVertexBuffer::useAttributes() const noexcept
{
    for(const auto& attribute : m_attributes)
    {
        glEnableVertexAttribArray(attribute.location);

        if(attribute.dataType == GL_INT ||
           attribute.dataType == GL_UNSIGNED_INT ||
           attribute.dataType == GL_UNSIGNED_SHORT ||
           attribute.dataType == GL_SHORT ||
           attribute.dataType == GL_UNSIGNED_BYTE ||
           attribute.dataType == GL_BYTE)
        {
            glVertexAttribIPointer(attribute.location, attribute.size, attribute.dataType, attribute.stride, (void*) attribute.offset);
        }
        else
        {
            glVertexAttribPointer(attribute.location, attribute.size, attribute.dataType, attribute.isNormalized, attribute.stride, (void*) attribute.offset);
        }

        glVertexAttribDivisor(attribute.location, attribute.divisor);
    }
}
