//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IINDEXBUFFER_H
#define NATIVECORE_IINDEXBUFFER_H

#include "GraphicsDataTypes.h"

namespace SGCore
{
    class IIndexBuffer
    {
    protected:
        SGGUsage m_usage = SGGUsage::SGG_STATIC;

    public:
        virtual ~IIndexBuffer() = default;

        virtual void create() = 0;
        virtual void create(const size_t& byteSize) = 0;
        virtual void destroy() = 0;

        virtual void putData(const std::vector<std::uint32_t>& data) = 0;
        virtual void subData(const std::vector<std::uint32_t>& data, const int& offset) = 0;
        virtual void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) = 0;
        virtual void bind() = 0;

        virtual void setUsage(SGGUsage) = 0;

        const std::vector<std::uint32_t> getData() const noexcept;

    protected:
        std::vector<std::uint32_t> m_data;
    };
}

#endif //NATIVECORE_IINDEXBUFFER_H
