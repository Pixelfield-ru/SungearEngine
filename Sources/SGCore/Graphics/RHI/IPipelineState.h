//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstddef>
#include <sgcore_export.h>

#include "IGPUObject.h"
#include "IShaderProgram.h"
#include "RHITypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    /**
     * Everything that is immutable pipeline state on Vulkan/DX12: shader program, fixed-function
     * states (the existing RenderState / BlendingState / MeshRenderState structs are reused as
     * they are), vertex input layout and render target formats. Equal descs → same pipeline
     * object (IDevice::getOrCreatePipeline caches by hash of the desc).
     */
    struct PipelineStateDesc
    {
        Ref<IShaderProgram> m_program;

        RenderState m_renderState { };
        BlendingState m_blendingState { };
        MeshRenderState m_meshRenderState { };

        VertexInputDesc m_vertexInput { };
        RenderTargetsDesc m_renderTargets { };

        std::string m_debugName;

        [[nodiscard]] bool equalsForCache(const PipelineStateDesc& other) const noexcept
        {
            return m_program == other.m_program &&
                   m_renderState == other.m_renderState &&
                   m_blendingState == other.m_blendingState &&
                   m_meshRenderState == other.m_meshRenderState &&
                   m_vertexInput == other.m_vertexInput &&
                   m_renderTargets == other.m_renderTargets;
        }

        [[nodiscard]] std::size_t hash() const noexcept;
    };

    class SGCORE_EXPORT IPipelineState : public IGPUObject
    {
    public:
        [[nodiscard]] const PipelineStateDesc& getDesc() const noexcept { return m_desc; }

    protected:
        PipelineStateDesc m_desc;
    };
}
