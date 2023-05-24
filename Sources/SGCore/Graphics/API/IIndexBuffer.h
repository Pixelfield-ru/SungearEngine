//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IINDEXBUFFER_H
#define NATIVECORE_IINDEXBUFFER_H

#include <vector>

#include "GraphicsDataTypes.h"

namespace Core::Graphics::API
{
    class IIndexBuffer
    {
    protected:
        SGGBufferUsage m_usage = SGGBufferUsage::SGG_STATIC;

    public:
        virtual ~IIndexBuffer() = default;

        virtual IIndexBuffer* create() = 0;

        virtual void destroy() = 0;

        virtual IIndexBuffer* putData(std::vector<int> data) = 0;
        virtual IIndexBuffer* subData(std::vector<int> data, const int& offset) = 0;
        virtual IIndexBuffer* bind() = 0;

        virtual IIndexBuffer* setUsage(SGGBufferUsage) = 0;
    };
}

#endif //NATIVECORE_IINDEXBUFFER_H
