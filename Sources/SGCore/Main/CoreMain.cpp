#include "CoreMain.h"

#include <locale>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGConsole/API/Console.h"
#include "SGCore/Utils/ShadersPaths.h"

#include "SGCore/Input/InputManager.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Physics/PhysicsWorld3D.h"

void SGCore::CoreMain::start()
{
    const auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream timeStringStream;
    timeStringStream << std::put_time(std::localtime(&in_time_t), "%Y_%m_%d_%H_%M_%S");

    auto currentSessionLogger = spdlog::basic_logger_mt("current_session", "logs/sg_log_" + timeStringStream.str() + ".txt");
    spdlog::set_default_logger(currentSessionLogger);

    // todo: move
    /*system("chcp 65001");
    setlocale(LC_ALL, "Russian");*/

    m_renderer = GL4Renderer::getInstance();
    // m_renderer = GL46Renderer::getInstance();
    //m_renderer = VkRenderer::getInstance();

    m_window.create();

    m_renderer->init();

    SGUtils::Singleton::getSharedPtrInstance<ShadersPaths>()->createDefaultPaths();
    InputManager::init();
    AssetManager::init();

    std::shared_ptr<SGUtils::TimerCallback> globalTimerCallback = std::make_shared<SGUtils::TimerCallback>();

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

    std::shared_ptr<SGUtils::TimerCallback> fixedTimerCallback = std::make_shared<SGUtils::TimerCallback>();

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

SGCore::Ref<SGCore::IRenderer> SGCore::CoreMain::getRenderer() noexcept
{
    return m_renderer;
}

SGUtils::Timer& SGCore::CoreMain::getRenderTimer() noexcept
{
    return m_renderTimer;
}

SGUtils::Timer& SGCore::CoreMain::getFixedTimer() noexcept
{
    return m_fixedTimer;
}
