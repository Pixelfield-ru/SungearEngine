//
// Created by stuka on 15.02.2025.
//

#ifndef UIMESH_H
#define UIMESH_H

#include "SGCore/Math/AABB.h"
#include "UIVertex.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IIndexBuffer;
    class IVertexArray;
    class IVertexBuffer;
}

namespace SGCore::UI
{
    /**
     * We are using this struct, not IMeshData, because IMeshData is too complex and big for UI elements.
     */
    struct UIElementMesh
    {
        virtual ~UIElementMesh() noexcept = default;

        virtual void prepare() noexcept;
        virtual void update() noexcept;

        AABB<> m_aabb;

        std::vector<std::uint32_t> m_indices;

        std::vector<UIVertex> m_vertices;

        [[nodiscard]] Ref<IVertexArray> getVertexArray() const noexcept;

    protected:
        Ref<IVertexArray> m_vertexArray;

        Ref<IVertexBuffer> m_verticesBuffer;

        Ref<IIndexBuffer> m_indicesBuffer;
    };
}

#endif //UIMESH_H
