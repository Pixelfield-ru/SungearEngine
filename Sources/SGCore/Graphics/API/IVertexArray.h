//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IVERTEXARRAY_H
#define NATIVECORE_IVERTEXARRAY_H

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace SGCore
{
    class IVertexBuffer;
    class IIndexBuffer;

    class IVertexArray
    {
        friend class IVertexBuffer;
        friend class IIndexBuffer;

    public:
        std::uint64_t m_indicesCount = 0;

        virtual ~IVertexArray();

        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual void bind() = 0;

        void addVertexBuffer(IVertexBuffer* vertexBuffer) noexcept;
        void setIndexBuffer(IIndexBuffer* indexBuffer) noexcept;

        const std::unordered_set<IVertexBuffer*>& getVertexBuffers() noexcept;
        IIndexBuffer* getIndexBuffer() noexcept;

    private:
        std::unordered_set<IVertexBuffer*> m_vertexBuffers;
        IIndexBuffer* m_indexBuffer {};
    };
}

#endif //NATIVECORE_IVERTEXARRAY_H
