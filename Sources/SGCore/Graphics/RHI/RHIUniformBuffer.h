//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_RHIUNIFORMBUFFER_H
#define SUNGEARENGINE_RHIUNIFORMBUFFER_H

#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "IGPUBuffer.h"

namespace SGCore
{
    /// Legacy IUniformBuffer over a host-visible RHI buffer. The engine's shared blocks
    /// (CameraData, ProgramDataBlock, ...) keep their CPU-side mirror in IUniformBuffer and this
    /// class only pushes the changed range to the GPU.
    ///
    /// setLayoutLocation() is remembered but unused: the shaders declare these blocks without an
    /// explicit binding, so the vulkanizer picks the binding and shaders find the buffer
    /// by block name through SharedUniformBuffers.
    class RHIUniformBuffer : public IUniformBuffer
    {
    public:
        ~RHIUniformBuffer() override;

        void bind() noexcept final;
        void prepare() noexcept final;
        void setLayoutLocation(const std::uint16_t& location) noexcept final;
        void destroy() noexcept final;

        [[nodiscard]] const Ref<IGPUBuffer>& getBuffer() const noexcept { return m_buffer_gpu; }

    private:
        void subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept final;

        Ref<IGPUBuffer> m_buffer_gpu;
    };
}

#endif //SUNGEARENGINE_RHIUNIFORMBUFFER_H
