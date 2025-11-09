//
// Created by stuka on 24.04.2023.
//

#pragma once

#include "GraphicsDataTypes.h"

namespace SGCore
{
    class IVertexArray;

    class IVertexBuffer
    {
        friend class IVertexArray;

    protected:
        SGGUsage m_usage = SGGUsage::SGG_STATIC;

    public:
        virtual ~IVertexBuffer();

        virtual void create() = 0;
        virtual void create(const size_t& byteSize) = 0;
        virtual void destroy() = 0;
        
        template<typename DataType>
        requires(std::is_trivially_copyable_v<DataType>)
        void putData(const std::vector<DataType>& data) noexcept
        {
            static constexpr auto sizeof_type = sizeof(DataType);

            m_data.resize(data.size() * sizeof_type);
            std::memcpy(m_data.data(), data.data(), data.size() * sizeof_type);

            subDataOnGAPISide(data.data(), data.size() * sizeof_type, 0, true);
        }
        
        template<typename DataType>
        requires(std::is_trivially_copyable_v<DataType>)
        void subData(const std::vector<DataType>& data, const size_t& elementsOffset) noexcept
        {
            static constexpr auto sizeof_type = sizeof(DataType);

            if((elementsOffset + data.size()) * sizeof_type > m_data.size()) return;

            std::memcpy(m_data.data() + elementsOffset * sizeof_type, data.data(), data.size() * sizeof_type);

            subDataOnGAPISide(data.data(), data.size() * sizeof_type, elementsOffset * sizeof_type, false);
        }
        
        template<typename DataType>
        requires(std::is_trivially_copyable_v<DataType>)
        void subData(const DataType* data, const size_t& elementsCount, const size_t& elementsOffset) noexcept
        {
            static constexpr auto sizeof_type = sizeof(DataType);

            if((elementsOffset + elementsCount) * sizeof_type > m_data.size()) return;

            std::memcpy(m_data.data() + elementsOffset * sizeof_type, data, elementsCount * sizeof_type);

            subDataOnGAPISide(data, elementsCount * sizeof_type, elementsOffset * sizeof_type, false);
        }
        
        virtual void bind() = 0;

        virtual void setUsage(SGGUsage) = 0;

        virtual void addAttribute(std::uint32_t location,
                                  std::int32_t scalarsCount,
                                  SGGDataType dataType,
                                  bool isNormalized,
                                  std::int32_t stride,
                                  std::uint64_t offsetInStruct,
                                  std::int32_t divisor) noexcept = 0;

        void addAttribute(std::uint32_t location,
                          std::int32_t scalarsCount,
                          SGGDataType dataType,
                          bool isNormalized,
                          std::int32_t stride,
                          std::uint64_t offsetInStruct) noexcept;

        virtual void useAttributes() const noexcept = 0;

        const std::vector<std::uint8_t>& getData() const noexcept;

    protected:
        std::vector<std::uint8_t> m_data;
        IVertexArray* m_parentVertexArray { };

        virtual void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) = 0;
    };
}
