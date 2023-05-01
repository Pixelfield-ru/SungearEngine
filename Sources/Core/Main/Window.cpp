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

    handler = glfwCreateWindow(this->wnd_config.size_x, this->wnd_config.size_y, this->wnd_config.name.c_str(), nullptr, nullptr);

    if(!handler)
    {
        Logging::consolePrintf(Logging::SG_INFO, "Failed to initialize GLFW Window!");
    }

    glfwSetWindowCloseCallback(handler, windowCloseCallback);
    glfwSetWindowIconifyCallback(handler, windowIconifyCallback);
    glfwSetKeyCallback(handler, InputManager::keyboardKeyCallback);
    glfwSetMouseButtonCallback(handler, InputManager::mouseButtonCallback);

    glfwMakeContextCurrent(handler);

    glfwSetWindowPos(handler, this->wnd_config.positionX, this->wnd_config.positionY);
    glfwSetWindowSizeLimits(handler, this->wnd_config.sizeMinLimitX, this->wnd_config.sizeMinLimitY, this->wnd_config.sizeMaxLimitX, this->wnd_config.sizeMaxLimitY);
    glfwSwapInterval(this->wnd_config.swapInterval);
    glfwSetWindowUserPointer(handler, this);

    glfwShowWindow(handler);

    glfwSetInputMode(handler, GLFW_STICKY_KEYS, this->wnd_config.enableStickyKeys);
    glfwSetInputMode(handler, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
}

void Core::Main::Window::proceedFrame()
{
    glfwSwapBuffers(handler);
    glfwPollEvents();
}