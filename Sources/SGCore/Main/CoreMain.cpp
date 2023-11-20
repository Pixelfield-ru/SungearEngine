#include "CoreMain.h"

#include <locale>

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"
#include "SGCore/Graphics/API/Vulkan/VkRenderer.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGConsole/API/Console.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/Utils/ShadersPaths.h"

void SGCore::CoreMain::start()
{
    system("chcp 65001");
    setlocale(LC_ALL, "Russian");

    m_renderer = GL4Renderer::getInstance();
    //m_renderer = Graphics::VkRenderer::getInstance();

    m_window.create();

    m_renderer->init();

    // core components init -------------
    ShadersPaths::getMainInstance().createDefaultPaths();
    InputManager::init();
    AssetManager::init();
    Log::init();
    ECSWorld::init();
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
    globalTimerCallback->setDestinationReachedFunction([]() {
        m_window.setTitle("Sungear Engine. FPS: " + std::to_string(m_renderTimer.getFramesPerDestination()));
    });

    m_renderTimer.addCallback(globalTimerCallback);
    m_renderTimer.m_targetFrameRate = 1200.0;

    // -----------------

    std::shared_ptr<TimerCallback> fixedTimerCallback = std::make_shared<TimerCallback>();

    fixedTimerCallback->setFixedUpdateFunction([]()
                                                {
                                                    fixedUpdate();
                                                });

    m_fixedTimer.m_targetFrameRate = 60;
    m_fixedTimer.m_useFixedUpdate = true;
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
