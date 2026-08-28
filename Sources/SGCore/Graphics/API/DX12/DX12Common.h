//
// Created by 8bitniksis on 18.08.2026.
//

#pragma once

#include <cstdint>
#include <source_location>
#include <string>

#include <sgcore_export.h>

#if defined(_WIN32)

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <wrl/client.h>

namespace SGCore
{
    /// COM smart pointer used throughout the DX12 backend. Same role VkHandle wrappers play for
    /// Vulkan: ownership is refcounted and released in reverse creation order automatically.
    template<typename T>
    using DX12Ptr = Microsoft::WRL::ComPtr<T>;

    /// Human-readable HRESULT for logs.
    [[nodiscard]] SGCORE_EXPORT std::string dx12ResultToString(HRESULT result) noexcept;

    /// Logs the failing expression, the HRESULT and the call site. Mirrors SG_VK_CHECK so both
    /// explicit backends report failures the same way.
    SGCORE_EXPORT bool dx12Check(HRESULT result, const char* what,
                                 const std::source_location& location = std::source_location::current()) noexcept;
}

/// Evaluates the call once and reports it when it fails. Returns true on success.
#define SG_DX_CHECK(call) SGCore::dx12Check((call), #call)

#endif
