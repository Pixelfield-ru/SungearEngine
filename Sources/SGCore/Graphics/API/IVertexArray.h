//
// Created by stuka on 24.04.2023.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "IVertexBuffer.h"
#include <sgcore_export.h>

namespace SGCore
{
    class IVertexBuffer;
    class IIndexBuffer;

    class SGCORE_EXPORT IVertexArray
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

        /**
         * Records the attributes a buffer contributes TO THIS ARRAY when they differ from the layout
         * the buffer itself has recorded. The mesh buffers are bound into more than one array —
         * their own (locations from 0) and the one Instancing builds (the same buffers behind the
         * per-instance transform, locations from 7) — and a layout is a property of that pairing,
         * not of the buffer. GL keeps it in the vertex array object; the explicit backends have to
         * rebuild it per draw, and without this they would read whichever layout was recorded last.
         */
        void setBufferAttributes(const IVertexBuffer* vertexBuffer,
                                 std::vector<IVertexBuffer::AttributeDesc> attributes) noexcept;

        /// Attributes recorded for this pairing, or nullptr — then the buffer's own layout applies.
        [[nodiscard]] const std::vector<IVertexBuffer::AttributeDesc>*
        getBufferAttributes(const IVertexBuffer* vertexBuffer) const noexcept;

    private:
        std::unordered_set<IVertexBuffer*> m_vertexBuffers;
        IIndexBuffer* m_indexBuffer {};
        std::unordered_map<const IVertexBuffer*, std::vector<IVertexBuffer::AttributeDesc>> m_bufferAttributes;
    };
}
