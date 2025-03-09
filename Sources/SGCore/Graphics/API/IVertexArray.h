//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IVERTEXARRAY_H
#define NATIVECORE_IVERTEXARRAY_H

#include <SGCore/pch.h>

namespace SGCore
{
    class IVertexArray : public std::enable_shared_from_this<IVertexArray>
    {
    public:
        std::uint64_t m_indicesCount = 0;

        virtual ~IVertexArray() = default;

        virtual std::shared_ptr<IVertexArray> create() = 0;
        virtual void destroy() = 0;

        virtual void bind() = 0;
    };
}

#endif //NATIVECORE_IVERTEXARRAY_H
