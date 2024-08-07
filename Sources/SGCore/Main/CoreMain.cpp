#include "CoreMain.h"

#include <locale>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "SGCore/Utils/DefaultShadersPaths.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/PluginsSystem/PluginsManager.h"

void SGCore::CoreMain::start()
{
    const auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream timeStringStream;
    timeStringStream << std::put_time(std::localtime(&in_time_t), "%Y_%m_%d_%H_%M_%S");

    const std::string finalLogName = "logs/sg_log_" + timeStringStream.str() + ".log";

    try
    {
        std::filesystem::remove_all("ConsoleTmp");
    }
    catch(const std::exception& e)
    {
        std::printf("err: %s\n", e.what());
    }

    AssetManager::init();

    /*CrashHandler::hc_application_name = "Sungear Engine";
    CrashHandler::hc_log_file_output = finalLogName;
    CrashHandler::hc_install();*/

    HwExceptionHandler::setApplicationName("Sungear Engine");
    HwExceptionHandler::setOutputLogFilePath(finalLogName);
    HwExceptionHandler::setupHandler();

    m_defaultLogger = spdlog::basic_logger_mt("current_session", finalLogName);
    spdlog::set_default_logger(m_defaultLogger);

    spdlog::flush_on(spdlog::level::info);

    // todo: move
    /*system("chcp 65001");
    setlocale(LC_ALL, "Russian");*/

    m_renderer = GL4Renderer::getInstance();
    // m_renderer = GL46Renderer::getInstance();
    //m_renderer = VkRenderer::getInstance();

    m_window.create();

    SGCore::ImGuiWrap::ImGuiLayer::init();

    AudioDevice::init();
    AudioDevice::getDefaultDevice()->makeCurrent();

    m_renderer->init();

    DefaultShadersPaths::init();
    InputManager::init();
    FontsManager::init();

    m_renderTimer.onUpdate.connect<&updateStart>(0);
    m_renderTimer.onUpdate.connect<&updateEnd>(std::numeric_limits<size_t>::max());
    m_renderTimer.setTargetFrameRate(Window::getPrimaryMonitorRefreshRate());

    // -----------------

    m_fixedTimer.onUpdate.connect<&fixedUpdateStart>(0);
    m_fixedTimer.onUpdate.connect<&fixedUpdateEnd>(std::numeric_limits<size_t>::max());
    // m_fixedTimer.m_useFixedUpdateCatchUp = false;

    //Graphics::GL::GL4Renderer::getInstance()->checkForErrors();

    try
    {
        onInit();
    }
    catch(const std::exception& e)
    {
        std::string what = e.what();
        std::printf("Error while onInit. Error is: %s\n", what.c_str());
        spdlog::error("Error while onInit. Error is: {0}", what);
    }

    m_fixedTimer.resetTimer();
    m_renderTimer.resetTimer();

    // m_fixedTimerThread.detach();

    while (!m_window.shouldClose())
    {
        Threading::ThreadsManager::getMainThread()->processTasks();
        
        m_fixedTimer.startFrame();
        m_renderTimer.startFrame();
    }
    
    spdlog::shutdown();
}

void SGCore::CoreMain::fixedUpdateStart(const double& dt, const double& fixedDt)
{
    InputManager::startFrame();

    for(const auto& plugin : PluginsManager::getPlugins())
    {
        try
        {
            plugin->getPlugin()->fixedUpdate(dt, fixedDt);
        }
        catch(const std::exception& e)
        {
            std::string what = e.what();
            std::printf("Error while fixedUpdate plugin. Error is: %s\n", what.c_str());
            spdlog::error("Error while fixedUpdate plugin. Error is: {0}", what);
        }
    }
}

void SGCore::CoreMain::fixedUpdateEnd(const double& dt, const double& fixedDt)
{

}

void SGCore::CoreMain::updateStart(const double& dt, const double& fixedDt)
{
    glm::ivec2 windowSize;
    m_window.getSize(windowSize.x, windowSize.y);
    m_renderer->prepareFrame(windowSize);

    for(const auto& plugin : PluginsManager::getPlugins())
    {
        try
        {
            plugin->getPlugin()->update(dt, fixedDt);
        }
        catch(const std::exception& e)
        {
            std::string what = e.what();
            std::printf("Error while update plugin. Error is: %s\n", what.c_str());
            spdlog::error("Error while update plugin. Error is: {0}", what);
        }
    }
}

void SGCore::CoreMain::updateEnd(const double& dt, const double& fixedDt)
{
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

SGCore::Timer& SGCore::CoreMain::getRenderTimer() noexcept
{
    return m_renderTimer;
}

SGCore::Timer& SGCore::CoreMain::getFixedTimer() noexcept
{
    return m_fixedTimer;
}

std::uint16_t SGCore::CoreMain::getFPS() noexcept
{
    return m_renderTimer.getFramesPerSecond();
}