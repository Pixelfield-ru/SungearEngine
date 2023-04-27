#include "Window.h"
#include "../Graphics/API/IRenderer.h"
#include "Core.h"
#include "../Graphics/API/OpenGL/GLRenderer.h"

void Core::Main::Core::start()
{
    window.create();

    renderer = new Graphics::API::OpenGL::GLRenderer;
    renderer->init(window);

    sgCallCoreInitCallback();

    renderer->startLoop();
}

Core::Main::Window& Core::Main::Core::getWindow() noexcept
{
    return window;
}

Core::Graphics::API::IRenderer& Core::Main::Core::getRenderer() noexcept
{
    return *renderer;
}