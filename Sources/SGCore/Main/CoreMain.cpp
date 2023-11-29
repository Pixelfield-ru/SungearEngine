#include "CoreMain.h"

#include <locale>

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"
#include "SGCore/Graphics/API/Vulkan/VkRenderer.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGConsole/API/Console.h"
#include "SGCore/Utils/ShadersPaths.h"

void SGCore::CoreMain::start()
{
    // todo: move
    system("chcp 65001");
    setlocale(LC_ALL, "Russian");

    m_renderer = GL4Renderer::getInstance();
    //m_renderer = VkRenderer::getInstance();

    m_window.create();

    m_renderer->init();

    // core components init -------------
    SGSingleton::getSharedPtrInstance<ShadersPaths>()->createDefaultPaths();
    InputManager::init();
    AssetManager::init();
    Log::init();
    // ----------------------------------

    std::shared_ptr<TimerCallback> globalTimerCallback = std::make_shared<TimerCallback>();

    // delta update
    //globalTimerCallback->setDeltaUpdateFunction([](const double& deltaTime) { deltaUpdate(deltaTime); });

    // update
    globalTimerCallback->setUpdateFunction([]()
                                                {
                                                    update();
                                                });

    // when reached destination (in the case of this timer, 1 second) second
    globalTimerCallback->setFixedUpdateFunction([]() {
        m_window.setTitle("Sungear Engine. FPS: " + std::to_string(m_renderTimer.getFramesPerDestination()));
    });

    m_renderTimer.addCallback(globalTimerCallback);
    m_renderTimer.m_targetFrameRate = 1200.0;
    m_renderTimer.m_useFixedUpdateCatchUp = false;

    // -----------------

    std::shared_ptr<TimerCallback> fixedTimerCallback = std::make_shared<TimerCallback>();

    fixedTimerCallback->setFixedUpdateFunction([]()
                                                {
                                                    fixedUpdate();
                                                });

    m_fixedTimer.addCallback(fixedTimerCallback);

    //Graphics::GL::GL4Renderer::getInstance()->checkForErrors();

    sgCallCoreInitCallback();

    while(!m_window.shouldClose())
    {
        m_renderTimer.startFrame();
        m_fixedTimer.startFrame();
    }
}

void SGCore::CoreMain::fixedUpdate()
{
    InputManager::startFrame();

    sgCallFixedUpdateCallback();
}

void SGCore::CoreMain::update()
{
    glm::ivec2 windowSize;
    m_window.getSize(windowSize.x, windowSize.y);
    m_renderer->prepareFrame(windowSize);

    sgCallUpdateCallback();

    m_window.swapBuffers();

    m_window.pollEvents();
}

SGCore::Window& SGCore::CoreMain::getWindow() noexcept
{
    return m_window;
}

SGCore::IRenderer& SGCore::CoreMain::getRenderer() noexcept
{
    return *m_renderer;
}

SGCore::Timer& SGCore::CoreMain::getRenderTimer() noexcept
{
    return m_renderTimer;
}

SGCore::Timer& SGCore::CoreMain::getFixedTimer() noexcept
{
    return m_fixedTimer;
}
