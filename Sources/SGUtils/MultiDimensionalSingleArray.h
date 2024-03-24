#ifndef SUNGEARENGINE_MULTIDIMENSIONALSINGLEARRAY_H
#define SUNGEARENGINE_MULTIDIMENSIONALSINGLEARRAY_H

#include <cstddef>
#include <type_traits>
#include <utility>
#include <tuple>
#include <cstring>

#include "SGUtils/TypeTraits.h"

namespace SGCore
{
    template<typename T, long DimensionsCnt>
    struct MultiDimensionalSingleArray
    {
        using dimensions_tuple_t = repeated_tuple<repeated_type<long, DimensionsCnt>>;
        
        using type = T;
        using const_type = T;
        using pointer_t = T*;
        using const_pointer_t = T*;
        static const constexpr long dimensions_count = DimensionsCnt;
        
        MultiDimensionalSingleArray() = default;
        
        MultiDimensionalSingleArray(const MultiDimensionalSingleArray& other) noexcept
        {
            *this = other;
        }
        
        MultiDimensionalSingleArray(MultiDimensionalSingleArray&& other) noexcept
        {
            *this = std::move(other);
        }
        
        template<typename... DimensionT>
        requires((std::is_integral_v<DimensionT> && ...) && sizeof...(DimensionT) == DimensionsCnt)
        explicit MultiDimensionalSingleArray(DimensionT... dimensionSizes) noexcept
        {
            m_dimensionsSizes = std::make_tuple(dimensionSizes...);
            
            getSize(m_size);
            
            // std::cout << "m_size : " << m_size << std::endl;
            
            m_buffer = new T[m_size];
        }
        
        ~MultiDimensionalSingleArray() noexcept
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
        
        MultiDimensionalSingleArray& operator=(const MultiDimensionalSingleArray& other) noexcept
        {
            if(this == std::addressof(other)) return *this;
            
            m_size = other.m_size;
            
            delete m_buffer;
            
            m_buffer = new T[m_size];
            
            std::memcpy(m_buffer, other.m_buffer, m_size * sizeof(T));
            
            return *this;
        }
        
        MultiDimensionalSingleArray& operator=(MultiDimensionalSingleArray&& other) noexcept
        {
            if(this == std::addressof(other)) return *this;
            
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
            else if constexpr(curIdx == DimensionsCnt - 2)
            {
                if constexpr(curIdx == 0)
                {
                    offset = std::get<curIdx>(m_dimensionsSizes);
                }
                
                offset *= indices[curIdx];
                getOffset<curIdx + 1>(indices, offset);
            }
            else
            {
                if constexpr(curIdx == 0)
                {
                    offset = 1;
                }
                offset *= std::get<curIdx>(m_dimensionsSizes);
                getOffset<curIdx + 1>(indices, offset);
            }
        }
        
        template<size_t curIdx = 0>
        void getSize(size_t& size)
        {
            if constexpr(curIdx < DimensionsCnt)
            {
                if constexpr(curIdx == 0)
                {
                    size = 1;
                }
                size *= std::get<curIdx>(m_dimensionsSizes);
                getSize<curIdx + 1>(size);
            }
        }
        
        size_t m_size = 0;
        
        T* m_buffer = nullptr;
        
        dimensions_tuple_t m_dimensionsSizes { };
    };
}

#endif // SUNGEARENGINE_MULTIDIMENSIONALSINGLEARRAY_H
