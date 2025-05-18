#include "CoreMain.h"

#include <locale>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/Paths.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/Coro/CoroScheduler.h"
#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/PluginsSystem/PluginsManager.h"
#include "SGCore/MetaInfo/MetaInfo.h"
#include "SGCore/Utils/StringInterpolation/InterpolationResolver.h"
#include "SGCore/Utils/SGSL/SGSLETranslator.h"

void SGCore::CoreMain::start()
{
    const std::string finalLogFileName = "logs/sg_log_" + Utils::getTimeAsString("%Y_%m_%d_%H_%M_%S") + ".log";
    
    HwExceptionHandler::setApplicationName("Sungear Engine");
    HwExceptionHandler::setOutputLogFilePath(finalLogFileName);
    HwExceptionHandler::setupHandler();
    
    auto defaultLogger = Logger::createLogger("current_session", finalLogFileName);
    Logger::setDefaultLogger(defaultLogger);
    
    // ================================================================================
    // ================================================================================
    // ================================================================================
    
    const char* sungearEngineRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    if(!sungearEngineRoot)
    {
        const std::string errorMsg = "The 'SUNGEAR_SOURCES_ROOT' environment variable does not exist. "
                                     "Make sure that you have added 'SUNGEAR_SOURCES_ROOT' to the environment variables. "
                                     "In the variable value, specify the path to the Sungear Engine. "
                                     "Until then, you will not be able to build the project, as well as some other features of the engine.";

        LOG_C_UNFORMATTED(SGCORE_TAG, errorMsg)
        assert(errorMsg.c_str());
        std::exit(0);
    }
    else
    {
        const std::filesystem::path sungearEngineRootPath = sungearEngineRoot;
        const std::filesystem::path sungearEngineIncludeCMakeFile = sungearEngineRootPath / "cmake/SungearEngineInclude.cmake";
        if (!std::filesystem::exists(sungearEngineRootPath) ||
            !std::filesystem::exists(sungearEngineIncludeCMakeFile))
        {
            const std::string errorMsg = "The 'SUNGEAR_SOURCES_ROOT' environment variable contains an invalid value. "
                                         "Make sure that the 'SUNGEAR_SOURCES_ROOT' environment variable contains the correct value and indeed points to the Sungear Engine root folder. "
                                         "Until then, you will not be able to build the project, as well as some other features of the engine.\n"
                                         "Current value of 'SUNGEAR_SOURCES_ROOT': " + std::string(sungearEngineRoot);

            LOG_C_UNFORMATTED(SGCORE_TAG, errorMsg)
            assert(errorMsg.c_str());
            std::exit(0);
        }

        s_sungearEngineRootPath = sungearEngineRootPath;
    }

    PathInterpolationMarkup::getGlobalMarkup()["enginePath"] = s_sungearEngineRootPath;

    std::cout << "core init" << std::endl;

    try
    {
        std::filesystem::remove_all("ConsoleTmp");
    }
    catch(const std::exception& e)
    {
        std::printf("err: %s\n", e.what());
    }

    MetaInfo::addStandardMetaInfo();

    SGSLETranslator::includeDirectory(s_sungearEngineRootPath / "Resources");

    /*CrashHandler::hc_application_name = "Sungear Engine";
    CrashHandler::hc_log_file_output = finalLogName;
    CrashHandler::hc_install();*/

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

    AssetManager::init();

    Paths::init();
    InputManager::init();
    UI::FontsManager::getInstance().init();

    m_renderTimer.onPreUpdate = updateStart;
    m_renderTimer.onPostUpdate = updateEnd;
    m_renderTimer.setTargetFrameRate(Window::getPrimaryMonitorRefreshRate());

    // -----------------

    m_fixedTimer.onPreUpdate = fixedUpdateStart;
    m_fixedTimer.onPostUpdate = fixedUpdateEnd;
    // m_fixedTimer.m_useFixedUpdateCatchUp = false;

    Window::onFrameBufferSizeChanged += onFrameBufferResize;

    onInit();

    m_fixedTimer.resetTimer();
    m_renderTimer.resetTimer();

    while (!m_window.shouldClose())
    {
        Threading::ThreadsManager::getMainThread()->processTasks();
        
        m_fixedTimer.startFrame();
        m_renderTimer.startFrame();

        Coro::CoroScheduler::process();
    }
    
    spdlog::shutdown();
}

void SGCore::CoreMain::fixedUpdateStart(const double& dt, const double& fixedDt)
{
    InputManager::startFrame();

    for(const auto& plugin : PluginsManager::getPlugins())
    {
        /*try
        {*/
            if(plugin->getPlugin())
            {
                plugin->getPlugin()->fixedUpdate(dt, fixedDt);
            }
        /*}
        catch(const std::exception& e)
        {
            std::string what = e.what();
            LOG_E(SGCORE_TAG,
                  "Error while fixedUpdate plugin. Error is: {}", what);
        }*/
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
        /*try
        {*/
            if(plugin->getPlugin())
            {
                plugin->getPlugin()->update(dt, fixedDt);
            }
        /*}
        catch(const std::exception& e)
        {
            std::string what = e.what();
            LOG_E(SGCORE_TAG, "Error while update plugin. Error is: {}", what);
        }*/
    }
}

void SGCore::CoreMain::updateEnd(const double& dt, const double& fixedDt)
{
    m_window.swapBuffers();
    m_window.pollEvents();
}

void SGCore::CoreMain::onFrameBufferResize(SGCore::Window& window, const int& width, const int& height) noexcept
{
    if(Scene::getCurrentScene())
    {
        auto cameras3DView = Scene::getCurrentScene()->getECSRegistry()->view<Camera3D, RenderingBase>();

        cameras3DView.each([width, height](Camera3D::reg_t& camera, RenderingBase::reg_t renderingBase) {
            renderingBase->m_aspect = std::max((float) width, 10.0f) / std::max((float) height, 10.0f);
        });
    }
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

std::filesystem::path SGCore::CoreMain::getSungearEngineRootPath() noexcept
{
    return s_sungearEngineRootPath;
}
