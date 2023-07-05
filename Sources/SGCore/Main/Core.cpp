#include <locale>

#include "Window.h"
#include "Core.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGConsole/API/Console.h"
#include "SGCore/ECS/ECSWorld.h"

void Core::Main::Core::start()
{
    system("chcp 65001");
    setlocale(1251, "ru");

    // core components init -------------
    InputManager::init();
    Memory::AssetManager::init();
    Logging::init();
    ECS::ECSWorld::init();
    // ----------------------------------

    m_window.create();

    m_renderer = Graphics::GL::GL46Renderer::getInstance();
    m_renderer->init();

    std::shared_ptr<Utils::TimerCallback> globalTimerCallback = std::make_shared<Utils::TimerCallback>();

    // delta update
    globalTimerCallback->setDeltaUpdateFunction([](const double& deltaTime) { deltaUpdate(deltaTime); });

    // update
    globalTimerCallback->setUpdateFunction([]() {
        update();
    });

    // when reached destination (in the case of this timer, 1 second) second
    globalTimerCallback->setDestinationReachedFunction([]() {
        m_window.setTitle("Sungear Engine. FPS: " + std::to_string(m_globalTimer.getFramesPerDestination()));
    });

    m_globalTimer.addCallback(globalTimerCallback);

    Graphics::GL::GL46Renderer::getInstance()->checkForErrors();

    sgCallCoreInitCallback();

    while(!m_window.shouldClose())
    {
        m_globalTimer.startFrame();
    }
}

void Core::Main::Core::update()
{
    glm::ivec2 windowSize;
    m_window.getSize(windowSize.x, windowSize.y);
    m_renderer->renderFrame(windowSize);

    sgCallFramePostRenderCallback();

    m_window.swapBuffers();
}

void Core::Main::Core::deltaUpdate(const long double& deltaTime)
{
    InputManager::startFrame();
    sgCallDeltaUpdateCallback(deltaTime);

    m_window.pollEvents();
}

Core::Main::Window& Core::Main::Core::getWindow() noexcept
{
    return m_window;
}

Core::Graphics::IRenderer& Core::Main::Core::getRenderer() noexcept
{
    return *m_renderer;
}
