#include <locale>

#include "Window.h"
#include "Core.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"
#include "SGCore/Memory/AssetManager.h"

void Core::Main::Core::start()
{
    system("chcp 65001");
    setlocale(1251, "ru");

    // core components init -------------
    InputManager::init();
    Memory::AssetManager::init();
    Logging::init();
    // ----------------------------------

    m_window.create();

    m_renderer = Graphics::API::GL::GL46::GL46Renderer::getInstance();
    m_renderer->init();

    std::shared_ptr<Utils::TimerCallback> globalTimerCallback = std::make_shared<Utils::TimerCallback>();

    globalTimerCallback->setDeltaUpdateFunction([](const long double& deltaTime) { deltaUpdate(deltaTime); });
    globalTimerCallback->setDestinationReachedFunction([]() {
        m_window.setTitle("Sungear Engine. FPS: " + std::to_string(m_globalTimer.getFramesPerDestination()));
    });

    m_globalTimer.addCallback(globalTimerCallback);

    Graphics::API::GL::GL46::GL46Renderer::getInstance()->checkForErrors();

    sgCallCoreInitCallback();

    update();
}

void Core::Main::Core::update()
{
    while(!m_window.shouldClose())
    {
        InputManager::startFrame();

        m_globalTimer.startFrame();

        glm::ivec2 windowSize;
        m_window.getSize(windowSize.x, windowSize.y);
        m_renderer->renderFrame(windowSize);

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

Core::Graphics::API::IRenderer& Core::Main::Core::getRenderer() noexcept
{
    return *m_renderer;
}
