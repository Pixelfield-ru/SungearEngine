//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <sgcore_export.h>

#include "ICommandList.h"
#include "IDescriptorSet.h"
#include "IGPUBuffer.h"
#include "IPipelineState.h"
#include "IShaderProgram.h"
#include "ISwapchain.h"
#include "RHITypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    /// A slice of the per-frame uniform arena, ready to be filled and bound at a single offset.
    struct UniformSlice
    {
        Ref<IGPUBuffer> m_buffer;
        std::uint64_t m_offset { };
        /// Host-visible memory of exactly the requested size; nullptr when the allocation failed.
        void* m_mapped { };
    };

    /**
     * Root of the render hardware interface: creates GPU objects, owns the pipeline cache and
     * the swapchain, reports backend properties. One IDevice per process; obtained through
     * IRenderer::getDevice() during the migration (docs/RHI_DESIGN.md).
     */
    class SGCORE_EXPORT IDevice
    {
    public:
        virtual ~IDevice() = default;

        [[nodiscard]] virtual const DeviceProperties& getProperties() const noexcept = 0;

        [[nodiscard]] virtual Ref<IGPUBuffer> createBuffer(const GPUBufferDesc& desc) noexcept = 0;
        [[nodiscard]] virtual Ref<IShaderProgram> createShaderProgram(const ShaderProgramDesc& desc) noexcept = 0;
        [[nodiscard]] virtual Ref<IDescriptorSet> createDescriptorSet() noexcept = 0;

        /// Pipeline cache: an equal desc returns the same object.
        [[nodiscard]] virtual Ref<IPipelineState> getOrCreatePipeline(const PipelineStateDesc& desc) noexcept = 0;

        [[nodiscard]] virtual Ref<ICommandList> createCommandList() noexcept = 0;

        /// Executes a recorded command list. Explicit backends queue it for the frame's submit;
        /// GL has already executed it during recording and returns immediately.
        virtual void submit(const Ref<ICommandList>& commandList) noexcept = 0;

        [[nodiscard]] virtual ISwapchain& getSwapchain() noexcept = 0;

        /**
         * Reserves uniform memory for the draw being recorded.
         *
         * Commands run long after they are recorded, so values that differ per draw can not live in
         * one buffer written in place: the last write would be what every draw of the pass reads.
         * Each draw copies its values into its own slice instead and binds that offset. Immediate
         * backends (GL) need none of this and return an empty slice.
         */
        [[nodiscard]] virtual UniformSlice allocateTransientUniforms(std::uint64_t /*size*/) noexcept { return { }; }

        /// A 1x1 white texture of the backend's own type, for descriptors nothing was bound to.
        /// Empty on backends that do not need every declared descriptor written (GL).
        [[nodiscard]] virtual Ref<IGPUObject> getDummyBackendTexture() noexcept { return nullptr; }

        /// Releases the object once every frame that could still use it has completed.
        virtual void destroyDeferred(Ref<IGPUObject> object) noexcept = 0;

        /// Blocks until the GPU finished all submitted work (shutdown, resource re-creation).
        virtual void waitIdle() noexcept = 0;
    };
}
