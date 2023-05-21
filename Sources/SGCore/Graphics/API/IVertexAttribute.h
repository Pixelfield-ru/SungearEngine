//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IVERTEXATTRIBUTE_H
#define SUNGEARENGINE_IVERTEXATTRIBUTE_H

#include <string>

namespace Core::Graphics::API
{
    // each BufferLayout must have its own set of IVertexAttribute. watch buffer layout
    class IVertexAttribute
    {
    private:
        // offset in vertex buffer
        int m_offset = 0;
        // size of attribute
        int m_size = 0;
        // is attribute normalized
        bool m_normalized = false;

    public:
        // ID in shader
        int m_ID = 0;
        std::string m_name;
        int m_dataType = 0;

        IVertexAttribute() = delete;

        IVertexAttribute(int ID, std::string name, int dataType, bool normalized) : m_ID(ID), m_name(std::move(name)), m_dataType(dataType), m_normalized(normalized)
        {

        }

        virtual int getAttributeSize() = 0;
        virtual int getAttributeSizeInBytes() = 0;
    };
}

#endif //SUNGEARENGINE_IVERTEXATTRIBUTE_H
