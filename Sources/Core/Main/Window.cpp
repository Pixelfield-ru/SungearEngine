#include "Window.h"

void Core::Main::Window::create()
{
    glfwSetErrorCallback(errorCallback);

    if(!glfwInit())
    {
        Logging::consolePrintf(Logging::SG_INFO, "Failed to initialize GLFW!");
    }

    Logging::consolePrintf(Logging::SG_INFO, "GLFW version is %i.%i.%i", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    handler = glfwCreateWindow(this->wndConfig.size_x, this->wndConfig.size_y, this->wndConfig.name.c_str(), nullptr, nullptr);

    if(!handler)
    {
        Logging::consolePrintf(Logging::SG_INFO, "Failed to initialize GLFW Window!");
    }

    glfwSetWindowCloseCallback(handler, windowCloseCallback);
    glfwSetWindowIconifyCallback(handler, windowIconifyCallback);
    glfwSetKeyCallback(handler, InputManager::keyboardKeyCallback);
    glfwSetMouseButtonCallback(handler, InputManager::mouseButtonCallback);

    glfwMakeContextCurrent(handler);

    glfwSetWindowPos(handler, this->wndConfig.positionX, this->wndConfig.positionY);
    glfwSetWindowSizeLimits(handler, this->wndConfig.sizeMinLimitX, this->wndConfig.sizeMinLimitY, this->wndConfig.sizeMaxLimitX, this->wndConfig.sizeMaxLimitY);
    glfwSwapInterval(this->wndConfig.swapInterval);
    glfwSetWindowUserPointer(handler, this);

    glfwShowWindow(handler);

    glfwSetInputMode(handler, GLFW_STICKY_KEYS, this->wndConfig.enableStickyKeys);
    glfwSetInputMode(handler, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
}

void Core::Main::Window::proceedFrame()
{
    glfwSwapBuffers(handler);
    glfwPollEvents();
}

void Core::Main::Window::setSize(const int& sizeX, const int& sizeY) noexcept
{
    wndConfig.size_x = sizeX;
    wndConfig.size_y = sizeY;

    glfwSetWindowSize(handler, sizeX, sizeY);
}

void Core::Main::Window::setPosition(const int& posX, const int& posY) noexcept
{
    wndConfig.positionX = posX;
    wndConfig.positionY = posY;

    glfwSetWindowSize(handler, posX, posY);
}

void Core::Main::Window::setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept
{
    wndConfig.sizeMinLimitX = sizeMinLimitX;
    wndConfig.sizeMinLimitY = sizeMinLimitY;

    wndConfig.sizeMaxLimitX = sizeMaxLimitX;
    wndConfig.sizeMaxLimitY = sizeMaxLimitY;

    glfwSetWindowSizeLimits(handler, sizeMinLimitX, sizeMinLimitY, sizeMaxLimitX, sizeMaxLimitY);
}

void Core::Main::Window::setSwapInterval(const bool& swapInterval) noexcept
{
    wndConfig.swapInterval = swapInterval;

    glfwSwapInterval(swapInterval);
}

void Core::Main::Window::setEnableStickyKeys(const bool& enableStickyKeys) noexcept
{
    wndConfig.enableStickyKeys = enableStickyKeys;

    glfwSetInputMode(handler, GLFW_STICKY_KEYS, enableStickyKeys);
}

inline void Core::Main::Window::getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept
{
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPhysicalSize(primaryMonitor, &sizeX, &sizeY);
}

void Core::Main::Window::windowCloseCallback(GLFWwindow* window)
{
    sgCallWindowCloseCallback(window);

    Logging::consolePrintf(Logging::MessageType::SG_INFO, "GLFW window closed.");
}

void Core::Main::Window::windowIconifyCallback(GLFWwindow* window, int iconified)
{
    sgCallWindowIconifyCallback(window, iconified);

    Logging::consolePrintf(Logging::MessageType::SG_INFO, "GLFW window iconified.");
}

void Core::Main::Window::errorCallback(int errCode, const char* err_msg)
{
    Logging::consolePrintf(Logging::SG_ERROR, "GLFW error (code %i): %s", errCode, err_msg);
}