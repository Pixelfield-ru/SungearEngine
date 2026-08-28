//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <sgcore_export.h>

#include "IGPUObject.h"
#include "RHITypes.h"

namespace SGCore
{
    /// A GPU buffer of any usage (vertex, index, uniform, storage, staging). Replaces the
    /// separate IVertexBuffer / IIndexBuffer / IUniformBuffer of the legacy API.
    class SGCORE_EXPORT IGPUBuffer : public IGPUObject
    {
    public:
        [[nodiscard]] const GPUBufferDesc& getDesc() const noexcept { return m_desc; }

        /**
         * Maps host-visible memory for writing. Not allowed for SGG_DEVICE_LOCAL buffers — use
         * ICommandList::uploadData for those.
         * @return Pointer to the mapped range or nullptr if the buffer can not be mapped.
         */
        [[nodiscard]] virtual void* map(std::uint64_t offset, std::uint64_t size) noexcept = 0;
        virtual void unmap() noexcept = 0;

        /// Convenience for host-visible buffers: map, copy, unmap.
        virtual bool write(const void* data, std::uint64_t size, std::uint64_t offset = 0) noexcept = 0;

    protected:
        GPUBufferDesc m_desc;
    };
}
