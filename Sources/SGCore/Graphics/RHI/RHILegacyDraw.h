//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#include <sgcore_export.h>

#include "IDevice.h"
#include "RHILegacyShader.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IMeshData;
    class IVertexArray;

    /**
     * The legacy drawing paths (IRenderer::renderMeshData / renderArray) expressed in RHI calls,
     * shared by every explicit backend: what differs between them is the command list and the
     * "currently bound shader", both of which are handed in.
     *
     * The pipeline is built per draw from the bound program, the cached fixed-function state and the
     * mesh layout, and looked up in the device's pipeline cache — the same shape GL46Renderer uses.
     */
    struct SGCORE_EXPORT RHILegacyDraw
    {
        /// Builds the mesh's RHI buffers and vertex layout once (IMeshData::m_rhi).
        static bool prepareMesh(IDevice& device, IMeshData& meshData) noexcept;

        static void drawMesh(IDevice& device, ICommandList& commandList, RHILegacyShader& shader,
                             const RenderState& renderState, const IMeshData& meshData,
                             const MeshRenderState& meshRenderState) noexcept;

        static void drawArray(IDevice& device, ICommandList& commandList, RHILegacyShader& shader,
                              const RenderState& renderState, const Ref<IVertexArray>& vertexArray,
                              const MeshRenderState& meshRenderState,
                              int verticesCount, int indicesCount, int instancesCount) noexcept;
    };
}
