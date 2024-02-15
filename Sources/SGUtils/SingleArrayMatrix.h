//
// Created by Ilya on 15.02.2024.
//

#ifndef SUNGEARENGINE_SINGLEARRAYMATRIX_H
#define SUNGEARENGINE_SINGLEARRAYMATRIX_H

#include <cstddef>
#include <cstring>
#include <memory>

namespace SGCore
{
    template<typename T>
    struct SingleArrayMatrix
    {
        SingleArrayMatrix() = default;

        SingleArrayMatrix(const size_t& rowsCount, const size_t& columnsCount) noexcept
        {
            m_rowsCount = rowsCount;
            m_columnsCount = columnsCount;

            m_buffer = new T[m_rowsCount * m_columnsCount];
        }

        SingleArrayMatrix(const SingleArrayMatrix& other) noexcept
        {
            *this = other;
        }

        SingleArrayMatrix(SingleArrayMatrix&& other) noexcept
        {
            *this = std::move(other);
        }

        ~SingleArrayMatrix() noexcept
        {
            delete m_buffer;
            m_buffer = nullptr;
        }

        T& get(const size_t& row, const size_t& column) noexcept
        {
            return m_buffer[m_columnsCount * row + column];
        }

        SingleArrayMatrix& operator=(const SingleArrayMatrix& other) noexcept
        {
            if(this == std::addressof(other)) return *this;

            m_rowsCount = other.m_rowsCount;
            m_columnsCount = other.m_columnsCount;

            delete m_buffer;

            m_buffer = new T[m_rowsCount * m_columnsCount];

            std::memcpy(m_buffer, other.m_buffer, m_rowsCount * m_columnsCount * sizeof(T));

            return *this;
        }

        SingleArrayMatrix& operator=(SingleArrayMatrix&& other) noexcept
        {
            if (this == std::addressof(other)) return *this;

            m_rowsCount = other.m_rowsCount;
            m_columnsCount = other.m_columnsCount;

            other.m_rowsCount = 0;
            other.m_columnsCount = 0;

            m_buffer = other.m_buffer;
            other.m_buffer = nullptr;

            return *this;
        }

        T* data() const noexcept
        {
            return m_buffer;
        }

    private:
        T* m_buffer = nullptr;

        size_t m_rowsCount = 0;
        size_t m_columnsCount = 0;
    };
}

#endif //SUNGEARENGINE_SINGLEARRAYMATRIX_H
