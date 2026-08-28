//
// Created by 8bitniksis on 17.08.2026.
//

#include "GL46Swapchain.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Main/Window.h"

bool SGCore::GL46Swapchain::beginFrame() noexcept
{
    return true;
}

void SGCore::GL46Swapchain::present() noexcept
{
    CoreMain::getWindow().swapBuffers();
    ++m_frameNumber;
}

std::uint32_t SGCore::GL46Swapchain::getWidth() const noexcept
{
    int width = 0, height = 0;
    CoreMain::getWindow().getSize(width, height);
    return static_cast<std::uint32_t>(width);
}

std::uint32_t SGCore::GL46Swapchain::getHeight() const noexcept
{
    int width = 0, height = 0;
    CoreMain::getWindow().getSize(width, height);
    return static_cast<std::uint32_t>(height);
}
