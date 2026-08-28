//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <unordered_map>
#include <vector>

#include "GL46Swapchain.h"
#include "SGCore/Graphics/RHI/IDevice.h"

namespace SGCore
{
    class GL4Renderer;
    class GL46PipelineState;

    /// RHI device of the GL 4.6 backend. Immediate execution, one frame in flight, no deferred
    /// destruction needed (GL keeps objects alive until the driver is done with them).
    class GL46Device final : public IDevice
    {
    public:
        explicit GL46Device(GL4Renderer& renderer) noexcept;

        [[nodiscard]] const DeviceProperties& getProperties() const noexcept override { return m_properties; }

        [[nodiscard]] Ref<IGPUBuffer> createBuffer(const GPUBufferDesc& desc) noexcept override;
        [[nodiscard]] Ref<IShaderProgram> createShaderProgram(const ShaderProgramDesc& desc) noexcept override;
        [[nodiscard]] Ref<IDescriptorSet> createDescriptorSet() noexcept override;
        [[nodiscard]] Ref<IPipelineState> getOrCreatePipeline(const PipelineStateDesc& desc) noexcept override;
        [[nodiscard]] Ref<ICommandList> createCommandList() noexcept override;

        void submit(const Ref<ICommandList>& commandList) noexcept override;
        [[nodiscard]] ISwapchain& getSwapchain() noexcept override { return m_swapchain; }
        void destroyDeferred(Ref<IGPUObject> object) noexcept override;
        void waitIdle() noexcept override;

    private:
        GL4Renderer& m_renderer;
        DeviceProperties m_properties;
        GL46Swapchain m_swapchain;
        std::unordered_map<std::size_t, std::vector<Ref<GL46PipelineState>>> m_pipelineCache;
    };
}
