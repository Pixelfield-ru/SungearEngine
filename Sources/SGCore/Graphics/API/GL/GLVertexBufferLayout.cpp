//
// Created by stuka on 22.05.2023.
//
#include <glad/glad.h>

#include "GLVertexBufferLayout.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "GLGraphicsTypesCaster.h"

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::GLVertexBufferLayout::prepare() noexcept
{
    std::uint32_t offset = 0;
    m_stride = 0;

    for(const Ref<IVertexAttribute>& attribute : m_attributes)
    {
        attribute->m_offset = offset;

        offset += attribute->m_size;
        m_stride += attribute->m_size;
    }

    return shared_from_this();
}

std::uint16_t SGCore::GLVertexBufferLayout::getVertexAttributeSizeInLayout
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

SGCore::GLVertexAttribute* SGCore::GLVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept
{
    return new GLVertexAttribute(ID, std::move(name), dataType, false);
}

SGCore::GLVertexAttribute* SGCore::GLVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept
{
    return new GLVertexAttribute(ID, std::move(name), dataType, normalized);
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::GLVertexBufferLayout::addAttribute
(Ref<IVertexAttribute> attribute) noexcept
{
    attribute->m_size = getSGGDataTypeSizeInBytes(attribute->m_dataType);
    m_attributes.push_back(std::move(attribute));

    return shared_from_this();
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::GLVertexBufferLayout::enableAttribute
(const Ref<IVertexAttribute>& attribute) noexcept
{
    glEnableVertexAttribArray(attribute->m_ID);
    glVertexAttribPointer(
            attribute->m_ID,
            getVertexAttributeSizeInLayout(attribute->m_dataType),
            GLGraphicsTypesCaster::sggDataTypeToGL(attribute->m_dataType),
            attribute->m_normalized,
            (GLsizei) m_stride,
            (GLvoid*) attribute->m_offset
            );

    //glDisableVertexAttribArray(attribute->m_ID);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::GLVertexBufferLayout::enableAttributes() noexcept
{
    for(auto& attribute : m_attributes)
    {
        enableAttribute(attribute);
    }

    return shared_from_this();
}

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::GLVertexBufferLayout::reset() noexcept
{
    m_stride = 0;
    m_attributes.clear();

    return shared_from_this();
}
