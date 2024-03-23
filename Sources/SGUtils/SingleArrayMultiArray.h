//
// Created by ilya on 23.03.24.
//

#ifndef OCEANSEDGE_SINGLEARRAYMULTIARRAY_H
#define OCEANSEDGE_SINGLEARRAYMULTIARRAY_H

#include <cstddef>
#include <type_traits>
#include <utility>
#include <tuple>

#include "SGUtils/Utils.h"

namespace SGCore
{
    template<typename T, long DimensionsCnt>
    struct SingleArrayMultiArray
    {
        using dimensions_tuple_t = SGUtils::Utils::repeated_tuple<SGUtils::Utils::repeated_type<long, DimensionsCnt>>;
        
        SingleArrayMultiArray() = default;
        
        template<typename... DimensionT>
        requires((std::is_integral_v<DimensionT> && ...) && sizeof...(DimensionT) == DimensionsCnt)
        explicit SingleArrayMultiArray(DimensionT... dimensionSizes) noexcept
        {
            m_dimensionsSizes = std::make_tuple(dimensionSizes...);
            
            (std::cout << ... << dimensionSizes) << std::endl;
            
            getOffset(std::array<long, DimensionsCnt>({ (long) dimensionSizes... }), m_size);
            
            std::cout << "m_size : " << m_size << std::endl;
            
            m_buffer = new T[m_size];
        }
        
        ~SingleArrayMultiArray() noexcept
        {
            delete m_buffer;
            m_buffer = nullptr;
        }
        
        template<typename... DimensionT>
        requires((std::is_integral_v<DimensionT> && ...) && sizeof...(DimensionT) == DimensionsCnt)
        T& get(DimensionT... indexes) noexcept
        {
            size_t offset = 0;
            getOffset(std::array<long, DimensionsCnt>({ indexes... }), offset);
            return m_buffer[offset];
        }
        
        T* data() const noexcept
        {
            return m_buffer;
        }
        
        SingleArrayMultiArray& operator=(const SingleArrayMultiArray& other) noexcept
        {
            if(this == std::addressof(other)) return *this;
            
            m_size = other.m_size;
            
            delete m_buffer;
            
            m_buffer = new T[m_size];
            
            std::memcpy(m_buffer, other.m_buffer, m_size * sizeof(T));
            
            return *this;
        }
        
        SingleArrayMultiArray& operator=(SingleArrayMultiArray&& other) noexcept
        {
            if (this == std::addressof(other)) return *this;
            
            m_size = other.m_size;
            
            other.m_size = 0;
            
            m_buffer = other.m_buffer;
            other.m_buffer = nullptr;
            
            return *this;
        }
    
    private:
        template<size_t curIdx = 0>
        void getOffset(const std::array<long, DimensionsCnt>& indices, size_t& offset)
        {
            if constexpr(curIdx == DimensionsCnt - 1)
            {
                offset += indices[curIdx];
            }
            else
            {
                if constexpr(curIdx == 0)
                {
                    offset = 1;
                }
                // std::cout << indices[curIdx] << std::endl;
                offset *= indices[curIdx];
                getOffset<curIdx + 1>(indices, offset);
            }
        }
        
        size_t m_size = 0;
        
        T* m_buffer = nullptr;
        
        dimensions_tuple_t m_dimensionsSizes { };
    };
}

#endif //OCEANSEDGE_SINGLEARRAYMULTIARRAY_H
