//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IVERTEXBUFFER_H
#define NATIVECORE_IVERTEXBUFFER_H

#include <iostream>
#include <vector>

#include "GraphicsDataTypes.h"

namespace Core::Graphics::API
{
    class IVertexBuffer
    {
    protected:
        SGGBufferUsage m_usage = SGGBufferUsage::SGG_BUFFER_USAGE_STATIC;

    public:
        virtual ~IVertexBuffer() = default;

        virtual IVertexBuffer* create() = 0;

        virtual void destroy() = 0;

        virtual IVertexBuffer* putData(std::vector<float> data) = 0;
        virtual IVertexBuffer* subData(std::vector<float> data, const int& offset) = 0;
        virtual IVertexBuffer* bind() = 0;

        virtual IVertexBuffer* setUsage(SGGBufferUsage) = 0;
    };
}

#endif //NATIVECORE_IVERTEXBUFFER_H
