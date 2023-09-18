#include "SGCore/Graphics/API/IRenderer.h"
#include "Window.h"
#include "CoreMain.h"

void Core::Main::Window::create()
{
    glfwSetErrorCallback(errorCallback);

    if(!glfwInit())
    {
        SGC_ERROR("Failed to initialize GLFW!");
    }

    SGC_INFO("-----------------------------------");
    SGC_INFO("GLFW info:");
    SGC_INFO("GLFW version is " + std::to_string(GLFW_VERSION_MAJOR) + "." +
             std::to_string(GLFW_VERSION_MINOR) + "." +
             std::to_string(GLFW_VERSION_REVISION));
    SGC_INFO("-----------------------------------");

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // OpenGL is the default API for GLFW, so it's not here
    if(CoreMain::getRenderer().getAPIType() == Graphics::SG_API_TYPE_VULKAN)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    m_handler = glfwCreateWindow(this->m_config->m_sizeX, this->m_config->m_sizeY, this->m_config->m_title.c_str(), nullptr, nullptr);

    if(!m_handler)
    {
        SGC_ERROR("Failed to initialize GLFW Window!");
    }

    glfwSetWindowCloseCallback(m_handler, windowCloseCallback);
    glfwSetWindowIconifyCallback(m_handler, windowIconifyCallback);
    glfwSetKeyCallback(m_handler, InputManager::keyboardKeyCallback);
    glfwSetMouseButtonCallback(m_handler, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(m_handler, InputManager::cursorPositionCallback);

    makeCurrent();

    glfwSetWindowPos(m_handler, this->m_config->m_positionX, this->m_config->m_positionY);
    glfwSetWindowSizeLimits(m_handler, this->m_config->m_sizeMinLimitX, this->m_config->m_sizeMinLimitY, this->m_config->m_sizeMaxLimitX, this->m_config->m_sizeMaxLimitY);
    glfwSetWindowUserPointer(m_handler, this);

    glfwShowWindow(m_handler);

    setSwapInterval(this->m_config->m_swapInterval);

    glfwSetInputMode(m_handler, GLFW_STICKY_KEYS, this->m_config->m_enableStickyKeys);
    glfwSetInputMode(m_handler, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    // ------------- post create
    const GLFWvidmode* primaryVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    if(m_config->m_useHalfMonitor)
    {
        setSize(primaryVideoMode->width / 2, primaryVideoMode->height / 2);
    }

    if(m_config->m_centralizeWindow)
    {
        int wndSizeX = 0;
        int wndSizeY = 0;

        getSize(wndSizeX, wndSizeY);
        setPosition((primaryVideoMode->width - wndSizeX) / 2, (primaryVideoMode->height - wndSizeY) / 2);
    }
    // -------------------------
}

void Core::Main::Window::makeCurrent() noexcept
{
    Graphics::APIType apiType = CoreMain::getRenderer().getAPIType();
    if(apiType >= Graphics::SG_API_TYPE_GL4 && apiType <= Graphics::SG_API_TYPE_GLES3)
    {
        glfwMakeContextCurrent(m_handler);
    }
}

#pragma region Setters

void Core::Main::Window::setSize(const int& sizeX, const int& sizeY) noexcept
{
    m_config->m_sizeX = sizeX;
    m_config->m_sizeY = sizeY;

    glfwSetWindowSize(m_handler, sizeX, sizeY);
}

void Core::Main::Window::setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept
{
    m_config->m_sizeMinLimitX = sizeMinLimitX;
    m_config->m_sizeMinLimitY = sizeMinLimitY;

    m_config->m_sizeMaxLimitX = sizeMaxLimitX;
    m_config->m_sizeMaxLimitY = sizeMaxLimitY;

    glfwSetWindowSizeLimits(m_handler, sizeMinLimitX, sizeMinLimitY, sizeMaxLimitX, sizeMaxLimitY);
}

void Core::Main::Window::setPosition(const int& posX, const int& posY) noexcept
{
    m_config->m_positionX = posX;
    m_config->m_positionY = posY;

    glfwSetWindowPos(m_handler, posX, posY);
}

void Core::Main::Window::setTitle(const std::string& title) noexcept
{
    m_config->m_title = title;

    glfwSetWindowTitle(m_handler, title.c_str());
}

void Core::Main::Window::setSwapInterval(const bool& swapInterval) noexcept
{
    m_config->m_swapInterval = swapInterval;

    Graphics::APIType apiType = CoreMain::getRenderer().getAPIType();
    if(apiType >= Graphics::SG_API_TYPE_GL4 && apiType <= Graphics::SG_API_TYPE_GLES3)
    {
        glfwSwapInterval(swapInterval);
    }
}

void Core::Main::Window::setEnableStickyKeys(const bool& enableStickyKeys) noexcept
{
    m_config->m_enableStickyKeys = enableStickyKeys;

    glfwSetInputMode(m_handler, GLFW_STICKY_KEYS, enableStickyKeys);
}

void Core::Main::Window::setHideAndCentralizeCursor(const bool& hideAndCentralizeCursor) noexcept
{
    m_config->m_hideAndCentralizeCursor = hideAndCentralizeCursor;

    glfwSetInputMode(m_handler, GLFW_CURSOR, hideAndCentralizeCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
}

bool Core::Main::Window::isHideAndCentralizeCursor() noexcept
{
    return m_config->m_hideAndCentralizeCursor;
}

void Core::Main::Window::setConfig(const std::shared_ptr<WindowConfig>& other) noexcept
{
    m_config = other;
}

void Core::Main::Window::setShouldClose(const bool& shouldClose) noexcept
{
    glfwSetWindowShouldClose(m_handler, shouldClose);
}

void Core::Main::Window::setCursorPosition(const double& x, const double& y) noexcept
{
    glfwSetCursorPos(m_handler, x, y);
}

#pragma endregion

#pragma region Getters

void Core::Main::Window::getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept
{
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPhysicalSize(primaryMonitor, &sizeX, &sizeY);
}

bool Core::Main::Window::shouldClose() noexcept
{
    return glfwWindowShouldClose(m_handler);
}

std::shared_ptr<Core::Main::WindowConfig> Core::Main::Window::getConfig() noexcept
{
    return m_config;
}

void Core::Main::Window::getSize(int& sizeX, int& sizeY) noexcept
{
    glfwGetWindowSize(m_handler, &sizeX, &sizeY);
}

#pragma endregion

void Core::Main::Window::windowCloseCallback(GLFWwindow* window)
{
    sgCallWindowCloseCallback(window);

    SGC_INFO("GLFW window closed.");
}

void Core::Main::Window::windowIconifyCallback(GLFWwindow* window, int iconified)
{
    sgCallWindowIconifyCallback(window, iconified);

    SGC_INFO("GLFW window iconified.");
}

void Core::Main::Window::errorCallback(int errCode, const char* err_msg)
{
    SGC_ERROR("GLFW error (code " + std::to_string(errCode) + "): " + err_msg);
}

void Core::Main::Window::swapBuffers()
{
    glfwSwapBuffers(m_handler);
}

void Core::Main::Window::pollEvents()
{
    glfwPollEvents();
}
