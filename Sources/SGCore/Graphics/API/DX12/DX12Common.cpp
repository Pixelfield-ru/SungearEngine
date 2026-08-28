//
// Created by 8bitniksis on 18.08.2026.
//

#include "DX12Common.h"

#if defined(_WIN32)

#include <comdef.h>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/Utils.h"

std::string SGCore::dx12ResultToString(HRESULT result) noexcept
{
    // the few codes worth naming outright; the rest go through the system message table, which is
    // more informative than a hex number and does not need a hand-written table to stay current
    switch(result)
    {
        case S_OK: return "S_OK";
        case E_OUTOFMEMORY: return "E_OUTOFMEMORY";
        case E_INVALIDARG: return "E_INVALIDARG";
        case E_NOINTERFACE: return "E_NOINTERFACE";
        case E_NOTIMPL: return "E_NOTIMPL";
        case DXGI_ERROR_DEVICE_REMOVED: return "DXGI_ERROR_DEVICE_REMOVED";
        case DXGI_ERROR_DEVICE_HUNG: return "DXGI_ERROR_DEVICE_HUNG";
        case DXGI_ERROR_DEVICE_RESET: return "DXGI_ERROR_DEVICE_RESET";
        case DXGI_ERROR_NOT_FOUND: return "DXGI_ERROR_NOT_FOUND";
        case DXGI_ERROR_UNSUPPORTED: return "DXGI_ERROR_UNSUPPORTED";
        case DXGI_ERROR_SDK_COMPONENT_MISSING: return "DXGI_ERROR_SDK_COMPONENT_MISSING";
        default: break;
    }

    const _com_error error(result);
    return fmt::format("0x{:08X} ({})", static_cast<std::uint32_t>(result), Utils::toUTF8(error.ErrorMessage()));
}

bool SGCore::dx12Check(HRESULT result, const char* what, const std::source_location& location) noexcept
{
    if(SUCCEEDED(result)) return true;

    SG_LOG_E("DX12 call '{}' failed with {}.\n{}", what, dx12ResultToString(result),
             Utils::sourceLocationToString(location));
    return false;
}

#endif
