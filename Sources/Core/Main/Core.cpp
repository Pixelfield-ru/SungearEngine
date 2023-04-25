//
// Created by stuka on 17.04.2023.
//
#include "../Graphics/API/OpenGL/GLRenderer.h"
#include "Core.h"

void Core::Main::Core::start()
{
    window.create();

    renderer = new Graphics::API::OpenGL::GLRenderer;
    renderer->init(window);

    sg_call_core_init_callback();

    renderer->start_loop();
}