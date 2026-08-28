//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_RHIVERTEXBUFFER_H
#define SUNGEARENGINE_RHIVERTEXBUFFER_H

#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "IGPUBuffer.h"

namespace SGCore
{
    /// Legacy IVertexBuffer over an RHI buffer, shared by every explicit backend. Attributes are only
    /// recorded by the base class
    /// (getAttributes()); the vertex layout belongs to the pipeline there, so useAttributes()
    /// and bind() have nothing to do — the renderer builds VertexInputDesc from those records.
    class RHIVertexBuffer : public IVertexBuffer
    {
    public:
        ~RHIVertexBuffer() noexcept override;

        void create() noexcept final;
        void create(const size_t& byteSize) noexcept final;
        void destroy() noexcept final;

        void bind() noexcept final;
        void setUsage(SGGUsage) noexcept final;

        void addAttributeImpl(std::uint32_t location,
                              std::int32_t scalarsCount,
                              SGGDataType dataType,
                              bool isNormalized,
                              std::int32_t stride,
                              std::uint64_t offsetInStruct,
                              std::int32_t divisor) noexcept final;

        void useAttributes() const noexcept final;

        [[nodiscard]] std::uintptr_t getNativeHandle() const noexcept override;
        [[nodiscard]] const Ref<IGPUBuffer>& getGPUBuffer() const noexcept { return m_gpuBuffer; }

    protected:
        void subDataOnGAPISide(const void* data, const size_t& bytesCount, const size_t& bytesOffset, bool isPutData) noexcept override;

    private:
        /// (Re)creates the RHI buffer when it is missing or too small for byteSize.
        void ensureCapacity(std::uint64_t byteSize) noexcept;

        Ref<IGPUBuffer> m_gpuBuffer;
        std::uint64_t m_capacity { };
    };
}

#endif //SUNGEARENGINE_RHIVERTEXBUFFER_H
