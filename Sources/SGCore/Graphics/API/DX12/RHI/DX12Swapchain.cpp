//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12Swapchain.h"

#if defined(_WIN32)

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "SGCore/Logger/Logger.h"
#include "DX12Device.h"

SGCore::DX12Swapchain::DX12Swapchain(DX12Device& device, GLFWwindow* window) noexcept : m_device(device), m_window(window)
{
    m_debugName = "swapchain";
    m_hwnd = window ? glfwGetWin32Window(window) : nullptr;
    if(!m_hwnd)
    {
        SG_LOG_E("DX12Swapchain: the window has no HWND, the swapchain can not be created.");
        return;
    }

    recreate();
}

SGCore::DX12Swapchain::~DX12Swapchain()
{
    // the GPU may still be reading the back buffers
    m_device.waitIdle();
    releaseImages();
    m_swapchain.Reset();
}

void SGCore::DX12Swapchain::releaseImages() noexcept
{
    m_images.clear();
    m_lastSubmissions.clear();
}

bool SGCore::DX12Swapchain::queryWindowSize(std::uint32_t& outWidth, std::uint32_t& outHeight) const noexcept
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    if(width <= 0 || height <= 0) return false;
    outWidth = static_cast<std::uint32_t>(width);
    outHeight = static_cast<std::uint32_t>(height);
    return true;
}

bool SGCore::DX12Swapchain::recreate() noexcept
{
    auto& context = m_device.getContext();
    m_needsRecreate = false;
    m_acquired = false;

    std::uint32_t width = 0;
    std::uint32_t height = 0;
    if(!queryWindowSize(width, height))
    {
        // minimized: keep what we have and try again next frame
        m_needsRecreate = true;
        return false;
    }

    // everything recorded against the old back buffers has to be done before they are released
    m_device.waitIdle();
    releaseImages();

    if(!m_swapchain)
    {
        DXGI_SWAP_CHAIN_DESC1 desc { };
        desc.Width = width;
        desc.Height = height;
        // UNORM, not sRGB: the engine writes display-ready values, as it does on GL and Vulkan
        desc.Format = swapchain_format;
        desc.SampleDesc.Count = 1;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        // the buffer count is the frames in flight: a back buffer is reused only after the frame
        // that presented it has finished (see ensureAcquired())
        desc.BufferCount = frames_in_flight;
        desc.Scaling = DXGI_SCALING_NONE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        DX12Ptr<IDXGISwapChain1> swapchain1;
        if(!SG_DX_CHECK(context.m_factory->CreateSwapChainForHwnd(context.m_directQueue.Get(), m_hwnd, &desc,
                                                                 nullptr, nullptr, &swapchain1)))
        {
            return false;
        }
        // alt+enter fullscreen behind our back would resize the swapchain without telling us
        SG_DX_CHECK(context.m_factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER));
        if(!SG_DX_CHECK(swapchain1.As(&m_swapchain))) return false;
    }
    else if(!SG_DX_CHECK(m_swapchain->ResizeBuffers(frames_in_flight, width, height, swapchain_format, 0)))
    {
        return false;
    }

    m_width = width;
    m_height = height;

    for(std::uint32_t i = 0; i < frames_in_flight; ++i)
    {
        DX12Ptr<ID3D12Resource> buffer;
        if(!SG_DX_CHECK(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer))))
        {
            releaseImages();
            return false;
        }
        // a fresh back buffer is in PRESENT state
        m_images.push_back(DX12Texture::wrap(m_device.getContextRef(), buffer.Get(), swapchain_format, width, height,
                                             D3D12_RESOURCE_STATE_PRESENT, "swapchain_image_" + std::to_string(i)));
        m_lastSubmissions.push_back(0);
    }

    m_bufferIndex = m_swapchain->GetCurrentBackBufferIndex();

    SG_LOG_I("DX12: swapchain {}x{} with {} buffers (flip discard).", m_width, m_height, frames_in_flight);
    return true;
}

bool SGCore::DX12Swapchain::beginFrame() noexcept
{
    m_device.retire(false);

    std::uint32_t width = 0;
    std::uint32_t height = 0;
    if(queryWindowSize(width, height) && (width != m_width || height != m_height))
    {
        m_needsRecreate = true;
    }
    if(m_needsRecreate || !m_swapchain)
    {
        recreate();
    }
    return isValid();
}

bool SGCore::DX12Swapchain::ensureAcquired() noexcept
{
    if(m_acquired) return true;
    if(m_needsRecreate || !isValid())
    {
        if(!recreate()) return false;
    }

    m_bufferIndex = m_swapchain->GetCurrentBackBufferIndex();
    if(m_bufferIndex >= m_images.size()) return false;

    // bound the frames in flight: this buffer is written again only after the frame that presented
    // it has completed
    m_device.waitForSubmission(m_lastSubmissions[m_bufferIndex]);

    m_acquired = true;
    return true;
}

SGCore::Ref<SGCore::DX12Texture> SGCore::DX12Swapchain::getCurrentTexture() const noexcept
{
    if(!m_acquired || m_bufferIndex >= m_images.size()) return nullptr;
    return m_images[m_bufferIndex];
}

void SGCore::DX12Swapchain::present() noexcept
{
    ++m_frameNumber;
    if(!m_acquired)
    {
        // nothing was rendered into the window this frame
        return;
    }

    auto image = m_images[m_bufferIndex];

    // the presentation engine requires the PRESENT state, and only a command list can transition
    auto commandContext = m_device.acquireCommandContext();
    if(commandContext.m_list)
    {
        image->recordTransition(commandContext.m_list.Get(), D3D12_RESOURCE_STATE_PRESENT);
        if(SG_DX_CHECK(commandContext.m_list->Close()))
        {
            DX12Submission submission;
            submission.m_contexts.push_back(std::move(commandContext));
            submission.m_keepAlive.push_back(image);
            m_device.submitRaw(std::move(submission));
        }
    }

    // vsync, the counterpart of Vulkan FIFO
    const HRESULT result = m_swapchain->Present(1, 0);
    if(result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
    {
        SG_LOG_C("DX12: the device was removed while presenting ({}).", dx12ResultToString(result));
        m_needsRecreate = true;
    }
    else
    {
        SG_DX_CHECK(result);
    }

    // The present itself is queue work, and the fence signalled before it says nothing about it.
    // Signalling again here is what makes "wait for this buffer" mean "the presentation engine is
    // done with it" — without it the swapchain could be resized or destroyed with a present still
    // in flight, which the debug layer turns into a fatal exception at release time.
    m_lastSubmissions[m_bufferIndex] = m_device.submitRaw({ });

    m_acquired = false;
}

#endif
