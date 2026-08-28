//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_RHIINDEXBUFFER_H
#define SUNGEARENGINE_RHIINDEXBUFFER_H

#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "IGPUBuffer.h"

namespace SGCore
{
    /// Legacy IIndexBuffer over an RHI buffer. Indices are 32-bit, as everywhere in the engine.
    class RHIIndexBuffer : public IIndexBuffer
    {
    public:
        ~RHIIndexBuffer() noexcept override;

        void create() noexcept final;
        void create(const size_t& byteSize) noexcept final;
        void destroy() noexcept final;

        void putData(const std::vector<std::uint32_t>& data) noexcept final;
        void subData(const std::vector<std::uint32_t>& data, const int& offset) noexcept final;
        void subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept final;

        void bind() noexcept final;
        void setUsage(SGGUsage) noexcept final;

        [[nodiscard]] std::uintptr_t getNativeHandle() const noexcept override;
        [[nodiscard]] const Ref<IGPUBuffer>& getGPUBuffer() const noexcept { return m_gpuBuffer; }

    private:
        void ensureCapacity(std::uint64_t byteSize) noexcept;

        Ref<IGPUBuffer> m_gpuBuffer;
        std::uint64_t m_capacity { };
    };
}

#endif //SUNGEARENGINE_RHIINDEXBUFFER_H
