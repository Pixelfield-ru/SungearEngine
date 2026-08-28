#include "CoreMain.h"

#include <locale>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Config.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/Paths.h"

#include "SGCore/Graphics/API/GAPISelector.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Physics/PhysicsWorld3D.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/PluginsSystem/PluginsManager.h"
#include "SGCore/MetaInfo/MetaInfo.h"
#include "SGCore/Utils/StringInterpolation/InterpolationResolver.h"
#include "SGCore/Utils/SGSL/SGSLETranslator.h"
#include "SGCore/Input/PCInput.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/Serde/StandardSerdeSpecs/STD.h"
#include "SGCore/Serde/StandardSerdeSpecs/Utils.h"
#include "SGCore/ECS/Visitors.h"

SGCore::Signal<void()> SGCore::CoreMain::onInit;
std::filesystem::path SGCore::CoreMain::s_sungearEngineRootPath;
SGCore::Window SGCore::CoreMain::m_window;
SGCore::Ref<SGCore::IRenderer> SGCore::CoreMain::m_renderer;
std::atomic<bool> SGCore::CoreMain::m_shouldRestoreState { false };
std::atomic<bool> SGCore::CoreMain::m_isInitialized { false };
SGCore::Timer SGCore::CoreMain::m_renderTimer { true, 1200 };
SGCore::Timer SGCore::CoreMain::m_fixedTimer { true, 100 };

void SGCore::CoreMain::init()
{
    HwExceptionHandler::setApplicationName("Sungear Engine");
    HwExceptionHandler::setOutputLogFilePath(Utils::toUTF8(Logger::getDefaultLogger()->getLogFilePath()));
    HwExceptionHandler::setupHandler();
    
    // ================================================================================
    // ================================================================================
    // ================================================================================
    
    const char* sungearEngineRoot = std::getenv("SUNGEAR_SOURCES_ROOT");

    if(sungearEngineRoot)
    {
        if(std::strcmp(sungearEngineRoot, "") != 0)
        {
            s_sungearEngineRootPath = sungearEngineRoot;
        }
        else s_sungearEngineRootPath = ".";
    }
    else
    {
        s_sungearEngineRootPath = ".";
    }

    PathInterpolationMarkupSpec::setKey("enginePath", s_sungearEngineRootPath);

    SG_LOG_I("SGCore start...");
    SG_LOG_I("Removing tmp directories...");

    try
    {
        std::filesystem::remove_all("SGSLETranslatorOutputDebug");
        std::filesystem::remove_all("ConsoleTmp");
    }
    catch(const std::exception& e)
    {
        std::printf("err: %s\n", e.what());
    }

    SG_LOG_I("Registering standard meta info...");

    MetaInfo::addStandardMetaInfo();

    SG_LOG_I("Registering standard visitors for ECS components...");

    ECS::addStandardVisitors(ECS::VisitorsRegistry::instance());

    SGSLETranslator::includeDirectory(s_sungearEngineRootPath / "Resources");

    /*CrashHandler::hc_application_name = "Sungear Engine";
    CrashHandler::hc_log_file_output = finalLogName;
    CrashHandler::hc_install();*/

    // todo: move
    /*system("chcp 65001");
    setlocale(LC_ALL, "Russian");*/

    SG_LOG_I("Selecting graphics API and creating renderer...");

    m_renderer = GAPISelector::selectRenderer();

    if(!m_renderer)
    {
        SG_LOG_C("Engine can not start without a graphics API. Aborting initialization.");
        return;
    }

    SG_LOG_I("Creating window...");

    m_window.create();

    SGCore::ImGuiWrap::ImGuiLayer::init();

    SG_LOG_I("Initializing audio...");

    AudioDevice::init();
    AudioDevice::getDefaultDevice()->makeCurrent();

    SG_LOG_I("Initializing renderer...");

    m_renderer->init();

    // A backend that could not initialize asks for the window to close (GL4Renderer and VkRenderer
    // both do this when confirmSupport() fails). Initialization has to stop here too: everything
    // below creates GPU resources through the renderer, and a backend that just failed hands out
    // nullptr, so the run would abort on the first dereference instead of reporting the real reason.
    if(m_window.shouldClose())
    {
        SG_LOG_C("Renderer initialization failed, stopping engine initialization.");
        return;
    }

    SG_LOG_I("Adding standard assets...");

    AssetManager::getInstance()->addStandardAssets();

    SG_LOG_I("Initializing standard paths...");

    Paths::init();

    SG_LOG_I("Initializing standard fonts...");

    UI::FontsManager::getInstance().init();

    SG_LOG_I("Setting callbacks...");

    m_renderTimer.onPreUpdate = updateStart;
    m_renderTimer.onPostUpdate = updateEnd;
    m_renderTimer.setTargetFrameRate(Window::getPrimaryMonitorRefreshRate());

    // -----------------

    m_fixedTimer.onPreUpdate = fixedUpdateStart;
    m_fixedTimer.onPostUpdate = fixedUpdateEnd;
    // m_fixedTimer.m_useFixedUpdateCatchUp = false;

    Window::onFrameBufferSizeChanged += onFrameBufferResize;

    SG_LOG_I("Calling onInit signal...");

    onInit();

    m_fixedTimer.resetTimer();
    m_renderTimer.resetTimer();

    m_isInitialized = true;

    SG_LOG_I("SGCore was successfully initialized!");
}

void SGCore::CoreMain::startCycle() noexcept
{
    while (!m_window.shouldClose())
    {
        Threading::ThreadsManager::getMainThread()->processTasks();

        m_fixedTimer.startFrame();
        m_renderTimer.startFrame();

        // Coro::CoroScheduler::process();
    }

    if(m_renderer) m_renderer->shutdown();
    AudioDevice::shutdown();

    spdlog::shutdown();
}

SGCore::Config SGCore::CoreMain::loadConfig(const std::filesystem::path& configPath)
{
    if(!std::filesystem::exists(configPath))
    {
        SG_LOG_E("Config file does not exist. Path: '{}'", Utils::toUTF8(configPath));
        return{};
    }

    Config loadedConfig;
    std::string configLoadLog;
    Serde::Serializer::fromFormat(
        FileUtils::readFile(configPath),
        loadedConfig, Serde::FormatType::JSON, configLoadLog);

    if(!configLoadLog.empty())
    {
        SG_LOG_E(
              "Can not load config by path: '{}'.\nError: {}",
              SGCore::Utils::toUTF8(configPath),
              configLoadLog);
    }
    else
    {
        for(const auto& loadablePluginConfig : loadedConfig.m_loadablePlugins)
        {
            if(!loadablePluginConfig.m_isLoadable) continue;

            const auto pluginWrap = PluginsManager::loadPlugin(loadablePluginConfig.m_pluginName,
                                       loadablePluginConfig.m_pluginPath.resolved(),
                                       loadablePluginConfig.m_pluginEntryArgs,
                                       loadablePluginConfig.m_pluginCMakeBuildDir);

            pluginWrap->getPlugin()->m_isActive = loadablePluginConfig.m_isActive;
        }
    }

    return loadedConfig;
}

void SGCore::CoreMain::fixedUpdateStart(const double& dt, const double& fixedDt)
{
    Input::PC::startFrame();

    for(const auto& pluginWrap : PluginsManager::getPlugins())
    {
        if(!pluginWrap) continue;
        if(!pluginWrap->getPluginLib()->getNativeHandler()) continue;
        if(!pluginWrap->getPlugin()) continue;
        if(!pluginWrap->getPlugin()->m_isActive) continue;

        /*try
        {*/
        pluginWrap->getPlugin()->fixedUpdate(dt, fixedDt);
        /*}
        catch(const std::exception& e)
        {
            std::string what = e.what();
            SG_LOG_E("Error while fixedUpdate plugin. Error is: {}", what);
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

    for(const auto& pluginWrap : PluginsManager::getPlugins())
    {
        if(!pluginWrap) continue;
        if(!pluginWrap->getPluginLib()->getNativeHandler()) continue;
        if(!pluginWrap->getPlugin()) continue;
        if(!pluginWrap->getPlugin()->m_isActive) continue;

        /*try
        {*/
        pluginWrap->getPlugin()->update(dt, fixedDt);
        /*}
        catch(const std::exception& e)
        {
            std::string what = e.what();
            SG_LOG_E("Error while update plugin. Error is: {}", what);
        }*/
    }
}

void SGCore::CoreMain::updateEnd(const double& dt, const double& fixedDt)
{
    m_window.swapBuffers();
    m_window.pollEvents();

    // always restore state after window recreation
    if(m_shouldRestoreState)
    {
        restoreState();
        m_shouldRestoreState = false;
    }
}

void SGCore::CoreMain::onFrameBufferResize(SGCore::Window& window, const int& width, const int& height) noexcept
{
    if(Scene::getCurrentScene())
    {
        auto cameras3DView = Scene::getCurrentScene()->getECSRegistry()->view<Camera3D, RenderingBase>();

        cameras3DView.each([width, height](const Camera3D& camera, RenderingBase& renderingBase) {
            renderingBase.m_aspect = std::max((float) width, 10.0f) / std::max((float) height, 10.0f);
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

void SGCore::CoreMain::setShouldRestoreState(bool shouldRestore) noexcept
{
    m_shouldRestoreState = shouldRestore;
}

bool SGCore::CoreMain::isShouldRestoreState() noexcept
{
    return m_shouldRestoreState;
}

bool SGCore::CoreMain::isInitialized() noexcept
{
    return m_isInitialized;
}

void SGCore::CoreMain::restoreState() noexcept
{
    for(const auto& plugin : PluginsManager::getPlugins())
    {
        getRenderer()->reload();
        plugin->getPlugin()->restoreState();
    }
}
