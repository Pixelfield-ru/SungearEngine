//
// Created by stuka on 24.04.2023.
//

//#include "Core.h"

#include "Window.h"

void Core::Main::Window::create()
{
    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) { }

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    wnd = glfwCreateWindow(this->wnd_config.size_x, this->wnd_config.size_y, this->wnd_config.name.c_str(), nullptr, nullptr);

    if(wnd == nullptr)
    {
        Logging::c_printf(Logging::SG_INFO, "Failed to initialize GLFW Window!");
    }

    glfwSetWindowCloseCallback(wnd, window_close_callback);
    glfwSetWindowIconifyCallback(wnd, window_iconify_callback);

    glfwMakeContextCurrent(wnd);

    glfwSetWindowPos(wnd, this->wnd_config.position_x, this->wnd_config.position_y);

    glfwSwapInterval(this->wnd_config.swap_interval);

    glfwShowWindow(wnd);
}

void Core::Main::Window::proceed_frame()
{
    glfwSwapBuffers(wnd);
    glfwPollEvents();
}