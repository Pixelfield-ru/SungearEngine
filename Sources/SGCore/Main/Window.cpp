#include <spdlog/spdlog.h>

#include "SGCore/Graphics/API/IRenderer.h"
#include "Window.h"
#include "CoreMain.h"

#include "SGCore/Input/InputManager.h"
#include "SGCore/ImGuiWrap/ImGuiLayer.h"

void SGCore::Window::create()
{
    glfwSetErrorCallback(errorCallback);

    if(!glfwInit())
    {
        spdlog::error("Failed to initialize GLFW!\n{0}", SG_CURRENT_LOCATION_STR);
    }

    spdlog::info("-----------------------------------");
    spdlog::info("GLFW info:");
    spdlog::info("GLFW version is {0}.{1}.{2}",
                 GLFW_VERSION_MAJOR,
                 GLFW_VERSION_MINOR,
                 GLFW_VERSION_REVISION);
    spdlog::info("-----------------------------------");

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // OpenGL is the default API for GLFW, so it's not here
    if(CoreMain::getRenderer()->getGAPIType() == GAPIType::SG_API_TYPE_VULKAN)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    m_handler = glfwCreateWindow(this->m_config.m_sizeX, this->m_config.m_sizeY, this->m_config.m_title.c_str(), m_config.m_fullsreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if(!m_handler)
    {
        spdlog::error("Failed to initialize GLFW Window!\n{0}", SG_CURRENT_LOCATION_STR);
    }
    
    glfwSetWindowCloseCallback(m_handler, nativeCloseCallback);
    glfwSetWindowIconifyCallback(m_handler, nativeIconifyCallback);
    glfwSetKeyCallback(m_handler, nativeKeyboardKeyCallback);
    glfwSetMouseButtonCallback(m_handler, nativeMouseButtonCallback);
    glfwSetCursorPosCallback(m_handler, nativeMouseButtonCallback);

    makeCurrent();

    glfwSetWindowPos(m_handler, this->m_config.m_positionX, this->m_config.m_positionY);
    glfwSetWindowSizeLimits(m_handler, this->m_config.m_sizeMinLimitX, this->m_config.m_sizeMinLimitY, this->m_config.m_sizeMaxLimitX, this->m_config.m_sizeMaxLimitY);
    glfwSetWindowUserPointer(m_handler, this);

    glfwShowWindow(m_handler);

    setSwapInterval(this->m_config.m_swapInterval);

    glfwSetInputMode(m_handler, GLFW_STICKY_KEYS, this->m_config.m_enableStickyKeys);
    glfwSetInputMode(m_handler, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

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
}

void SGCore::Window::recreate()
{
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(m_handler);

    create();
}

void SGCore::Window::makeCurrent() noexcept
{
    GAPIType apiType = CoreMain::getRenderer()->getGAPIType();
    if(apiType >= GAPIType::SG_API_TYPE_GL4 && apiType <= GAPIType::SG_API_TYPE_GLES3)
    {
        glfwMakeContextCurrent(m_handler);
    }
}

#pragma region Setters

void SGCore::Window::setSize(const int& sizeX, const int& sizeY) noexcept
{
    m_config.m_sizeX = sizeX;
    m_config.m_sizeY = sizeY;

    glfwSetWindowSize(m_handler, sizeX, sizeY);
}

void SGCore::Window::setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept
{
    m_config.m_sizeMinLimitX = sizeMinLimitX;
    m_config.m_sizeMinLimitY = sizeMinLimitY;

    m_config.m_sizeMaxLimitX = sizeMaxLimitX;
    m_config.m_sizeMaxLimitY = sizeMaxLimitY;

    glfwSetWindowSizeLimits(m_handler, sizeMinLimitX, sizeMinLimitY, sizeMaxLimitX, sizeMaxLimitY);
}

void SGCore::Window::setPosition(const int& posX, const int& posY) noexcept
{
    m_config.m_positionX = posX;
    m_config.m_positionY = posY;

    glfwSetWindowPos(m_handler, posX, posY);
}

void SGCore::Window::setTitle(const std::string& title) noexcept
{
    m_config.m_title = title;

    glfwSetWindowTitle(m_handler, title.c_str());
}

void SGCore::Window::setSwapInterval(const bool& swapInterval) noexcept
{
    m_config.m_swapInterval = swapInterval;

    GAPIType apiType = CoreMain::getRenderer()->getGAPIType();
    if(apiType >= GAPIType::SG_API_TYPE_GL4 && apiType <= GAPIType::SG_API_TYPE_GLES3)
    {
        glfwSwapInterval(swapInterval);
    }
}

void SGCore::Window::setEnableStickyKeys(const bool& enableStickyKeys) noexcept
{
    m_config.m_enableStickyKeys = enableStickyKeys;

    glfwSetInputMode(m_handler, GLFW_STICKY_KEYS, enableStickyKeys);
}

void SGCore::Window::setHideAndCentralizeCursor(const bool& hideAndCentralizeCursor) noexcept
{
    m_config.m_hideAndCentralizeCursor = hideAndCentralizeCursor;

    glfwSetInputMode(m_handler, GLFW_CURSOR, hideAndCentralizeCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
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
    glfwSetWindowShouldClose(m_handler, shouldClose);
}

void SGCore::Window::setCursorPosition(const double& x, const double& y) noexcept
{
    glfwSetCursorPos(m_handler, x, y);
}

void SGCore::Window::getCursorPosition(double& posX, double& posY) noexcept
{
    glfwGetCursorPos(m_handler, &posX, &posY);
}

SGCore::KeyState SGCore::Window::getKeyboardKeyState(const SGCore::KeyboardKey& key)
{
    return (KeyState) glfwGetKey(m_handler, std::to_underlying(key));
}

SGCore::KeyState SGCore::Window::getMouseButtonState(const SGCore::MouseButton& button)
{
    return (KeyState) glfwGetMouseButton(m_handler, std::to_underlying(button));
}

#pragma endregion

#pragma region Getters

void SGCore::Window::getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept
{
    const GLFWvidmode* primaryVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    sizeX = primaryVideoMode->width;
    sizeY = primaryVideoMode->height;
}

bool SGCore::Window::shouldClose() noexcept
{
    return glfwWindowShouldClose(m_handler);
}

SGCore::WindowConfig& SGCore::Window::getConfig() noexcept
{
    return m_config;
}

void SGCore::Window::getSize(int& sizeX, int& sizeY) noexcept
{
    glfwGetWindowSize(m_handler, &sizeX, &sizeY);
}

#pragma endregion

void SGCore::Window::nativeCloseCallback(GLFWwindow* window) noexcept
{
    onClose(*((Window*) glfwGetWindowUserPointer(window)));

    spdlog::info("GLFW window closed.");
}

void SGCore::Window::nativeIconifyCallback(GLFWwindow* window, int iconified) noexcept
{
    onIconify(*((Window*) glfwGetWindowUserPointer(window)), iconified);

    spdlog::info("GLFW window iconified.");
}

void SGCore::Window::nativeKeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    onKeyboardKeyEvent(*((Window*) glfwGetWindowUserPointer(window)), (KeyboardKey) key, scancode, (KeyState) action, mods);
}

void SGCore::Window::nativeMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept
{
    onMouseButtonEvent(*((Window*) glfwGetWindowUserPointer(window)), (MouseButton) button, (KeyState) action, mods);
}

void SGCore::Window::nativeMouseButtonCallback(GLFWwindow* window, double xpos, double ypos) noexcept
{
    onCursorPositionChanged(*((Window*) glfwGetWindowUserPointer(window)), xpos, ypos);
}

void SGCore::Window::errorCallback(int errCode, const char* err_msg)
{
    spdlog::error("GLFW error (code {0}): {1}\n{2}", errCode, err_msg, SG_CURRENT_LOCATION_STR);
}

void SGCore::Window::swapBuffers()
{
    auto t0 = glfwGetTime();
    glfwSwapBuffers(m_handler);
    auto t1 = glfwGetTime();
    
    m_swapBuffersExecutionTime = (t1 - t0) * 1000.0;
}

void SGCore::Window::pollEvents()
{
    glfwPollEvents();
}

void SGCore::Window::setFullscreen(bool fullscreen) noexcept
{
    m_config.m_fullsreen = fullscreen;

    int sizeX, sizeY;
    getPrimaryMonitorSize(sizeX, sizeY);

    if(fullscreen)
    {
        glfwSetWindowMonitor(m_handler, glfwGetPrimaryMonitor(), 0, 0, sizeX, sizeY, glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(m_handler, nullptr, 0, 0, sizeX, sizeY, glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
    }
}

bool SGCore::Window::isFullscreen() const noexcept
{
    return m_config.m_fullsreen;
}

double SGCore::Window::getSwapBuffersExecutionTime() const noexcept
{
    return m_swapBuffersExecutionTime;
}

int SGCore::Window::getPrimaryMonitorRefreshRate() noexcept
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
}
