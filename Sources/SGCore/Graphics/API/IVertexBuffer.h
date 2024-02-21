//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IVERTEXBUFFER_H
#define NATIVECORE_IVERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <memory>

#include "GraphicsDataTypes.h"

namespace SGCore
{
    class IVertexBuffer : public std::enable_shared_from_this<IVertexBuffer>
    {
    protected:
        SGGUsage m_usage = SGGUsage::SGG_STATIC;

    public:
        virtual ~IVertexBuffer() = default;

        virtual std::shared_ptr<IVertexBuffer> create() = 0;
        virtual void destroy() = 0;

        virtual std::shared_ptr<IVertexBuffer> putData(const std::vector<float>& data) = 0;
        /*
         * TODO: задуматься над темплейтным методом для того, чтобы класть не только float, но и другие числовые типы.
         * но тогда придётся отказаться от хранения копии / оригинала вектора
        */
        virtual void subData(const std::vector<float>& data, const size_t& offset) = 0;
        virtual void subData(float* data, const size_t& elementsCount, const size_t& offset) = 0;
        virtual std::shared_ptr<IVertexBuffer> bind() = 0;

        virtual std::shared_ptr<IVertexBuffer> setUsage(SGGUsage) = 0;
    };
}

#endif //NATIVECORE_IVERTEXBUFFER_H
