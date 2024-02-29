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
        
        template<typename DataType>
        requires(std::is_scalar_v<DataType>)
        std::shared_ptr<IVertexBuffer> putData(const std::vector<DataType>& data) noexcept
        {
            subDataOnGAPISide(data.data(), data.size() * sizeof(DataType), 0, true);
            
            return shared_from_this();
        }
        
        template<typename DataType>
        requires(std::is_scalar_v<DataType>)
        void subData(const std::vector<DataType>& data, const size_t& elementsOffset) noexcept
        {
            subDataOnGAPISide(data.data(), data.size() * sizeof(DataType), elementsOffset * sizeof(DataType), false);
        }
        
        template<typename DataType>
        requires(std::is_scalar_v<DataType>)
        void subData(const DataType* data, const size_t& elementsCount, const size_t& elementsOffset) noexcept
        {
            subDataOnGAPISide(data, elementsCount * sizeof(DataType), elementsOffset * sizeof(DataType), false);
        }
        
        virtual std::shared_ptr<IVertexBuffer> bind() = 0;

        virtual std::shared_ptr<IVertexBuffer> setUsage(SGGUsage) = 0;
        
    protected:
        virtual void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) = 0;
    };
}

#endif //NATIVECORE_IVERTEXBUFFER_H
