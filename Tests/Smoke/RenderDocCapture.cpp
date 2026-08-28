//
// Created by 8bitniksis on 17.08.2026.
//

#include "RenderDocCapture.h"

#include <cstdint>
#include <cstdio>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace
{
    // The subset of RenderDoc's in-app API this test needs, laid out exactly as renderdoc_app.h
    // declares RENDERDOC_API_1_1_2 up to the calls we use. The ABI is versioned by
    // RENDERDOC_GetAPI(version), so the layout of a requested version never changes — that is the
    // contract the header itself relies on. Declared here instead of vendoring the whole header,
    // which would drag a third-party file into the repository for three function pointers.
    using pRENDERDOC_GetAPI = int (*)(std::uint32_t version, void** outAPIPointers);
    constexpr std::uint32_t api_version_1_1_2 = 10102;

    struct RenderDocApi_1_1_2
    {
        void* GetAPIVersion;
        void* SetCaptureOptionU32;
        void* SetCaptureOptionF32;
        void* GetCaptureOptionU32;
        void* GetCaptureOptionF32;
        void* SetFocusToggleKeys;
        void* SetCaptureKeys;
        void* GetOverlayBits;
        void* MaskOverlayBits;
        void* RemoveHooks;
        void* UnloadCrashHandler;
        void* SetCaptureFilePathTemplate;
        void* GetCaptureFilePathTemplate;
        void* GetNumCaptures;
        void* GetCapture;
        void* TriggerCapture;
        void* IsTargetControlConnected;
        void* LaunchReplayUI;
        void* SetActiveWindow;
        void (*StartFrameCapture)(void* device, void* windowHandle);
        std::uint32_t (*IsFrameCapturing)();
        std::uint32_t (*EndFrameCapture)(void* device, void* windowHandle);
    };

    RenderDocApi_1_1_2* api() noexcept
    {
        static RenderDocApi_1_1_2* s_api = []() -> RenderDocApi_1_1_2* {
#if defined(_WIN32)
            // GetModuleHandle, not LoadLibrary: capturing only works when the process was started
            // under RenderDoc, and loading the dll afterwards would not hook anything
            const HMODULE module = GetModuleHandleA("renderdoc.dll");
            if(!module) return nullptr;

            const auto getApi = reinterpret_cast<pRENDERDOC_GetAPI>(
                reinterpret_cast<void*>(GetProcAddress(module, "RENDERDOC_GetAPI")));
            if(!getApi) return nullptr;

            RenderDocApi_1_1_2* result = nullptr;
            if(getApi(api_version_1_1_2, reinterpret_cast<void**>(&result)) != 1) return nullptr;
            return result;
#else
            return nullptr;
#endif
        }();
        return s_api;
    }
}

bool SGSmoke::RenderDocCapture::isAvailable() noexcept
{
    return api() != nullptr;
}

void SGSmoke::RenderDocCapture::beginFrame() noexcept
{
    auto* renderDoc = api();
    if(!renderDoc || !renderDoc->StartFrameCapture) return;

    // null device/window: capture whatever the frame touches, which is what a single-window test wants
    renderDoc->StartFrameCapture(nullptr, nullptr);
    std::printf("RenderDoc: frame capture started\n");
}

void SGSmoke::RenderDocCapture::endFrame() noexcept
{
    auto* renderDoc = api();
    if(!renderDoc || !renderDoc->EndFrameCapture) return;

    const bool captured = renderDoc->EndFrameCapture(nullptr, nullptr) == 1;
    std::printf("RenderDoc: frame capture %s\n", captured ? "written" : "FAILED");
}
