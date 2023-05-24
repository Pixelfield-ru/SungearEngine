//
// Created by stuka on 22.05.2023.
//

#include "GLVertexBufferLayout.h"

#include <glad/glad.h>

#include "GL46/GL46Renderer.h"

Core::Graphics::API::GL::GLVertexBufferLayout* Core::Graphics::API::GL::GLVertexBufferLayout::prepare() noexcept
{
    std::uint32_t offset = 0;
    m_stride = 0;

    for(const std::shared_ptr<IVertexAttribute>& attribute : m_attributes)
    {
        attribute->m_offset = offset;

        offset += attribute->m_size;
        m_stride += attribute->m_size;
    }

    return this;
}

std::uint16_t Core::Graphics::API::GL::GLVertexBufferLayout::getVertexAttributeSizeInLayout
(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGG_NONE: size = 0; break;

        case SGG_INT: size = 1; break;
        case SGG_INT2: size = 2; break;
        case SGG_INT3: size = 3; break;
        case SGG_INT4: size = 4; break;

        case SGG_FLOAT: size = 1; break;
        case SGG_FLOAT2: size = 2; break;
        case SGG_FLOAT3: size = 3; break;
        case SGG_FLOAT4: size = 4; break;

        case SGG_MAT2: size = 2 * 2; break;
        case SGG_MAT3: size = 3 * 3; break;
        case SGG_MAT4: size = 4 * 4; break;

        case SGG_BOOL: size = 1; break;

        default: size = 0; break;
    }

    return size;
}

std::uint16_t Core::Graphics::API::GL::GLVertexBufferLayout::getVertexAttributeSizeInBytes
(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGG_NONE: size = 0; break;

        case SGG_INT: size = 4; break;
        case SGG_INT2: size = 4 * 2; break;
        case SGG_INT3: size = 4 * 3; break;
        case SGG_INT4: size = 4 * 4; break;

        case SGG_FLOAT: size = 4; break;
        case SGG_FLOAT2: size = 4 * 2; break;
        case SGG_FLOAT3: size = 4 * 3; break;
        case SGG_FLOAT4: size = 4 * 4; break;

        case SGG_MAT2: size = 4 * 2 * 2; break;
        case SGG_MAT3: size = 4 * 3 * 3; break;
        case SGG_MAT4: size = 4 * 4 * 4; break;

        case SGG_BOOL: size = 4; break;

        default: size = 0; break;
    }

    return size;
}

std::uint16_t Core::Graphics::API::GL::GLVertexBufferLayout::getVertexAttributeAPIDataType
(const SGGDataType& dataType) noexcept
{
    int apiDataType;

    switch(dataType)
    {
        case SGG_NONE: apiDataType = GL_NONE; break;

        case SGG_INT: apiDataType = GL_INT; break;
        case SGG_INT2: apiDataType = GL_INT; break;
        case SGG_INT3: apiDataType = GL_INT; break;
        case SGG_INT4: apiDataType = GL_INT; break;

        case SGG_FLOAT: apiDataType = GL_FLOAT; break;
        case SGG_FLOAT2: apiDataType = GL_FLOAT; break;
        case SGG_FLOAT3: apiDataType = GL_FLOAT; break;
        case SGG_FLOAT4: apiDataType = GL_FLOAT; break;

        case SGG_MAT2: apiDataType = GL_FLOAT; break;
        case SGG_MAT3: apiDataType = GL_FLOAT; break;
        case SGG_MAT4: apiDataType = GL_FLOAT; break;

        case SGG_BOOL: apiDataType = GL_BOOL; break;

        default: apiDataType = GL_NONE; break;
    }

    return apiDataType;
}

Core::Graphics::API::GL::GLVertexAttribute* Core::Graphics::API::GL::GLVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept
{
    return new GLVertexAttribute(ID, std::move(name), dataType, false);
}

Core::Graphics::API::GL::GLVertexAttribute* Core::Graphics::API::GL::GLVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept
{
    return new GLVertexAttribute(ID, std::move(name), dataType, normalized);
}

Core::Graphics::API::GL::GLVertexBufferLayout* Core::Graphics::API::GL::GLVertexBufferLayout::addAttribute
(std::shared_ptr<IVertexAttribute> attribute) noexcept
{
    attribute->m_size = getVertexAttributeSizeInBytes(attribute->m_dataType);
    m_attributes.push_back(std::move(attribute));

    return this;
}

Core::Graphics::API::GL::GLVertexBufferLayout* Core::Graphics::API::GL::GLVertexBufferLayout::enableAttribute
(const std::shared_ptr<IVertexAttribute>& attribute) noexcept
{
    glEnableVertexAttribArray(attribute->m_ID);
    glVertexAttribPointer(
            attribute->m_ID,
            getVertexAttributeSizeInLayout(attribute->m_dataType),
            getVertexAttributeAPIDataType(attribute->m_dataType),
            attribute->m_normalized,
            (GLsizei) m_stride,
            (GLvoid*) attribute->m_offset
            );

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return this;
}

Core::Graphics::API::GL::GLVertexBufferLayout* Core::Graphics::API::GL::GLVertexBufferLayout::enableAttributes() noexcept
{
    for(const std::shared_ptr<IVertexAttribute>& attribute : m_attributes)
    {
        enableAttribute(attribute);
    }

    return this;
}
