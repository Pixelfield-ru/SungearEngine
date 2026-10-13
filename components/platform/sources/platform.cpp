//
// Created by stuka on 13.10.2026.
//

#include "pixelfield/platform/platform.h"

#include "GLFW/glfw3.h"
#include "pixelfield/platform/glfw/glfw_errors.h"

pf::platform::Platform::~Platform() noexcept
{
    glfwTerminate();
}

std::error_code pf::platform::Platform::init() noexcept
{
    if(initialized_) return {};

    glfwSetErrorCallback(glfwErrorCallback);

    if(!glfwInit())
    {
        debug_output_(core::LogLevel::kError, "Cannot init GLFW library");
        return GLFWError::kCannotInitGLFW;
    }

    initialized_ = true;

    return {};
}

bool pf::platform::Platform::initialized() const noexcept
{
    return initialized_;
}

void pf::platform::Platform::setDebugOutput(core::LogCallback callback) noexcept
{
    debug_output_ = std::move(callback);
}

void pf::platform::Platform::glfwErrorCallback(int level, const char* description) noexcept
{
    platform().debug_output_(core::LogLevel::kError, description);
}

pf::platform::Platform& pf::platform::platform() noexcept
{
    static Platform instance;
    return instance;
}
