//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <string>
#include <sgcore_export.h>

namespace SGCore
{
    /// Base of every RHI object: a debug name and virtual destruction. Lifetime is owned by
    /// Ref<>; explicit APIs defer the actual GPU release until the frames using the object
    /// have completed (IDevice::destroyDeferred).
    class SGCORE_EXPORT IGPUObject
    {
    public:
        virtual ~IGPUObject() = default;

        [[nodiscard]] const std::string& getDebugName() const noexcept { return m_debugName; }
        void setDebugName(std::string name) noexcept { m_debugName = std::move(name); }

    protected:
        std::string m_debugName;
    };
}
