#include <locale>

#include "Window.h"
#include "Core.h"
#include "SGCore/Graphics/API/OpenGL/GLRenderer.h"
#include "SGCore/Memory/AssetManager.h"

void Core::Main::Core::start()
{
    system("chcp 65001");
    setlocale(1251, "ru");

    m_window.create();

    m_renderer = new Graphics::API::OpenGL::GLRenderer;
    m_renderer->init(m_window);

    sgCallCoreInitCallback();

    std::shared_ptr<Utils::TimerCallback> globalTimerCallback = std::make_shared<Utils::TimerCallback>();

    globalTimerCallback->setDeltaUpdateFunction([](const long double& deltaTime) { deltaUpdate(deltaTime); });
    globalTimerCallback->setDestinationReachedFunction([]() {
        m_window.setTitle("Sungear Engine. FPS: " + std::to_string(m_globalTimer.getFramesPerDestination()));
    });

    m_globalTimer.addCallback(globalTimerCallback);

    // core components init -------------
    InputManager::init();
    Memory::AssetManager::init();

    // найс это работает. TODO: убрать! просто ради теста
    std::shared_ptr<Memory::Assets::FileAsset> s = Memory::AssetManager::loadAsset<Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl");

    std::cout << s->getData() << std::endl;
    // ----------------------------------

    update();
}

void Core::Main::Core::update()
{
    while(!m_window.shouldClose())
    {
        m_globalTimer.startFrame();

        m_renderer->renderFrame();

        sgCallFramePostRenderCallback();

        m_globalTimer.endFrame();

        m_window.proceedFrame();
    }
}

void Core::Main::Core::deltaUpdate(const long double& deltaTime)
{
    //std::cout << deltaTime << std::endl;
}

Core::Main::Window& Core::Main::Core::getWindow() noexcept
{
    return m_window;
}

const Core::Graphics::API::IRenderer& Core::Main::Core::getRenderer() noexcept
{
    return *m_renderer;
}