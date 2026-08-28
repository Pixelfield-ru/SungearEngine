//
// Created by stuka on 24.04.2023.
//

#pragma once

#include <cstdint>
#include <sgcore_export.h>

#include "GraphicsDataTypes.h"

namespace SGCore
{
    class IVertexArray;

    class SGCORE_EXPORT IVertexBuffer
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

        /// Backend-agnostic record of one attribute, kept so the RHI can build a VertexInputDesc
        /// for legacy meshes without asking the GL objects.
        struct AttributeDesc
        {
            std::uint32_t m_location { };
            std::int32_t m_scalarsCount { };
            SGGDataType m_dataType = SGGDataType::SGG_FLOAT;
            bool m_isNormalized { };
            std::int32_t m_stride { };
            std::uint64_t m_offsetInStruct { };
            std::int32_t m_divisor { };
        };

        void addAttribute(std::uint32_t location,
                          std::int32_t scalarsCount,
                          SGGDataType dataType,
                          bool isNormalized,
                          std::int32_t stride,
                          std::uint64_t offsetInStruct,
                          std::int32_t divisor) noexcept;

        void addAttribute(std::uint32_t location,
                          std::int32_t scalarsCount,
                          SGGDataType dataType,
                          bool isNormalized,
                          std::int32_t stride,
                          std::uint64_t offsetInStruct) noexcept;

        virtual void useAttributes() const noexcept = 0;

        /// Drops every recorded attribute, on this object and in the backend. A buffer whose layout is
        /// redefined (the same mesh buffer bound into a second vertex array at a different location
        /// offset — that is what Instancing does) must start from empty: addAttribute only ever
        /// appends, and useAttributes replays the whole list into the bound vertex array.
        void clearAttributes() noexcept
        {
            m_attributesDescs.clear();
            clearAttributesImpl();
        }

        /// Backend object handle (GL: buffer name), for wrapping legacy buffers into RHI objects.
        [[nodiscard]] virtual std::uintptr_t getNativeHandle() const noexcept { return 0; }

        const std::vector<std::uint8_t>& getData() const noexcept;
        [[nodiscard]] const std::vector<AttributeDesc>& getAttributes() const noexcept { return m_attributesDescs; }

    protected:
        std::vector<std::uint8_t> m_data;
        virtual void clearAttributesImpl() noexcept { }

        std::vector<AttributeDesc> m_attributesDescs;

        virtual void addAttributeImpl(std::uint32_t location,
                                      std::int32_t scalarsCount,
                                      SGGDataType dataType,
                                      bool isNormalized,
                                      std::int32_t stride,
                                      std::uint64_t offsetInStruct,
                                      std::int32_t divisor) noexcept = 0;
        IVertexArray* m_parentVertexArray { };

        virtual void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) = 0;
    };
}
