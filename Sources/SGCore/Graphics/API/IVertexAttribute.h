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
        size_t m_offset = 0;
        size_t m_stride = 0;
        // size of attribute
        std::uint32_t m_size = 0;
        std::uint16_t m_scalarsCount = 0;
        // is attribute normalized
        bool m_normalized = false;
        // ID in shader
        std::uint32_t m_ID = 0;
        std::string m_name;
        SGGDataType m_dataType = SGGDataType::SGG_NONE;
        
        bool m_useCustomMarkup = false;
        
        bool m_useDivisor = false;
        size_t m_divisor = 1;
        
        //virtual int getAttributeSize() = 0;
        //virtual int getAttributeSizeInBytes() = 0;
        //virtual int get
    };
}

#endif //SUNGEARENGINE_IVERTEXATTRIBUTE_H
