#include "Window.h"
#include "Core.h"
#include "../Graphics/API/OpenGL/GLRenderer.h"
#include "../Utils/TimerCallback.h"

void Core::Main::Core::start()
{
    m_window.create();

    m_renderer = new Graphics::API::OpenGL::GLRenderer;
    m_renderer->init(m_window);

    sgCallCoreInitCallback();

    m_renderer->startLoop();
}

Core::Main::Window& Core::Main::Core::getWindow() noexcept
{
    return m_window;
}

Core::Graphics::API::IRenderer& Core::Main::Core::getRenderer() noexcept
{
    return *m_renderer;
}