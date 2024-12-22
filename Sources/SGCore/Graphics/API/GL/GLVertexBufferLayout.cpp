//
// Created by stuka on 22.05.2023.
//

#include "GLVertexBufferLayout.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "GLGraphicsTypesCaster.h"

SGCore::Ref<SGCore::IVertexBufferLayout> SGCore::GLVertexBufferLayout::prepare() noexcept
{
    std::uint32_t offset = 0;
    m_stride = 0;

    for(const auto& attribute : m_attributes)
    {
        if(!attribute->m_useCustomMarkup)
        {
            attribute->m_offset = offset;
            attribute->m_scalarsCount = getVertexAttributeScalarsCount(attribute->m_dataType);
        }

        offset += attribute->m_size;
        m_stride += attribute->m_size;
    }
    
    for(const auto& attribute : m_attributes)
    {
        if(!attribute->m_useCustomMarkup)
        {
            attribute->m_stride = m_stride;
        }
    }

    return shared_from_this();
}

std::uint16_t SGCore::GLVertexBufferLayout::getVertexAttributeScalarsCount
(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGGDataType::SGG_NONE: size = 0; break;

        case SGGDataType::SGG_INT: size = 1; break;
        case SGGDataType::SGG_INT2: size = 2; break;
        case SGGDataType::SGG_INT3: size = 3; break;
        case SGGDataType::SGG_INT4: size = 4; break;

        case SGGDataType::SGG_FLOAT: size = 1; break;
        case SGGDataType::SGG_FLOAT2: size = 2; break;
        case SGGDataType::SGG_FLOAT3: size = 3; break;
        case SGGDataType::SGG_FLOAT4: size = 4; break;

        case SGGDataType::SGG_MAT2: size = 2 * 2; break;
        case SGGDataType::SGG_MAT3: size = 3 * 3; break;
        case SGGDataType::SGG_MAT4: size = 4 * 4; break;

        case SGGDataType::SGG_BOOL: size = 1; break;

        default: size = 0; break;
    }

    return size;
}

SGCore::GLVertexAttribute* SGCore::GLVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept
{
    auto* attrib = new GLVertexAttribute;
    attrib->m_ID = ID;
    attrib->m_name = name;
    attrib->m_dataType = dataType;
    
    return attrib;
}

SGCore::GLVertexAttribute* SGCore::GLVertexBufferLayout::createVertexAttribute
(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept
{
    auto* attrib = new GLVertexAttribute;
    attrib->m_ID = ID;
    attrib->m_name = name;
    attrib->m_dataType = dataType;
    attrib->m_normalized = normalized;
    
    return attrib;
}

SGCore::GLVertexAttribute*
SGCore::GLVertexBufferLayout::createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType,
                                                    bool normalized, const size_t& divisor) noexcept
{
    auto* attrib = new GLVertexAttribute;
    attrib->m_ID = ID;
    attrib->m_name = name;
    attrib->m_dataType = dataType;
    attrib->m_divisor = divisor;
    attrib->m_normalized = normalized;
    attrib->m_useDivisor = true;
    
    return attrib;
}

SGCore::GLVertexAttribute*
SGCore::GLVertexBufferLayout::createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType,
                                                    const size_t& divisor) noexcept
{
    auto* attrib = new GLVertexAttribute;
    attrib->m_ID = ID;
    attrib->m_name = name;
    attrib->m_dataType = dataType;
    attrib->m_divisor = divisor;
    attrib->m_useDivisor = true;
    
    return attrib;
}

SGCore::GLVertexAttribute*
SGCore::GLVertexBufferLayout::createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType,
                                                    const uint16_t& scalarsCount, bool normalized, const size_t& stride,
                                                    const size_t& offset, const size_t& divisor) noexcept
{
    auto* attrib = new GLVertexAttribute;
    attrib->m_ID = ID;
    attrib->m_name = name;
    attrib->m_dataType = dataType;
    attrib->m_divisor = divisor;
    attrib->m_stride = stride;
    attrib->m_offset = offset;
    attrib->m_useDivisor = true;
    attrib->m_useCustomMarkup = true;
    attrib->m_normalized = normalized;
    attrib->m_scalarsCount = scalarsCount;
    
    return attrib;
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
    if(attribute->m_dataType == SGGDataType::SGG_INT ||
       attribute->m_dataType == SGGDataType::SGG_INT2 ||
       attribute->m_dataType == SGGDataType::SGG_INT3 ||
       attribute->m_dataType == SGGDataType::SGG_INT4 ||
       attribute->m_dataType == SGGDataType::SGG_UNSIGNED_INT)
    {
        glVertexAttribIPointer(attribute->m_ID,
                               attribute->m_scalarsCount,
                               GLGraphicsTypesCaster::sggDataTypeToGL(attribute->m_dataType),
                               (GLsizei) attribute->m_stride,
                               (GLvoid*) attribute->m_offset);
    }
    else
    {
        glVertexAttribPointer(
                attribute->m_ID,
                attribute->m_scalarsCount,
                GLGraphicsTypesCaster::sggDataTypeToGL(attribute->m_dataType),
                attribute->m_normalized,
                (GLsizei) attribute->m_stride,
                (GLvoid*) attribute->m_offset
        );
    }
    
    if(attribute->m_useDivisor)
    {
        glVertexAttribDivisor(attribute->m_ID, attribute->m_divisor);
    }

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

