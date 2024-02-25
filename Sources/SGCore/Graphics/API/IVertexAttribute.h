//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IVERTEXATTRIBUTE_H
#define SUNGEARENGINE_IVERTEXATTRIBUTE_H

#include <string>

#include "GraphicsDataTypes.h"
//#include "IVertexBufferLayout.h"

namespace SGCore
{
    // each BufferLayout must have its own set of IVertexAttribute. watch buffer layout
    class IVertexAttribute
    {
    public:
        // offset in vertex buffer
        long long int m_offset = 0;
        // size of attribute
        std::uint32_t m_size = 0;
        // is attribute normalized
        bool m_normalized = false;
        // ID in shader
        std::uint32_t m_ID = 0;
        std::string m_name;
        SGGDataType m_dataType = SGGDataType::SGG_NONE;
        
        bool m_useDivisor = false;
        size_t m_divisor = 1;

        IVertexAttribute() = delete;
        
        IVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) : m_normalized(
                normalized), m_ID(ID), m_name(std::move(name)), m_dataType(dataType)
        {
        
        }
        
        IVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized,
                         const size_t& divisor) : m_normalized(normalized), m_ID(ID), m_name(std::move(name)),
                                                  m_dataType(dataType), m_useDivisor(true), m_divisor(divisor)
        {
        
        }

        //virtual int getAttributeSize() = 0;
        //virtual int getAttributeSizeInBytes() = 0;
        //virtual int get
    };
}

#endif //SUNGEARENGINE_IVERTEXATTRIBUTE_H
