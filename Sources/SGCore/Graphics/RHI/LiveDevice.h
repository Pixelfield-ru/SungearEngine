//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#include <sgcore_export.h>

namespace SGCore
{
    class IDevice;

    /// The RHI device of the running backend, or nullptr before init() and after shutdown().
    ///
    /// Legacy facades need the device from destructors that run in static destruction, when the
    /// renderer singleton may already be gone — reaching it through IRenderer there reads a
    /// destroyed object (the abort() at exit the Vulkan backend was debugged out of). Each backend
    /// publishes its device here while it lives, and the backend-neutral facades use only this.
    struct SGCORE_EXPORT LiveDevice
    {
        static void set(IDevice* device) noexcept;
        [[nodiscard]] static IDevice* get() noexcept;

    private:
        static inline IDevice* s_device = nullptr;
    };
}
