//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IINDEXBUFFER_H
#define NATIVECORE_IINDEXBUFFER_H

#include <SGCore/pch.h>

#include "GraphicsDataTypes.h"

namespace SGCore
{
    class IIndexBuffer : public std::enable_shared_from_this<IIndexBuffer>
    {
    protected:
        SGGUsage m_usage = SGGUsage::SGG_STATIC;

    public:
        virtual ~IIndexBuffer() = default;

        virtual std::shared_ptr<IIndexBuffer> create() = 0;
        virtual std::shared_ptr<IIndexBuffer> create(const size_t& byteSize) = 0;
        virtual void destroy() = 0;

        virtual std::shared_ptr<IIndexBuffer> putData(const std::vector<std::uint32_t>& data) = 0;
        virtual void subData(const std::vector<std::uint32_t>& data, const int& offset) = 0;
        virtual void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) = 0;
        virtual std::shared_ptr<IIndexBuffer> bind() = 0;

        virtual std::shared_ptr<IIndexBuffer> setUsage(SGGUsage) = 0;
    };
}

#endif //NATIVECORE_IINDEXBUFFER_H
