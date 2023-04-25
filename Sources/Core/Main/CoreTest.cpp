//
// Created by stuka on 18.04.2023.
//

#include <iostream>

#include "../Graphics/API/OpenGL/GLRenderer.h"
#include "Core.h"

void sg_wnd_close_fun(GLFWwindow* wnd)
{
    std::cout << "closed =)" << std::endl;
}

void sg_core_init_fun()
{
    std::cout << "core initialized =)" << std::endl;
}

void sg_frame_post_render_fun()
{
    std::cout << "rendering =)" << std::endl;
}

void sg_wnd_iconify_fun(GLFWwindow* wnd, int iconified)
{
    std::cout << "window iconified =)" << std::endl;
}

int main()
{
    sg_set_window_close_callback(sg_wnd_close_fun);
    sg_set_core_init_callback(sg_core_init_fun);
    //sg_set_frame_post_render_callback(sg_frame_post_render_fun);
    sg_set_window_iconify_callback(sg_wnd_iconify_fun);

    Core::Main::Core::start();

    return 0;
}