//
// Created by stuka on 02.10.2026.
//

#pragma once

#include <system_error>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;

    struct BufferCreateInfo final
    {
        const IContext* context_ {};
        ILogicalDevice* logical_device_ {};

        bool host_coherent = false;

        BufferType type = BufferType::kVertex;
        size_t size {};
        void* data {};
    };

    struct IBuffer
    {
        virtual ~IBuffer() = default;

        virtual bool valid() const noexcept = 0;

        virtual std::error_code update(void* data, size_t offset, size_t size) = 0;
        virtual size_t size() = 0;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const BufferCreateInfo& create_info) = 0;
    };
}
