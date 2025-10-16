#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "SGCore/Graphics/API/IRenderer.h"
#include "Window.h"
#include "CoreMain.h"

#include "SGCore/Input/PCInput.h"

void SGCore::Window::create()
{
#ifdef SG_PLATFORM_PC
    glfwSetErrorCallback(errorCallback);

    if(!glfwInit())
    {
        LOG_E(SGCORE_TAG, "Failed to initialize GLFW!\n{0}", SG_CURRENT_LOCATION_STR);
    }

    LOG_I(SGCORE_TAG, "-----------------------------------");
    LOG_I(SGCORE_TAG, "GLFW info:");
    LOG_I(SGCORE_TAG,
          "GLFW version is {}.{}.{}",
          GLFW_VERSION_MAJOR,
          GLFW_VERSION_MINOR,
          GLFW_VERSION_REVISION);
    LOG_I(SGCORE_TAG, "-----------------------------------");

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // OpenGL is the default API for GLFW, so it's not here
    if(CoreMain::getRenderer()->getGAPIType() == GAPIType::SG_API_TYPE_VULKAN)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    m_handle = glfwCreateWindow(this->m_config.m_sizeX, this->m_config.m_sizeY, this->m_config.m_title.c_str(), m_config.m_fullsreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if(!m_handle)
    {
        LOG_E(SGCORE_TAG, "Failed to initialize GLFW Window!\n{}", SG_CURRENT_LOCATION_STR);
        return;
    }
    
    glfwSetWindowCloseCallback(m_handle, nativeCloseCallback);
    glfwSetWindowIconifyCallback(m_handle, nativeIconifyCallback);
    glfwSetFramebufferSizeCallback(m_handle, nativeFramebufferSizeCallback);

    makeCurrent();

    glfwSetWindowPos(m_handle, this->m_config.m_positionX, this->m_config.m_positionY);
    glfwSetWindowSizeLimits(m_handle, this->m_config.m_sizeMinLimitX, this->m_config.m_sizeMinLimitY, this->m_config.m_sizeMaxLimitX, this->m_config.m_sizeMaxLimitY);
    glfwSetWindowUserPointer(m_handle, this);

    glfwShowWindow(m_handle);

    setSwapInterval(this->m_config.m_swapInterval);

    glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, this->m_config.m_enableStickyKeys);
    glfwSetInputMode(m_handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    // ------------- post create
    const GLFWvidmode* primaryVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    if(m_config.m_useHalfMonitor)
    {
        setSize(primaryVideoMode->width / 2, primaryVideoMode->height / 2);
    }

    if(m_config.m_centralizeWindow)
    {
        int wndSizeX = 0;
        int wndSizeY = 0;

        getSize(wndSizeX, wndSizeY);
        setPosition((primaryVideoMode->width - wndSizeX) / 2, (primaryVideoMode->height - wndSizeY) / 2);
    }
    // -------------------------
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::recreate()
{
#ifdef SG_PLATFORM_PC
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(m_handle);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif

    create();
}

void SGCore::Window::makeCurrent() noexcept
{
    Input::PC::setupInput(*this);

    GAPIType apiType = CoreMain::getRenderer()->getGAPIType();
    if(apiType >= GAPIType::SG_API_TYPE_GL4 && apiType <= GAPIType::SG_API_TYPE_GLES3)
    {
#ifdef SG_PLATFORM_PC
        glfwMakeContextCurrent(m_handle);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
    }
}

#pragma region Setters

void SGCore::Window::setSize(const int& sizeX, const int& sizeY) noexcept
{
    m_config.m_sizeX = sizeX;
    m_config.m_sizeY = sizeY;

#ifdef SG_PLATFORM_PC
    glfwSetWindowSize(m_handle, sizeX, sizeY);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept
{
    m_config.m_sizeMinLimitX = sizeMinLimitX;
    m_config.m_sizeMinLimitY = sizeMinLimitY;

    m_config.m_sizeMaxLimitX = sizeMaxLimitX;
    m_config.m_sizeMaxLimitY = sizeMaxLimitY;

#ifdef SG_PLATFORM_PC
    glfwSetWindowSizeLimits(m_handle, sizeMinLimitX, sizeMinLimitY, sizeMaxLimitX, sizeMaxLimitY);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::setPosition(const int& posX, const int& posY) noexcept
{
    m_config.m_positionX = posX;
    m_config.m_positionY = posY;

#ifdef SG_PLATFORM_PC
    glfwSetWindowPos(m_handle, posX, posY);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::setTitle(const std::string& title) noexcept
{
    m_config.m_title = title;

#ifdef SG_PLATFORM_PC
    glfwSetWindowTitle(m_handle, title.c_str());
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::setSwapInterval(const bool& swapInterval) noexcept
{
    m_config.m_swapInterval = swapInterval;

    GAPIType apiType = CoreMain::getRenderer()->getGAPIType();
    if(apiType >= GAPIType::SG_API_TYPE_GL4 && apiType <= GAPIType::SG_API_TYPE_GLES3)
    {
#ifdef SG_PLATFORM_PC
        glfwSwapInterval(swapInterval);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
    }
}

void SGCore::Window::setEnableStickyKeys(const bool& enableStickyKeys) noexcept
{
    m_config.m_enableStickyKeys = enableStickyKeys;

#ifdef SG_PLATFORM_PC
    glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, enableStickyKeys);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::setHideAndCentralizeCursor(const bool& hideAndCentralizeCursor) noexcept
{
    m_config.m_hideAndCentralizeCursor = hideAndCentralizeCursor;

#ifdef SG_PLATFORM_PC
    glfwSetInputMode(m_handle, GLFW_CURSOR, hideAndCentralizeCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

bool SGCore::Window::isHideAndCentralizeCursor() noexcept
{
    return m_config.m_hideAndCentralizeCursor;
}

void SGCore::Window::setConfig(WindowConfig&& other) noexcept
{
    m_config = std::move(other);
}

void SGCore::Window::setShouldClose(const bool& shouldClose) noexcept
{
#ifdef SG_PLATFORM_PC
    glfwSetWindowShouldClose(m_handle, shouldClose);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

#pragma endregion

#pragma region Getters

void SGCore::Window::getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept
{
#ifdef SG_PLATFORM_PC
    const GLFWvidmode* primaryVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    sizeX = primaryVideoMode->width;
    sizeY = primaryVideoMode->height;
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

bool SGCore::Window::shouldClose() noexcept
{
#ifdef SG_PLATFORM_PC
    return glfwWindowShouldClose(m_handle);
#elif defined(SG_PLATFORM_OS_ANDROID)
    return false;
#endif
}

SGCore::WindowConfig& SGCore::Window::getConfig() noexcept
{
    return m_config;
}

void SGCore::Window::getSize(int& sizeX, int& sizeY) noexcept
{
#ifdef SG_PLATFORM_PC
    glfwGetWindowSize(m_handle, &sizeX, &sizeY);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::getBordersSize(int& left, int& top, int& right, int& bottom) noexcept
{
#ifdef SG_PLATFORM_PC
    glfwGetWindowFrameSize(m_handle, &left, &top, &right, &bottom);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::getSizeWithBorders(int& sizeX, int& sizeY) noexcept
{
    getSize(sizeX, sizeY);

    int leftBorder;
    int topBorder;
    int rightBorder;
    int bottomBorder;

    getBordersSize(leftBorder, topBorder, rightBorder, bottomBorder);

    sizeX = sizeX + leftBorder + rightBorder;
    sizeY = sizeY + topBorder + bottomBorder;
}

#pragma endregion

void SGCore::Window::nativeCloseCallback(window_handle window) noexcept
{
#ifdef SG_PLATFORM_PC
    onClose(*((Window*) glfwGetWindowUserPointer(window)));
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif

    LOG_I(SGCORE_TAG, "Application window closed.");
}

void SGCore::Window::nativeIconifyCallback(window_handle window, int iconified) noexcept
{
#ifdef SG_PLATFORM_PC
    onIconify(*((Window*) glfwGetWindowUserPointer(window)), iconified);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif

    LOG_I(SGCORE_TAG, "Application window iconified.");
}

void SGCore::Window::nativeFramebufferSizeCallback(window_handle window, int width, int height) noexcept
{
#ifdef SG_PLATFORM_PC
    onFrameBufferSizeChanged(*((Window*) glfwGetWindowUserPointer(window)), width, height);
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::errorCallback(int errCode, const char* err_msg)
{
    LOG_E(SGCORE_TAG, "GLFW error (code {}): {}\n{}", errCode, err_msg, SG_CURRENT_LOCATION_STR);
}

void SGCore::Window::swapBuffers()
{
#ifdef SG_PLATFORM_PC
    auto t0 = glfwGetTime();
    glfwSwapBuffers(m_handle);
    auto t1 = glfwGetTime();

    
    m_swapBuffersExecutionTime = (t1 - t0) * 1000.0;
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::pollEvents()
{
#ifdef SG_PLATFORM_PC
    glfwPollEvents();
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

void SGCore::Window::setFullscreen(bool fullscreen) noexcept
{
    m_config.m_fullsreen = fullscreen;

    int sizeX, sizeY;
    getPrimaryMonitorSize(sizeX, sizeY);

#ifdef SG_PLATFORM_PC
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_handle, glfwGetPrimaryMonitor(), 0, 0, sizeX, sizeY, glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(m_handle, nullptr, 0, 0, sizeX, sizeY, glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
    }
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

bool SGCore::Window::isFullscreen() const noexcept
{
    return m_config.m_fullsreen;
}

double SGCore::Window::getSwapBuffersExecutionTime() const noexcept
{
    return m_swapBuffersExecutionTime;
}

SGCore::Window::window_handle SGCore::Window::getNativeHandle() noexcept
{
    return m_handle;
}

int SGCore::Window::getPrimaryMonitorRefreshRate() noexcept
{
#ifdef SG_PLATFORM_PC
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
#elif defined(SG_PLATFORM_OS_ANDROID)
    return 0;
#endif
}
