//
// Created by stuka on 24.04.2023.
//

//#include "Core.h"

#include "Window.h"

void Core::Main::Window::create()
{
    glfwSetErrorCallback(errorCallback);

    if(!glfwInit()) { }

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    wnd = glfwCreateWindow(this->wnd_config.size_x, this->wnd_config.size_y, this->wnd_config.name.c_str(), nullptr, nullptr);

    if(wnd == nullptr)
    {
        Logging::consolePrintf(Logging::SG_INFO, "Failed to initialize GLFW Window!");
    }

    glfwSetWindowCloseCallback(wnd, windowCloseCallback);
    glfwSetWindowIconifyCallback(wnd, windowIconifyCallback);
    glfwSetKeyCallback(wnd, keyCallback);

    glfwMakeContextCurrent(wnd);

    glfwSetWindowPos(wnd, this->wnd_config.positionX, this->wnd_config.positionY);
    glfwSetWindowSizeLimits(wnd, this->wnd_config.sizeMinLimitX, this->wnd_config.sizeMinLimitY, this->wnd_config.sizeMaxLimitX, this->wnd_config.sizeMaxLimitY);
    glfwSwapInterval(this->wnd_config.swapInterval);
    glfwSetInputMode(wnd, GLFW_STICKY_KEYS, this->wnd_config.enableStickyKeys);

    glfwShowWindow(wnd);
}

void Core::Main::Window::proceedFrame()
{
    glfwSwapBuffers(wnd);
    glfwPollEvents();
}